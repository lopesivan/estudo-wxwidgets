// g++ main.cpp `wx-config --cxxflags --libs std,adv,core,base` -O2
// -o app
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/image.h>

#include <vector>
#include <memory>
#include <cmath>
#include <unordered_map>
#include <optional>

// -------------------------------------------
// Modelo de dados da bola
// -------------------------------------------
struct Ball
{
    double   x, y;    // posição (px)
    double   vx, vy;  // velocidade (px/s)
    int      radius;  // raio (px)
    wxColour color;   // cor “base” (usada por estratégias que
                      // desenham sólido)
};

// -------------------------------------------
// Strategy: interface para “como desenhar a bola”
// -------------------------------------------
class IBallPainter
{
  public:
    virtual ~IBallPainter()                    = default;
    virtual void Draw(wxDC& dc, const Ball& b) = 0;
};

// -------------------------------------------
// Estratégia 1: círculo sólido (vetorial, rápido)
// -------------------------------------------
class SolidCirclePainter final : public IBallPainter
{
  public:
    explicit SolidCirclePainter(bool antialias = false)
        : m_antialias(antialias)
    {
    }

    void Draw(wxDC& dc, const Ball& b) override
    {
        if(!m_antialias)
        {
            dc.SetBrush(wxBrush(b.color));
            dc.SetPen(wxPen(b.color));
            dc.DrawCircle((int)std::lround(b.x),
                          (int)std::lround(b.y),
                          b.radius);
        }
        else
        {
            // Antialias com GraphicsContext
            auto gc = wxGraphicsContext::Create(dc);
            if(gc)
            {
                gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
                gc->SetBrush(wxBrush(b.color));
                gc->SetPen(wxPen(b.color));
                const double d = 2.0 * b.radius;
                gc->DrawEllipse(
                    b.x - b.radius, b.y - b.radius, d, d);
                delete gc;
            }
        }
    }

  private:
    bool m_antialias = false;
};

// -------------------------------------------
// Estratégia 2: bitmap PNG (com cache por raio)
// -------------------------------------------
class BitmapPainter final : public IBallPainter
{
  public:
    // imagePath: caminho do PNG (com alpha). Se não encontrar, faz
    // fallback no círculo sólido.
    explicit BitmapPainter(wxString imagePath,
                           wxColour fallbackColor     = *wxBLUE,
                           bool     antialiasFallback = true)
        : m_imagePath(std::move(imagePath)),
          m_fallbackColor(fallbackColor),
          m_circleFallback(antialiasFallback)
    {
        // Tenta carregar a imagem uma vez
        if(m_bitmap.LoadFile(m_imagePath, wxBITMAP_TYPE_PNG) &&
           m_bitmap.IsOk())
        {
            m_loaded = true;
        }
    }

    void Draw(wxDC& dc, const Ball& b) override
    {
        if(!m_loaded)
        {
            // Fallback: desenha círculo se o PNG não existe
            Ball temp  = b;
            temp.color = m_fallbackColor;
            m_circleFallback.Draw(dc, temp);
            return;
        }

        // Recupera (ou gera) um bitmap escalado para o diâmetro
        // desta bola
        const int diameter = 2 * b.radius;
        wxBitmap  scaled   = GetScaledBitmap(diameter);
        const int x = (int)std::lround(b.x) - scaled.GetWidth() / 2;
        const int y =
            (int)std::lround(b.y) - scaled.GetHeight() / 2;

        // true => usa alpha
        dc.DrawBitmap(scaled, x, y, true);
    }

  private:
    wxBitmap GetScaledBitmap(int diameter)
    {
        auto it = m_cache.find(diameter);
        if(it != m_cache.end())
            return it->second;

        wxImage img = m_bitmap.ConvertToImage();
        if(img.IsOk())
        {
            if(img.GetWidth() != diameter ||
               img.GetHeight() != diameter)
            {
                img = img.Rescale(
                    diameter, diameter, wxIMAGE_QUALITY_HIGH);
            }
            wxBitmap scaled(img);
            m_cache.emplace(diameter, scaled);
            return scaled;
        }
        // Se algo der errado, devolve o original
        return m_bitmap;
    }

    wxString m_imagePath;
    bool     m_loaded = false;
    wxBitmap m_bitmap;
    std::unordered_map<int, wxBitmap>
        m_cache;  // cache por diâmetro

    wxColour           m_fallbackColor;
    SolidCirclePainter m_circleFallback;
};

// -------------------------------------------
// “Factory” simples para escolher a Strategy atual
// -------------------------------------------
enum class PainterKind
{
    Circle,
    Bitmap
};

class PainterFactory
{
  public:
    static std::shared_ptr<IBallPainter> Make(PainterKind kind)
    {
        switch(kind)
        {
        case PainterKind::Circle:
            return std::make_shared<SolidCirclePainter>(
                /*antialias=*/true);
        case PainterKind::Bitmap:
            // Ajuste o caminho do PNG aqui, se preferir outro
            return std::make_shared<BitmapPainter>(
                wxString("ball.png"),
                *wxBLUE,
                /*antialiasFallback=*/true);
        }
        return std::make_shared<SolidCirclePainter>(true);
    }
};

// -------------------------------------------
// Painel principal (animação + física + criação de bolas)
// Cada bola guarda seu próprio “painter” (Strategy).
// -------------------------------------------
class BouncingBallPanel : public wxPanel
{
  public:
    explicit BouncingBallPanel(wxWindow* parent,
                               int       timerIntervalMs = 20)
        : wxPanel(parent, wxID_ANY), m_timer(this),
          m_timerIntervalMs(timerIntervalMs),
          m_activeKind(PainterKind::Circle),
          m_activePainter(PainterFactory::Make(m_activeKind))
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        SetDoubleBuffered(true);

        // Uma bola inicial
        AddBall({50.0,
                 50.0,
                 160.0,
                 140.0,
                 FromDIP(wxSize(20, 20)).GetWidth(),
                 *wxBLUE});

        m_timer.Start(m_timerIntervalMs, wxTIMER_CONTINUOUS);

        Bind(wxEVT_PAINT, &BouncingBallPanel::OnPaint, this);
        Bind(wxEVT_TIMER, &BouncingBallPanel::OnTimer, this);
        Bind(wxEVT_SIZE, &BouncingBallPanel::OnSize, this);
        Bind(wxEVT_ERASE_BACKGROUND,
             [](wxEraseEvent&) { /* no-op */ });

        SetFocusable(true);
        Bind(wxEVT_KEY_DOWN, &BouncingBallPanel::OnKeyDown, this);
    }

    // Troca a estratégia ativa (novas bolas usarão esta)
    void SetActivePainter(PainterKind kind)
    {
        m_activeKind    = kind;
        m_activePainter = PainterFactory::Make(kind);
        // Mensagem de status (se houver barra de status no frame)
        if(auto* frame = wxDynamicCast(GetParent(), wxFrame))
        {
            wxString s = (kind == PainterKind::Circle)
                             ? "Strategy: Circle"
                             : "Strategy: Bitmap";
            frame->SetStatusText(s);
        }
    }

    // Cria uma nova bola usando a Strategy ativa
    void AddBall(const Ball& bModel)
    {
        BallWrap w;
        w.ball    = bModel;
        w.painter = m_activePainter;  // Strategy atual
        m_balls.push_back(std::move(w));
    }

  private:
    struct BallWrap
    {
        Ball                          ball;
        std::shared_ptr<IBallPainter> painter;  // Strategy por bola
    };

    void OnPaint(wxPaintEvent&)
    {
        wxAutoBufferedPaintDC dc(this);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();

        for(const auto& w : m_balls)
        {
            w.painter->Draw(dc, w.ball);
        }
    }

    void OnTimer(wxTimerEvent&)
    {
        if(m_paused)
            return;

        const double dt =
            (m_timerIntervalMs / 1000.0) * m_speedScale;
        const wxSize sz = GetClientSize();
        const int    w = sz.x, h = sz.y;

        for(auto& bw : m_balls)
        {
            auto& b = bw.ball;
            b.x += b.vx * dt;
            b.y += b.vy * dt;

            // colisão com as bordas
            if(b.x - b.radius < 0.0)
            {
                b.x  = (double)b.radius;
                b.vx = -b.vx;
            }
            else if(b.x + b.radius > w)
            {
                b.x  = (double)(w - b.radius);
                b.vx = -b.vx;
            }
            if(b.y - b.radius < 0.0)
            {
                b.y  = (double)b.radius;
                b.vy = -b.vy;
            }
            else if(b.y + b.radius > h)
            {
                b.y  = (double)(h - b.radius);
                b.vy = -b.vy;
            }
        }

        Refresh(false);
    }

    void OnSize(wxSizeEvent& e)
    {
        Refresh(false);
        e.Skip();
    }

    void OnKeyDown(wxKeyEvent& e)
    {
        const int code = e.GetKeyCode();
        switch(code)
        {
        case WXK_SPACE:  // pausa
            m_paused = !m_paused;
            UpdateStatus();
            break;

        case '+':
        case WXK_NUMPAD_ADD:
            m_speedScale = std::min(8.0, m_speedScale * 1.25);
            UpdateStatus();
            break;

        case '-':
        case WXK_NUMPAD_SUBTRACT:
            m_speedScale = std::max(0.125, m_speedScale / 1.25);
            UpdateStatus();
            break;

        case 'C':  // Strategy: círculo
        case 'c':
            SetActivePainter(PainterKind::Circle);
            break;

        case 'B':  // Strategy: bitmap
        case 'b':
            SetActivePainter(PainterKind::Bitmap);
            break;

        case 'N':  // nova bola usando a Strategy ativa
        case 'n':
        {
            Ball nb;
            nb.radius = FromDIP(wxSize(16, 16)).GetWidth();
            nb.color  = *wxColour(30, 144, 255);  // dodger blue
            nb.x      = 30.0 + (double)(m_balls.size() * 20);
            nb.y      = 30.0 + (double)(m_balls.size() * 15);
            nb.vx     = 120.0 + 20.0 * (m_balls.size() % 5);
            nb.vy     = 100.0 + 30.0 * (m_balls.size() % 7);
            AddBall(nb);
            UpdateStatus();
            break;
        }

        default:
            e.Skip();
        }
    }

    void UpdateStatus()
    {
        if(auto* frame = wxDynamicCast(GetParent(), wxFrame))
        {
            frame->SetStatusText(wxString::Format(
                "Paused: %s | Speed: %.3fx | Balls: %zu",
                m_paused ? "Yes" : "No",
                m_speedScale,
                m_balls.size()));
        }
    }

  private:
    wxTimer m_timer;
    int     m_timerIntervalMs = 20;
    bool    m_paused          = false;
    double  m_speedScale      = 1.0;

    PainterKind                   m_activeKind;
    std::shared_ptr<IBallPainter> m_activePainter;

    std::vector<BallWrap> m_balls;
};

// -------------------------------------------
// Frame / App
// -------------------------------------------
class MyFrame : public wxFrame
{
  public:
    explicit MyFrame(const wxString& title)
        : wxFrame(nullptr,
                  wxID_ANY,
                  title,
                  wxDefaultPosition,
                  wxSize(600, 480))
    {
        CreateStatusBar();
        SetStatusText("C: Circle | B: Bitmap | N: New ball | "
                      "Space: Pause | +/-: Speed");

        m_panel = new BouncingBallPanel(this);
        m_panel->SetFocus();
    }

  private:
    BouncingBallPanel* m_panel = nullptr;
};

class MyApp : public wxApp
{
  public:
    bool OnInit() override
    {
        // Necessário para PNGs
        wxInitAllImageHandlers();

        auto* frame =
            new MyFrame("wxWidgets - Strategy Pattern para bolas");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
