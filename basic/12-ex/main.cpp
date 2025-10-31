#include <wx/wx.h>
#include <vector>
#include <random>

class Ball {
public:
    wxPoint position;
    wxPoint velocity;
    int radius;
    wxColour color;

    Ball(int x, int y, int r, const wxColour& c) 
        : position(x, y), radius(r), color(c) {
        // Gerar velocidades aleatórias
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-10, 10);
        
        velocity.x = dis(gen);
        velocity.y = dis(gen);
        
        // Garantir que a bola se mova
        if(velocity.x == 0) velocity.x = 5;
        if(velocity.y == 0) velocity.y = 5;
    }

    void Move(const wxSize& bounds) {
        position += velocity;

        // Colisão com bordas horizontais
        if (position.x - radius <= 0 || position.x + radius >= bounds.GetWidth()) {
            velocity.x = -velocity.x;
            position.x = wxMax(radius, wxMin(bounds.GetWidth() - radius, position.x));
        }

        // Colisão com bordas verticais
        if (position.y - radius <= 0 || position.y + radius >= bounds.GetHeight()) {
            velocity.y = -velocity.y;
            position.y = wxMax(radius, wxMin(bounds.GetHeight() - radius, position.y));
        }
    }

    void Draw(wxDC& dc) {
        dc.SetBrush(wxBrush(color));
        dc.SetPen(wxPen(*wxBLACK, 1));
        dc.DrawCircle(position, radius);
    }
};

class BouncingCanvas : public wxWindow {
public:
    std::vector<Ball> balls;
    wxTimer timer;

    BouncingCanvas(wxWindow* parent) : wxWindow(parent, wxID_ANY), timer(this) {
        // Criar 10 bolas aleatórias
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_radius(15, 30);
        std::uniform_int_distribution<> dis_color(0, 255);

        for (int i = 0; i < 10; ++i) {
            int r = dis_radius(gen);
            int x = dis_radius(gen) + r;
            int y = dis_radius(gen) + r;
            wxColour color(dis_color(gen), dis_color(gen), dis_color(gen));
            balls.emplace_back(x, y, r, color);
        }

        timer.Start(16); // ~60 FPS
        Bind(wxEVT_PAINT, &BouncingCanvas::OnPaint, this);
        Bind(wxEVT_TIMER, &BouncingCanvas::OnTimer, this);
        Bind(wxEVT_SIZE, &BouncingCanvas::OnSize, this);
    }

    void OnPaint(wxPaintEvent&) {
        wxBufferedPaintDC dc(this);
        Render(dc);
    }

    void OnTimer(wxTimerEvent&) {
        Refresh(); // Força o redesenho
    }

    void OnSize(wxSizeEvent&) {
        Refresh(); // Redesenha ao redimensionar
    }

    void Render(wxDC& dc) {
        // Limpar o fundo
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();

        // Atualizar e desenhar bolas
        for (auto& ball : balls) {
            ball.Move(GetClientSize());
            ball.Draw(dc);
        }
    }
};

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Bouncing Balls - wxWidgets") {
        new BouncingCanvas(this);
        SetClientSize(800, 600);
        Center();
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        new MyFrame();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
