// g++ main.cpp `wx-config --cxxflags --libs std,adv,core,base` -O2 -o app
#include <wx/wx.h>
#include <wx/dcbuffer.h>   // wxAutoBufferedPaintDC
#include <vector>
#include <cmath>           // std::lround

struct Ball
{
    double x, y;     // posição (px)
    double vx, vy;   // velocidade (px/s)
    int    radius;   // raio em px (DIP convertido)
    wxColour color;
};

class BouncingBallPanel : public wxPanel
{
public:
    explicit BouncingBallPanel (wxWindow* parent, int timerIntervalMs = 20)
        : wxPanel (parent, wxID_ANY)
        , m_timer (this)
        , m_timerIntervalMs (timerIntervalMs)
    {
        // Evita erase implícito e habilita double-buffer interno
        SetBackgroundStyle (wxBG_STYLE_PAINT);
        SetDoubleBuffered (true);

        // Iniciar bolas (exemplo com 1 bola; adicione mais se quiser)
        const int r = FromDIP (wxSize (20, 20)).GetWidth();
        balls.push_back (Ball
        {
            50.0, 50.0,           // x, y
            160.0, 140.0,         // vx, vy em px/s
            r,
            *wxBLUE
        });

        // Timer contínuo
        m_timer.Start (m_timerIntervalMs, wxTIMER_CONTINUOUS);

        // Eventos
        Bind (wxEVT_PAINT, &BouncingBallPanel::OnPaint, this);
        Bind (wxEVT_TIMER, &BouncingBallPanel::OnTimer, this);
        Bind (wxEVT_SIZE,  &BouncingBallPanel::OnSize,  this);
        Bind (wxEVT_ERASE_BACKGROUND, [] (wxEraseEvent&) { /* no-op, 0 flicker */ });

        // Teclado no painel
        SetCanFocus (true);
        Bind (wxEVT_KEY_DOWN, &BouncingBallPanel::OnKeyDown, this);
    }

private:
    void OnPaint (wxPaintEvent&)
    {
        wxAutoBufferedPaintDC dc (this); // já desenha em buffer
        dc.SetBackground (*wxWHITE_BRUSH);
        dc.Clear();

        // Desenho simples (rápido). Se quiser antialias: use wxGraphicsContext.
        for (const auto& b : balls)
        {
            dc.SetBrush (wxBrush (b.color));
            dc.SetPen (wxPen (b.color));
            dc.DrawCircle (static_cast<int> (std::lround (b.x)),
                           static_cast<int> (std::lround (b.y)),
                           b.radius);
        }
    }

    void OnTimer (wxTimerEvent&)
    {
        if (m_paused) return;

        // dt em segundos a partir do intervalo de timer
        const double dt = (m_timerIntervalMs / 1000.0) * m_speedScale;

        const wxSize sz = GetClientSize();
        const int w = sz.x, h = sz.y;

        for (auto& b : balls)
        {
            b.x += b.vx * dt;
            b.y += b.vy * dt;

            // Colisão com paredes (elástica e estável)
            if (b.x - b.radius < 0.0)
            {
                b.x = static_cast<double> (b.radius);
                b.vx = -b.vx;
            }
            else if (b.x + b.radius > w)
            {
                b.x = static_cast<double> (w - b.radius);
                b.vx = -b.vx;
            }
            if (b.y - b.radius < 0.0)
            {
                b.y = static_cast<double> (b.radius);
                b.vy = -b.vy;
            }
            else if (b.y + b.radius > h)
            {
                b.y = static_cast<double> (h - b.radius);
                b.vy = -b.vy;
            }
        }

        // Pinta sem apagar o fundo (já usamos double-buffer)
        Refresh (false);
    }

    void OnSize (wxSizeEvent& e)
    {
        // Apenas repintar; nada de recriar bitmaps por frame
        Refresh (false);
        e.Skip();
    }

    void OnKeyDown (wxKeyEvent& e)
    {
        const int code = e.GetKeyCode();
        switch (code)
        {
        case WXK_SPACE: // pausa/retoma
            m_paused = !m_paused;
            break;
        case '+':
        case WXK_NUMPAD_ADD:
            m_speedScale = std::min (m_speedScale * 1.25, 8.0);
            break;
        case '-':
        case WXK_NUMPAD_SUBTRACT:
            m_speedScale = std::max (m_speedScale / 1.25, 0.125);
            break;
        default:
            e.Skip();
        }
    }

private:
    wxTimer m_timer;
    int     m_timerIntervalMs = 20; // ~50 FPS alvo
    bool    m_paused = false;
    double  m_speedScale = 1.0;

    std::vector<Ball> balls;
};

class MyFrame : public wxFrame
{
public:
    explicit MyFrame (const wxString& title)
        : wxFrame (nullptr, wxID_ANY, title, wxDefaultPosition, wxSize (FromDIP (wxSize (500, 450))))
    {
        auto* panel = new BouncingBallPanel (this);
        // Deixar o foco no painel para capturar teclas
        panel->SetFocus();
        CreateStatusBar();
        SetStatusText ("Espaço: Pausar | + / - : Velocidade");
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MyFrame ("wxWidgets Bouncing Balls (otimizado)");
        frame->Show (true);
        return true;
    }
};

wxIMPLEMENT_APP (MyApp);
