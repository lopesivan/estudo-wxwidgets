// g++ main.cpp `wx-config --cxxflags --libs std,adv,core,base` -O2
// -o app
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/wx.h>
#include <wx/dcgraph.h>   // wxGCDC
#include <wx/dcmemory.h>  // wxMemoryDC

#include <cmath>
#include <memory>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>
#include <utility>

// ===================== Skins disponíveis (arquivos)
// =========================
static const std::vector<wxString> kSkins = {"ball_blue.png",
                                             "ball_brown.png",
                                             "ball_coral.png",
                                             "ball_cyan.png",
                                             "ball_gold.png",
                                             "ball_gray.png",
                                             "ball_green.png",
                                             "ball_indigo.png",
                                             "ball_lime.png",
                                             "ball_magenta.png",
                                             "ball_orange.png",
                                             "ball_pink.png",
                                             "ball.png",  // default
                                             "ball_purple.png",
                                             "ball_red.png",
                                             "ball_silver.png",
                                             "ball_turquoise.png",
                                             "ball_violet.png",
                                             "ball_yellow.png"};

// ============================== Modelo da bola
// ==============================
struct Ball
{
    double   x, y;    // posição (px)
    double   vx, vy;  // velocidade (px/s)
    int      radius;  // raio (px)
    wxColour color;   // cor (fallback p/ sólido)
};

// ============================ Strategy interface
// ============================
class IBallPainter
{
  public:
    virtual ~IBallPainter()                    = default;
    virtual void Draw(wxDC& dc, const Ball& b) = 0;
};

// ========================= Painter: círculo sólido
// ==========================
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
            // Antialias usando wxGCDC sobre o wxAutoBufferedPaintDC
            // (wxMemoryDC)
            wxMemoryDC& memdc =
                static_cast<wxMemoryDC&>(dc);  // seguro aqui
            wxGCDC gcdc(memdc);
            gcdc.SetBrush(wxBrush(b.color));
            gcdc.SetPen(wxPen(b.color));
            const int x = (int)std::lround(b.x - b.radius);
            const int y = (int)std::lround(b.y - b.radius);
            const int d = 2 * b.radius;
            gcdc.DrawEllipse(x, y, d, d);
        }
    }

  private:
    bool m_antialias = false;
};

// =============================== SpriteCache
// ================================ Mantém bitmaps originais por
// nome e versões escaladas por (nome, diâmetro)
class SpriteCache
{
  public:
    static SpriteCache& Get()
    {
        static SpriteCache inst;
        return inst;
    }

    // Carrega todos os arquivos (silencioso se falhar; mantém só os
    // OK)
    void LoadAll(const std::vector<wxString>& files)
    {
        for(const auto& f : files)
        {
            if(m_original.count(f))
                continue;
            wxBitmap bmp;
            if(bmp.LoadFile(f, wxBITMAP_TYPE_PNG) && bmp.IsOk())
            {
                m_original.emplace(f, bmp);
            }
        }
    }

    bool Has(const wxString& name) const
    {
        return m_original.find(name) != m_original.end();
    }

    // Retorna uma referência a bitmap escalado para (name,
    // diameter)
    const wxBitmap& GetScaled(const wxString& name, int diameter)
    {
        auto key = std::make_pair(name, diameter);
        auto it  = m_scaled.find(key);
        if(it != m_scaled.end())
            return it->second;

        auto it0 = m_original.find(name);
        if(it0 == m_original.end())
        {
            // Se não existir, devolve um placeholder sólido 1x1
            // transparente
            static wxBitmap kEmpty(1, 1);
            return kEmpty;
        }
        wxImage img = it0->second.ConvertToImage();
        if(img.IsOk())
        {
            if(img.GetWidth() != diameter ||
               img.GetHeight() != diameter)
            {
                img = img.Rescale(
                    diameter, diameter, wxIMAGE_QUALITY_HIGH);
            }
            auto [pos, _] = m_scaled.emplace(key, wxBitmap(img));
            return pos->second;
        }
        return it0->second;  // fallback: original
    }

  private:
    std::unordered_map<wxString, wxBitmap>       m_original;
    std::map<std::pair<wxString, int>, wxBitmap> m_scaled;
};

// ============================ Painter: bitmap PNG
// ===========================
class BitmapPainter final : public IBallPainter
{
  public:
    explicit BitmapPainter(wxString skinName,
                           wxColour fallbackColor     = *wxBLUE,
                           bool     antialiasFallback = true)
        : m_skin(std::move(skinName)),
          m_fallbackColor(fallbackColor),
          m_circleFallback(antialiasFallback)
    {
    }

    void SetSkin(const wxString& skinName)
    {
        m_skin = skinName;
    }
    const wxString& GetSkin() const
    {
        return m_skin;
    }

    void Draw(wxDC& dc, const Ball& b) override
    {
        auto& cache = SpriteCache::Get();

        if(!cache.Has(m_skin))
        {
            // Fallback: desenha círculo se o PNG não existe
            Ball tmp  = b;
            tmp.color = m_fallbackColor;
            m_circleFallback.Draw(dc, tmp);
            return;
        }
        const int       diameter = 2 * b.radius;
        const wxBitmap& scaled = cache.GetScaled(m_skin, diameter);

        const int x = (int)std::lround(b.x) - scaled.GetWidth() / 2;
        const int y =
            (int)std::lround(b.y) - scaled.GetHeight() / 2;
        dc.DrawBitmap(scaled, x, y, true);  // alpha=true
    }

  private:
    wxString           m_skin;
    wxColour           m_fallbackColor;
    SolidCirclePainter m_circleFallback;
};

// ================================ Factory
// ===================================
enum class PainterKind
{
    Circle,
    Bitmap
};

class PainterFactory
{
  public:
    static std::shared_ptr<IBallPainter>
    Make(PainterKind kind, const wxString& skin = "ball.png")
    {
        switch(kind)
        {
        case PainterKind::Circle:
            return std::make_shared<SolidCirclePainter>(true);
        case PainterKind::Bitmap:
        {
            return std::make_shared<BitmapPainter>(
                skin, *wxBLUE, true);
        }
        }
        return std::make_shared<SolidCirclePainter>(true);
    }
};

// ================================ Painel
// ====================================
class BouncingBallPanel : public wxPanel
{
  public:
    explicit BouncingBallPanel(wxWindow* parent,
                               int       timerIntervalMs = 20)
        : wxPanel(parent, wxID_ANY), m_timer(this),
          m_timerIntervalMs(timerIntervalMs),
          m_activeKind(PainterKind::Bitmap),
          m_activeSkinIndex(DefaultSkinIndex()),
          m_activePainter(PainterFactory::Make(
              m_activeKind, kSkins[m_activeSkinIndex]))
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
        Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent&) {});

        SetCanFocus(true);
        Bind(wxEVT_KEY_DOWN, &BouncingBallPanel::OnKeyDown, this);

        UpdateStatus();
    }

    void SetActivePainter(PainterKind kind)
    {
        m_activeKind = kind;
        // se bitmap, preserve skin atual
        if(kind == PainterKind::Bitmap)
            m_activePainter = PainterFactory::Make(
                kind, kSkins[m_activeSkinIndex]);
        else
            m_activePainter = PainterFactory::Make(kind);
        UpdateStatus();
    }

    void SetActiveSkinIndex(std::size_t idx)
    {
        if(idx >= kSkins.size())
            return;
        m_activeSkinIndex = idx;
        if(m_activeKind == PainterKind::Bitmap)
        {
            m_activePainter = PainterFactory::Make(
                PainterKind::Bitmap, kSkins[m_activeSkinIndex]);
        }
        UpdateStatus();
    }

    void AddBall(const Ball& bModel)
    {
        BallWrap w;
        w.ball = bModel;
        w.painter =
            m_activePainter;  // strategy (com skin, se bitmap)
        m_balls.push_back(std::move(w));
    }

  private:
    struct BallWrap
    {
        Ball                          ball;
        std::shared_ptr<IBallPainter> painter;  // Strategy por bola
    };

    static std::size_t DefaultSkinIndex()
    {
        for(std::size_t i = 0; i < kSkins.size(); ++i)
            if(kSkins[i] == "ball.png")
                return i;
        return 0;
    }

    void OnPaint(wxPaintEvent&)
    {
        wxAutoBufferedPaintDC dc(this);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        for(const auto& w : m_balls)
            w.painter->Draw(dc, w.ball);
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
        case WXK_SPACE:
            m_paused = !m_paused;
            break;
        case '+':
        case WXK_NUMPAD_ADD:
            m_speedScale = std::min(8.0, m_speedScale * 1.25);
            break;
        case '-':
        case WXK_NUMPAD_SUBTRACT:
            m_speedScale = std::max(0.125, m_speedScale / 1.25);
            break;

        case 'C':
        case 'c':
            SetActivePainter(PainterKind::Circle);
            break;
        case 'B':
        case 'b':
            SetActivePainter(PainterKind::Bitmap);
            break;

        // Troca skin: anterior/próxima
        case '[':
        {
            if(m_activeSkinIndex == 0)
                m_activeSkinIndex = kSkins.size() - 1;
            else
                --m_activeSkinIndex;
            SetActiveSkinIndex(m_activeSkinIndex);
        }
        break;
        case ']':
        {
            m_activeSkinIndex =
                (m_activeSkinIndex + 1) % kSkins.size();
            SetActiveSkinIndex(m_activeSkinIndex);
        }
        break;

        // N: nova bola com strategy + skin atuais
        case 'N':
        case 'n':
        {
            Ball nb;
            nb.radius = FromDIP(wxSize(16, 16)).GetWidth();
            nb.color  = *wxColour(30, 144, 255);
            nb.x      = 30.0 + (double)(m_balls.size() * 22);
            nb.y      = 30.0 + (double)(m_balls.size() * 18);
            nb.vx     = 120.0 + 20.0 * (m_balls.size() % 5);
            nb.vy     = 100.0 + 30.0 * (m_balls.size() % 7);
            AddBall(nb);
        }
        break;

        // Dígitos 1..0 => escolhe skin por índice (1..10)
        default:
            if(code >= '0' && code <= '9')
            {
                int idx = (code == '0')
                              ? 9
                              : (code - '1');  // '0' => 10a posição
                if((std::size_t)idx < kSkins.size())
                    SetActiveSkinIndex((std::size_t)idx);
            }
            else
            {
                e.Skip();
            }
        }
        UpdateStatus();
    }

    void UpdateStatus()
    {
        if(auto* frame = wxDynamicCast(GetParent(), wxFrame))
        {
            wxString kind = (m_activeKind == PainterKind::Bitmap)
                                ? "Bitmap"
                                : "Circle";
            frame->SetStatusText(wxString::Format(
                "Painter: %s | Skin: %zu/%zu (%s) | Paused: %s | "
                "Speed: %.2fx | Balls: %zu",
                kind,
                m_activeSkinIndex + 1,
                kSkins.size(),
                kSkins[m_activeSkinIndex],
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
    std::size_t                   m_activeSkinIndex;
    std::shared_ptr<IBallPainter> m_activePainter;

    std::vector<BallWrap> m_balls;
};

// =============================== Frame / App
// ================================
class MyFrame : public wxFrame
{
  public:
    explicit MyFrame(const wxString& title)
        : wxFrame(nullptr,
                  wxID_ANY,
                  title,
                  wxDefaultPosition,
                  wxSize(700, 520))
    {
        CreateStatusBar();
        SetStatusText("B: Bitmap | C: Circle | [ / ]: Skin "
                      "prev/next | 1..0: escolher skin | N: nova "
                      "bola | Space +/-: pausa/velocidade");
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
        wxInitAllImageHandlers();
        // Pré-carrega todas as skins no início:
        SpriteCache::Get().LoadAll(kSkins);

        auto* frame =
            new MyFrame("wxWidgets - Strategy + Skins PNG (cache)");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
