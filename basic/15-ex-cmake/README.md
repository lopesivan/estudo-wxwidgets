Ótima pergunta! **Não, a bola não é um PNG nem uma imagem carregada de arquivo.**
Ela é **desenhada diretamente na tela usando primitivas gráficas (desenho vetorial)** do próprio wxWidgets.

---

### ✅ **Como a bola é desenhada?**

No código, a bola é um círculo desenhado com `DrawCircle()` dentro do método `OnPaint`.
Esse círculo é preenchido com uma cor (`wxBrush`) e contornado com um pincel (`wxPen`).

Trecho do código:

```cpp
void OnPaint(wxPaintEvent&) {
    wxAutoBufferedPaintDC dc(this); // double-buffering automático
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    // Desenho de cada bola como um círculo
    for (const auto& b : balls) {
        dc.SetBrush(wxBrush(b.color));  // Cor de preenchimento
        dc.SetPen(wxPen(b.color));      // Cor da borda
        dc.DrawCircle(
            static_cast<int>(std::lround(b.x)),   // Centro X
            static_cast<int>(std::lround(b.y)),   // Centro Y
            b.radius                              // Raio
        );
    }
}
```

---

### 📎 **Então a bola é o quê exatamente?**

A estrutura da bola é apenas **dados na memória**, assim:

```cpp
struct Ball {
    double x, y;     // posição
    double vx, vy;   // velocidade
    int    radius;   // raio do círculo
    wxColour color;  // cor
};
```

E no construtor:

```cpp
const int r = FromDIP(wxSize(20, 20)).GetWidth();
balls.push_back(Ball{50.0, 50.0, 160.0, 140.0, r, *wxBLUE});
```

---

### 🖼️ **Se eu quiser que a bola seja uma imagem PNG, dá pra fazer?**

Sim, dá! Ao invés de desenhar um círculo com `DrawCircle()`, você poderia:

* Carregar uma imagem PNG com `wxBitmap` ou `wxImage`.
* Desenhar usando `dc.DrawBitmap()` no `OnPaint()`.
* A imagem pode até ser transparente.

Exemplo (opcional, se quiser imagem):

```cpp
wxBitmap ballImage{"ball.png", wxBITMAP_TYPE_PNG};

dc.DrawBitmap(ballImage, 
              static_cast<int>(std::lround(b.x - ballImage.GetWidth()/2)),
              static_cast<int>(std::lround(b.y - ballImage.GetHeight()/2)),
              true); // true para transparência (alpha)
```

---

### ✅ **Resumo**

| Pergunta                    | Resposta                                            |
| --------------------------- | --------------------------------------------------- |
| A bola é um PNG?            | ❌ Não, é desenhada como círculo com `DrawCircle()`. |
| Como ela é criada?          | Em memória com struct `Ball` + desenhada no DC.     |
| Posso usar imagem no lugar? | ✅ Sim, com `wxBitmap` + `DrawBitmap()`.             |

---

Se quiser, posso reescrever o código para a bola ser um PNG com sombra, transparência ou até sprites animados. Só me dizer!


# Solução para Erro `SetFocusable` no wxWidgets

O erro ocorre porque `SetFocusable()` foi introduzido no wxWidgets 3.1.5+, mas pode não estar disponível em todas as builds ou há um problema de compatibilidade.

## 🔧 Soluções

### Solução 1: Usar `SetCanFocus()` (Mais Compatível)

```cpp
// Em vez de:
SetFocusable(true);

// Use:
SetCanFocus(true);
```

### Solução 2: Verificação Condicional de Versão

```cpp
#include <wx/version.h>

// No construtor:
#if wxCHECK_VERSION(3, 1, 5)
    SetFocusable(true);
#else
    SetCanFocus(true);
#endif
```

### Solução 3: Código Completo Corrigido

Vou mostrar o código completo do painel com a correção:

```cpp
#include <wx/wx.h>
#include <wx/timer.h>
#include <vector>

// Estrutura para representar uma bola
struct Ball {
    double x, y;           // Posição
    double vx, vy;         // Velocidade
    double radius;         // Raio
    wxColour color;        // Cor
};

class BouncingBallPanel : public wxPanel {
private:
    std::vector<Ball> balls;
    wxTimer* timer;
    static constexpr double GRAVITY = 0.5;
    static constexpr double DAMPING = 0.98;
    static constexpr double FLOOR_BOUNCE = 0.8;

public:
    BouncingBallPanel(wxWindow* parent, int id = wxID_ANY)
        : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
        
        // CORREÇÃO: Use SetCanFocus ao invés de SetFocusable
        #if wxCHECK_VERSION(3, 1, 5)
            SetFocusable(true);
        #else
            SetCanFocus(true);
        #endif
        
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        SetBackgroundColour(*wxWHITE);

        // Cria algumas bolas iniciais
        AddBall(100, 100, 5, -3, 20, *wxRED);
        AddBall(200, 150, -4, 2, 25, *wxBLUE);
        AddBall(300, 80, 3, 4, 30, *wxGREEN);

        // Timer para animação (60 FPS)
        timer = new wxTimer(this);
        timer->Start(1000 / 60);

        // Event handlers
        Bind(wxEVT_PAINT, &BouncingBallPanel::OnPaint, this);
        Bind(wxEVT_TIMER, &BouncingBallPanel::OnTimer, this);
        Bind(wxEVT_LEFT_DOWN, &BouncingBallPanel::OnLeftClick, this);
        Bind(wxEVT_SIZE, &BouncingBallPanel::OnSize, this);
    }

    ~BouncingBallPanel() {
        timer->Stop();
        delete timer;
    }

    void AddBall(double x, double y, double vx, double vy, double radius, const wxColour& color) {
        balls.push_back({x, y, vx, vy, radius, color});
    }

private:
    void OnPaint(wxPaintEvent& event) {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        // Desenha todas as bolas
        for (const auto& ball : balls) {
            dc.SetBrush(wxBrush(ball.color));
            dc.SetPen(wxPen(ball.color.ChangeLightness(80), 2));
            dc.DrawCircle(
                static_cast<int>(ball.x),
                static_cast<int>(ball.y),
                static_cast<int>(ball.radius)
            );
        }
    }

    void OnTimer(wxTimerEvent& event) {
        wxSize size = GetSize();

        for (auto& ball : balls) {
            // Aplica gravidade
            ball.vy += GRAVITY;

            // Aplica damping (resistência do ar)
            ball.vx *= DAMPING;
            ball.vy *= DAMPING;

            // Atualiza posição
            ball.x += ball.vx;
            ball.y += ball.vy;

            // Colisão com paredes laterais
            if (ball.x - ball.radius < 0) {
                ball.x = ball.radius;
                ball.vx = -ball.vx * 0.9;
            } else if (ball.x + ball.radius > size.GetWidth()) {
                ball.x = size.GetWidth() - ball.radius;
                ball.vx = -ball.vx * 0.9;
            }

            // Colisão com teto
            if (ball.y - ball.radius < 0) {
                ball.y = ball.radius;
                ball.vy = -ball.vy * 0.9;
            }

            // Colisão com chão
            if (ball.y + ball.radius > size.GetHeight()) {
                ball.y = size.GetHeight() - ball.radius;
                ball.vy = -ball.vy * FLOOR_BOUNCE;
                
                // Para a bola se estiver quase parada
                if (std::abs(ball.vy) < 0.5) {
                    ball.vy = 0;
                    ball.y = size.GetHeight() - ball.radius;
                }
            }
        }

        Refresh();
    }

    void OnLeftClick(wxMouseEvent& event) {
        // Adiciona nova bola onde o usuário clicou
        wxPoint pos = event.GetPosition();
        
        // Velocidade aleatória
        double vx = (rand() % 10 - 5);
        double vy = -(rand() % 10 + 5);
        
        // Cor aleatória
        wxColour color(rand() % 256, rand() % 256, rand() % 256);
        
        AddBall(pos.x, pos.y, vx, vy, 20, color);
    }

    void OnSize(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }
};

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Bouncing Balls", 
                        wxDefaultPosition, wxSize(800, 600)) {
        
        // Cria o painel principal
        BouncingBallPanel* panel = new BouncingBallPanel(this);
        
        // Barra de status
        CreateStatusBar();
        SetStatusText("Click to add balls!");
        
        Centre();
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
```

## 📋 CMakeLists.txt Compatível

```cmake
cmake_minimum_required(VERSION 3.20)
project(BouncingBalls VERSION 1.0.0 LANGUAGES CXX)

# Padrão C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets not found!")
endif()

# Inclui configurações do wxWidgets
include(${wxWidgets_USE_FILE})

# Cria o executável
add_executable(app main.cpp)

# Link com wxWidgets
target_link_libraries(app PRIVATE ${wxWidgets_LIBRARIES})

# Flags de compilação
target_compile_options(app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g>
)

# Informações de debug
message(STATUS "=== wxWidgets Configuration ===")
message(STATUS "wxWidgets version: ${wxWidgets_VERSION_STRING}")
message(STATUS "wxWidgets libraries: ${wxWidgets_LIBRARIES}")
message(STATUS "wxWidgets include dirs: ${wxWidgets_INCLUDE_DIRS}")
```

## 🔍 Verificação da Versão do wxWidgets

Você pode verificar qual versão está usando:

```cpp
#include <wx/version.h>
#include <iostream>

int main() {
    std::cout << "wxWidgets version: " 
              << wxMAJOR_VERSION << "."
              << wxMINOR_VERSION << "."
              << wxRELEASE_NUMBER << std::endl;
    return 0;
}
```

## 💡 Alternativas ao SetFocusable/SetCanFocus

Se nenhum funcionar, você pode simplesmente remover essa linha. Ela é usada para permitir que o painel receba foco do teclado, mas não é essencial para o funcionamento básico:

```cpp
BouncingBallPanel(wxWindow* parent, int id = wxID_ANY)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    
    // Simplesmente remova ou comente essa linha se continuar dando erro
    // SetFocusable(true);
    
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxWHITE);
    
    // ... resto do código
}
```

## 🎯 Resumo das Soluções

1. ✅ **Mais simples**: Remova a linha `SetFocusable(true);`
2. ✅ **Compatível**: Use `SetCanFocus(true);`
3. ✅ **Completa**: Use verificação de versão com `#if wxCHECK_VERSION`

## 🚀 Compile e Teste

```bash
# Limpe e recompile
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/app
```

Qual solução funcionou melhor para você? 😊


