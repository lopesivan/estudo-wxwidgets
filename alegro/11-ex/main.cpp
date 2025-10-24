#include <wx/wx.h>
#include <wx/frame.h>
#include <vector>
#include <cstring>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#define X_KM_MAX 1920.0f
#define Y_KM_MAX 1080.0f

// ---- Assinaturas iguais ao mouse_pos.c ----
void armazenar_posicao_mouse(float mouse_x, float mouse_y, int screen_width,
                             int screen_height, float* pos_km_x,
                             float* pos_km_y, int index);
void converter_km_para_pixel(float km_x, float km_y, int screen_width,
                             int screen_height, float* px, float* py);
float calcular_modulo_vetor(float x0, float y0, float x1, float y1);
void desenhar_linha_conectando_dois_pontos(float* pos_km_x, float* pos_km_y,
                                           int screen_width,
                                           int screen_height);
void resetar_posicoes(float* pos_km_x, float* pos_km_y);

// ---- Implementação wxWidgets + Allegro (offscreen) ----
class AllegroPanel : public wxPanel {
public:
    AllegroPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720)),
          m_w(1280), m_h(720)
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        SetFocus(); // para receber teclas
        Bind(wxEVT_PAINT, &AllegroPanel::OnPaint, this);
        Bind(wxEVT_MOTION, &AllegroPanel::OnMouseMove, this);
        Bind(wxEVT_LEFT_DOWN, &AllegroPanel::OnLeftDown, this);
        Bind(wxEVT_SIZE, &AllegroPanel::OnResize, this);
        // Captura teclas mesmo sem foco direto em child: CHAR_HOOK no painel
        Bind(wxEVT_CHAR_HOOK, &AllegroPanel::OnCharHook, this);

        if (!al_init()) {
            wxLogError("Falha ao iniciar Allegro.");
            return;
        }
        al_install_mouse();
        al_install_keyboard();
        al_init_font_addon();
        al_init_ttf_addon();
        if (!al_init_primitives_addon()) {
            wxLogError("Falha ao iniciar primitives addon.");
            return;
        }

        al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
        m_buffer = al_create_bitmap(m_w, m_h);
        if (!m_buffer) {
            wxLogError("Falha ao criar bitmap Allegro.");
            return;
        }

        // Fonte personalizada como no mouse_pos.c, com fallback
        m_font = al_load_font("SauceCodeProNerdFontMono-Regular.ttf", 20, 0);
        if (!m_font) {
            wxLogWarning("Fonte personalizada não encontrada. Usando fonte padrão.");
            m_font = al_create_builtin_font();
            if (!m_font) {
                wxLogError("Falha ao carregar fonte padrão.");
            }
        }

        resetar_posicoes(m_pos_km_x, m_pos_km_y);

        al_set_target_bitmap(m_buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    ~AllegroPanel() override {
        if (m_font) { al_destroy_font(m_font); m_font = nullptr; }
        if (m_buffer) { al_destroy_bitmap(m_buffer); m_buffer = nullptr; }
        al_shutdown_primitives_addon();
        al_uninstall_system();
    }

private:
    int m_w, m_h;
    ALLEGRO_BITMAP* m_buffer = nullptr;
    ALLEGRO_FONT*   m_font   = nullptr;

    // Estado portado do mouse_pos.c
    float m_mouse_x = 0.0f, m_mouse_y = 0.0f;
    int   m_points_stored = 0;
    float m_pos_km_x[2] = {0.0f, 0.0f};
    float m_pos_km_y[2] = {0.0f, 0.0f};

    void OnResize(wxSizeEvent& evt) {
        wxSize s = evt.GetSize();
        m_w = std::max(1, s.GetWidth());
        m_h = std::max(1, s.GetHeight());

        al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
        ALLEGRO_BITMAP* newbuf = al_create_bitmap(m_w, m_h);
        if (!newbuf) {
            wxLogError("Falha ao redimensionar bitmap Allegro.");
            evt.Skip();
            return;
        }
        if (m_buffer) al_destroy_bitmap(m_buffer);
        m_buffer = newbuf;

        al_set_target_bitmap(m_buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));

        Refresh(false);
        evt.Skip();
    }

    void OnMouseMove(wxMouseEvent& e) {
        m_mouse_x = static_cast<float>(e.GetX());
        m_mouse_y = static_cast<float>(e.GetY());
        // No mouse_pos.c, mover não dispara desenho, só SPACE; mas podemos redesenhar HUD.
        Refresh(false);
    }

    void OnLeftDown(wxMouseEvent&) {
        // mouse_pos.c usa SPACE; manteremos SPACE.
        // Se quiser também capturar pelo mouse, descomente abaixo:
        // handleStorePoint();
    }

    void OnCharHook(wxKeyEvent& e) {
        const int code = e.GetKeyCode();
        if (code == WXK_ESCAPE) {
            // Fecha a janela (equivalente a sair)
            if (auto* frame = wxDynamicCast(GetParent(), wxFrame)) {
                frame->Close();
            } else {
                GetTopLevelParent()->Close();
            }
            return;
        } else if (code == WXK_SPACE) {
            handleStorePoint();
            return;
        }
        e.Skip();
    }

    void handleStorePoint() {
        if (m_points_stored < 2) {
            armazenar_posicao_mouse(m_mouse_x, m_mouse_y, m_w, m_h,
                                    m_pos_km_x, m_pos_km_y, m_points_stored);
            m_points_stored++;
        } else {
            m_points_stored = 0;
            resetar_posicoes(m_pos_km_x, m_pos_km_y);
        }
        Refresh(false);
    }

    void drawScene() {
        al_set_target_bitmap(m_buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (!m_font) {
            // sem fonte, ainda desenhamos os pontos/linhas
        }

        char buffer[256];

        // Desenha pontos armazenados + texto
        for (int i = 0; i < m_points_stored; ++i) {
            float px = 0, py = 0;
            converter_km_para_pixel(m_pos_km_x[i], m_pos_km_y[i], m_w, m_h, &px, &py);
            al_draw_filled_circle(px, py, 10, al_map_rgb(255, 0, 0));

            if (m_font) {
                std::snprintf(buffer, sizeof(buffer),
                              "Posicao %d: X = %.2f km, Y = %.2f km",
                              i + 1, m_pos_km_x[i], m_pos_km_y[i]);
                al_draw_text(m_font, al_map_rgb(255, 255, 255), 20, 20 + i * 30, 0, buffer);
            }
        }

        // Se dois pontos, desenha linha e deltas/modulo, como no mouse_pos.c
        if (m_points_stored == 2) {
            desenhar_linha_conectando_dois_pontos(m_pos_km_x, m_pos_km_y, m_w, m_h);

            if (m_font) {
                int id = 2;
                std::snprintf(buffer, sizeof(buffer),
                              "Delta   %d: dX = %.2f km, dY = %.2f km",
                              id + 1, m_pos_km_x[1] - m_pos_km_x[0],
                              m_pos_km_y[1] - m_pos_km_y[0]);
                al_draw_text(m_font, al_map_rgb(255, 255, 255), 20, 20 + id * 30, 0, buffer);

                id = 3;
                std::snprintf(buffer, sizeof(buffer),
                              "Modulo   %d: u = %.2f km",
                              id + 1,
                              calcular_modulo_vetor(m_pos_km_x[0], m_pos_km_y[0],
                                                    m_pos_km_x[1], m_pos_km_y[1]));
                al_draw_text(m_font, al_map_rgb(255, 255, 255), 20, 20 + id * 30, 0, buffer);
            }

            // zerando as posicoes (como no mouse_pos.c)
            resetar_posicoes(m_pos_km_x, m_pos_km_y);
        }

        // HUD de instruções
        if (m_font) {
            al_draw_text(m_font, al_map_rgb(200, 200, 200), 20, m_h - 40, 0,
                         "[ESPACO] marca ponto (2 pontos = linha)    [ESC] sair");
        }
    }

    void OnPaint(wxPaintEvent&) {
        wxPaintDC dc(this);
        if (!m_buffer) {
            dc.Clear();
            return;
        }

        drawScene();

        // Copia Allegro (ABGR8888 + pitch) -> wxImage RGB 24bpp
        ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(
            m_buffer, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READONLY
        );
        if (!lr || !lr->data) {
            if (lr) al_unlock_bitmap(m_buffer);
            wxLogError("Falha em al_lock_bitmap().");
            return;
        }

        const int src_bpp = 4;
        const int dst_bpp = 3;
        const int src_pitch = lr->pitch;
        const unsigned char* src = static_cast<const unsigned char*>(lr->data);

        std::vector<unsigned char> rgb(m_w * m_h * dst_bpp);
        for (int row = 0; row < m_h; ++row) {
            const unsigned char* s = src + row * src_pitch;
            unsigned char* d = rgb.data() + row * m_w * dst_bpp;
            for (int col = 0; col < m_w; ++col) {
                unsigned char A = s[0];
                unsigned char B = s[1];
                unsigned char G = s[2];
                unsigned char R = s[3];
                (void)A;
                d[0] = R; d[1] = G; d[2] = B;
                s += src_bpp;
                d += dst_bpp;
            }
        }
        al_unlock_bitmap(m_buffer);

        wxImage image(m_w, m_h);
        std::memcpy(image.GetData(), rgb.data(), rgb.size());
        wxBitmap bmp(image);
        dc.DrawBitmap(bmp, 0, 0, false);
    }
};

class MyFrame : public wxFrame {
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Allegro + wxWidgets (mouse_pos + fonte)",
                  wxDefaultPosition, wxSize(1280, 720))
    {
        auto* panel = new AllegroPanel(this);
        auto* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(panel, 1, wxEXPAND | wxALL, 8);
        SetSizer(sizer);
        Centre();
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

// ---- Implementações das funções do mouse_pos.c ----
void armazenar_posicao_mouse(float mouse_x, float mouse_y, int screen_width,
                             int screen_height, float* pos_km_x,
                             float* pos_km_y, int index)
{
    float km_x = (mouse_x / screen_width) * X_KM_MAX;
    float km_y = Y_KM_MAX - ((mouse_y / screen_height) * Y_KM_MAX);
    pos_km_x[index] = km_x;
    pos_km_y[index] = km_y;
}

void converter_km_para_pixel(float km_x, float km_y, int screen_width,
                             int screen_height, float* px, float* py)
{
    *px = (km_x / X_KM_MAX) * screen_width;
    *py = screen_height - (km_y / Y_KM_MAX) * screen_height;
}

float calcular_modulo_vetor(float x0, float y0, float x1, float y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    return std::sqrt(dx * dx + dy * dy);
}

void desenhar_linha_conectando_dois_pontos(float* pos_km_x, float* pos_km_y,
                                           int screen_width,
                                           int screen_height)
{
    float x1, y1, x2, y2;
    converter_km_para_pixel(pos_km_x[0], pos_km_y[0], screen_width,
                            screen_height, &x1, &y1);
    converter_km_para_pixel(pos_km_x[1], pos_km_y[1], screen_width,
                            screen_height, &x2, &y2);
    al_draw_line(x1, y1, x2, y2, al_map_rgb(0, 255, 0), 2.0f);
}

void resetar_posicoes(float* pos_km_x, float* pos_km_y)
{
    pos_km_x[0] = 0;
    pos_km_y[0] = 0;
    pos_km_x[1] = 0;
    pos_km_y[1] = 0;
}
