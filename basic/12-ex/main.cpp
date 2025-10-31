#include <wx/wx.h>
#include <vector>
#include <random>

// Estrutura para representar uma bola
struct Ball {
    double x, y;        // Posição
    double vx, vy;      // Velocidade
    double radius;      // Raio
    wxColour color;     // Cor
    
    Ball(double px, double py, double pvx, double pvy, double r, wxColour c)
        : x(px), y(py), vx(pvx), vy(pvy), radius(r), color(c) {}
};

// Painel customizado para desenhar as bolas
class BallPanel : public wxPanel {
private:
    std::vector<Ball> balls;
    wxTimer* timer;
    std::mt19937 rng;
    
    static constexpr double GRAVITY = 0.5;
    static constexpr double DAMPING = 0.98;  // Amortecimento ao quicar
    
public:
    BallPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
        // Configurar double buffering
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        
        // Criar timer para animação (30 FPS)
        timer = new wxTimer(this);
        timer->Start(33);  // ~30ms = 30 FPS
        
        // Inicializar gerador de números aleatórios
        rng.seed(std::random_device{}());
        
        // Criar bolas iniciais
        CreateBalls(10);
        
        // Conectar eventos
        Bind(wxEVT_PAINT, &BallPanel::OnPaint, this);
        Bind(wxEVT_TIMER, &BallPanel::OnTimer, this);
        Bind(wxEVT_LEFT_DOWN, &BallPanel::OnMouseClick, this);
        Bind(wxEVT_SIZE, &BallPanel::OnSize, this);
    }
    
    ~BallPanel() {
        timer->Stop();
        delete timer;
    }
    
private:
    void CreateBalls(int count) {
        wxSize size = GetClientSize();
        std::uniform_real_distribution<double> distX(50, size.GetWidth() - 50);
        std::uniform_real_distribution<double> distY(50, size.GetHeight() - 50);
        std::uniform_real_distribution<double> distV(-5, 5);
        std::uniform_int_distribution<int> distRadius(10, 30);
        std::uniform_int_distribution<int> distColor(0, 255);
        
        for (int i = 0; i < count; ++i) {
            double x = distX(rng);
            double y = distY(rng);
            double vx = distV(rng);
            double vy = distV(rng);
            double radius = distRadius(rng);
            wxColour color(distColor(rng), distColor(rng), distColor(rng));
            
            balls.emplace_back(x, y, vx, vy, radius, color);
        }
    }
    
    void OnPaint(wxPaintEvent& event) {
        wxPaintDC pdc(this);
        
        // Double buffering manual
        wxSize size = GetClientSize();
        wxBitmap bitmap(size.GetWidth(), size.GetHeight());
        wxMemoryDC dc(bitmap);
        
        // Limpar fundo
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        
        // Desenhar todas as bolas
        for (const auto& ball : balls) {
            dc.SetBrush(wxBrush(ball.color));
            dc.SetPen(wxPen(ball.color.ChangeLightness(80), 2));
            dc.DrawCircle(wxPoint(ball.x, ball.y), ball.radius);
        }
        
        // Desenhar instruções
        dc.SetTextForeground(*wxBLACK);
        dc.DrawText("Clique para adicionar bolas!", 10, 10);
        dc.DrawText(wxString::Format("Bolas: %zu", balls.size()), 10, 30);
        
        // Copiar buffer para tela
        pdc.Blit(0, 0, size.GetWidth(), size.GetHeight(), &dc, 0, 0);
    }
    
    void OnTimer(wxTimerEvent& event) {
        UpdateBalls();
        Refresh();  // Redesenhar
    }
    
    void UpdateBalls() {
        wxSize size = GetClientSize();
        
        for (auto& ball : balls) {
            // Aplicar gravidade
            ball.vy += GRAVITY;
            
            // Atualizar posição
            ball.x += ball.vx;
            ball.y += ball.vy;
            
            // Colisão com bordas horizontais
            if (ball.x - ball.radius < 0) {
                ball.x = ball.radius;
                ball.vx = -ball.vx * DAMPING;
            } else if (ball.x + ball.radius > size.GetWidth()) {
                ball.x = size.GetWidth() - ball.radius;
                ball.vx = -ball.vx * DAMPING;
            }
            
            // Colisão com bordas verticais
            if (ball.y - ball.radius < 0) {
                ball.y = ball.radius;
                ball.vy = -ball.vy * DAMPING;
            } else if (ball.y + ball.radius > size.GetHeight()) {
                ball.y = size.GetHeight() - ball.radius;
                ball.vy = -ball.vy * DAMPING;
                
                // Reduzir velocidade horizontal no chão (fricção)
                ball.vx *= 0.95;
            }
        }
    }
    
    void OnMouseClick(wxMouseEvent& event) {
        // Adicionar nova bola na posição do clique
        std::uniform_real_distribution<double> distV(-5, 5);
        std::uniform_int_distribution<int> distRadius(10, 30);
        std::uniform_int_distribution<int> distColor(0, 255);
        
        double x = event.GetX();
        double y = event.GetY();
        double vx = distV(rng);
        double vy = distV(rng);
        double radius = distRadius(rng);
        wxColour color(distColor(rng), distColor(rng), distColor(rng));
        
        balls.emplace_back(x, y, vx, vy, radius, color);
    }
    
    void OnSize(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }
};

// Frame principal
class MainFrame : public wxFrame {
public:
    MainFrame() : wxFrame(nullptr, wxID_ANY, "Bouncing Balls - wxWidgets", 
                          wxDefaultPosition, wxSize(800, 600)) {
        
        // Criar painel com as bolas
        BallPanel* panel = new BallPanel(this);
        
        // Criar menu
        wxMenuBar* menuBar = new wxMenuBar;
        wxMenu* fileMenu = new wxMenu;
        fileMenu->Append(wxID_EXIT, "Sair\tCtrl-Q");
        menuBar->Append(fileMenu, "Arquivo");
        SetMenuBar(menuBar);
        
        // Criar status bar
        CreateStatusBar();
        SetStatusText("Clique para adicionar bolas!");
        
        // Eventos
        Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
        
        Centre();
    }
    
private:
    void OnExit(wxCommandEvent& event) {
        Close(true);
    }
};

// Classe da aplicação
class BouncingBallsApp : public wxApp {
public:
    virtual bool OnInit() override {
        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(BouncingBallsApp);
