#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <wx/wx.h>

#include <cstring>
#include <vector>

// Domínio "km" lógico (equivalente ao mouse_pos.c)
#define X_KM_MAX 1920.0f
#define Y_KM_MAX 1080.0f

static void converter_km_para_pixel(float km_x,
                                    float km_y,
                                    int screen_width,
                                    int screen_height,
                                    float* px,
                                    float* py)
{
    // Mapeamento linear: [0..X_KM_MAX] -> [0..screen_width]
    //                    [0..Y_KM_MAX] -> [0..screen_height]
    if (px)
        *px = (km_x / X_KM_MAX) * screen_width;
    if (py)
        *py = (km_y / Y_KM_MAX) * screen_height;
}

static void converter_pixel_para_km(float px,
                                    float py,
                                    int screen_width,
                                    int screen_height,
                                    float* km_x,
                                    float* km_y)
{
    if (km_x)
        *km_x = (px / (float)screen_width) * X_KM_MAX;
    if (km_y)
        *km_y = (py / (float)screen_height) * Y_KM_MAX;
}

static void resetar_posicoes(float* pos_km_x, float* pos_km_y)
{
    pos_km_x[0] = 0.0f;
    pos_km_y[0] = 0.0f;
    pos_km_x[1] = 0.0f;
    pos_km_y[1] = 0.0f;
}

static void armazenar_posicao_mouse(float mouse_x_px,
                                    float mouse_y_px,
                                    int screen_width,
                                    int screen_height,
                                    float* pos_km_x,
                                    float* pos_km_y,
                                    int index)
{
    float kmx = 0.0f, kmy = 0.0f;
    converter_pixel_para_km(
        mouse_x_px, mouse_y_px, screen_width, screen_height, &kmx, &kmy);
    pos_km_x[index] = kmx;
    pos_km_y[index] = kmy;
}

// Painel que integra Allegro (offscreen) ao wxWidgets
class AllegroPanel : public wxPanel
{
public:
    AllegroPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(640, 360)),
          m_w(640), m_h(360)
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &AllegroPanel::OnPaint, this);
        Bind(wxEVT_MOTION, &AllegroPanel::OnMouseMove, this);
        Bind(wxEVT_LEFT_DOWN, &AllegroPanel::OnLeftDown, this);
        Bind(wxEVT_SIZE, &AllegroPanel::OnResize, this);

        if (!al_init())
        {
            wxLogError("Falha em al_init()");
            return;
        }
        if (!al_init_primitives_addon())
        {
            wxLogError("Falha em al_init_primitives_addon()");
            return;
        }

        al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
        m_buffer = al_create_bitmap(m_w, m_h);
        if (!m_buffer)
        {
            wxLogError("Falha em al_create_bitmap()");
            return;
        }

        resetar_posicoes(m_pos_km_x, m_pos_km_y);

        // Fundo inicial
        al_set_target_bitmap(m_buffer);
        al_clear_to_color(al_map_rgb(20, 20, 20));
    }

    ~AllegroPanel() override
    {
        if (m_buffer)
        {
            al_destroy_bitmap(m_buffer);
            m_buffer = nullptr;
        }
        al_shutdown_primitives_addon();
        al_uninstall_system();
    }

private:
    int m_w, m_h;
    ALLEGRO_BITMAP* m_buffer = nullptr;

    // Estados do "mouse_pos.c": dois pontos em "km"
    float m_pos_km_x[2] = {0.0f, 0.0f};
    float m_pos_km_y[2] = {0.0f, 0.0f};
    int m_index = 0;  // qual ponto está sendo atualizado (0 ou 1)

    void OnResize(wxSizeEvent& evt)
    {
        wxSize s = evt.GetSize();
        m_w = std::max(1, s.GetWidth());
        m_h = std::max(1, s.GetHeight());

        // Recria o bitmap em memória com o novo tamanho
        ALLEGRO_BITMAP* newbuf = nullptr;
        al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
        newbuf = al_create_bitmap(m_w, m_h);
        if (!newbuf)
        {
            wxLogError("Falha ao redimensionar bitmap Allegro.");
            evt.Skip();
            return;
        }
        if (m_buffer)
            al_destroy_bitmap(m_buffer);
        m_buffer = newbuf;

        // Redesenha o fundo
        al_set_target_bitmap(m_buffer);
        al_clear_to_color(al_map_rgb(20, 20, 20));

        Refresh(false);
        evt.Skip();
    }

    void OnLeftDown(wxMouseEvent& e)
    {
        // Alterna entre ponto 0 e 1 a cada clique
        m_index = 1 - m_index;

        const wxPoint p = e.GetPosition();
        armazenar_posicao_mouse(
            (float)p.x, (float)p.y, m_w, m_h, m_pos_km_x, m_pos_km_y, m_index);
        Refresh(false);
    }

    void OnMouseMove(wxMouseEvent& e)
    {
        if (!e.Dragging() && !e.LeftIsDown())
        {
            // atualiza continuamente o ponto "corrente" mesmo sem arrastar
            const wxPoint p = e.GetPosition();
            armazenar_posicao_mouse((float)p.x,
                                    (float)p.y,
                                    m_w,
                                    m_h,
                                    m_pos_km_x,
                                    m_pos_km_y,
                                    m_index);
            Refresh(false);
        }
        else if (e.LeftIsDown())
        {
            // se segurando botão, arrasta o ponto corrente
            const wxPoint p = e.GetPosition();
            armazenar_posicao_mouse((float)p.x,
                                    (float)p.y,
                                    m_w,
                                    m_h,
                                    m_pos_km_x,
                                    m_pos_km_y,
                                    m_index);
            Refresh(false);
        }
    }

    void DesenharCenaAllegro()
    {
        al_set_target_bitmap(m_buffer);
        // limpa com leve transparência simulando "fade" discreto
        al_draw_filled_rectangle(0, 0, m_w, m_h, al_map_rgba(20, 20, 20, 255));

        // Converte os dois pontos (em km) para pixel do painel
        float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        converter_km_para_pixel(
            m_pos_km_x[0], m_pos_km_y[0], m_w, m_h, &x1, &y1);
        converter_km_para_pixel(
            m_pos_km_x[1], m_pos_km_y[1], m_w, m_h, &x2, &y2);

        // desenha linha entre os dois pontos
        al_draw_line(x1, y1, x2, y2, al_map_rgb(0, 255, 0), 2.0f);

        // desenha os pontos (círculos)
        al_draw_filled_circle(x1, y1, 6.0f, al_map_rgb(255, 80, 80));
        al_draw_filled_circle(x2, y2, 6.0f, al_map_rgb(80, 160, 255));
    }

    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(this);
        if (!m_buffer)
        {
            dc.Clear();
            return;
        }

        DesenharCenaAllegro();

        // Copia segura do bitmap Allegro (ABGR8888 com pitch) -> wxImage RGB
        ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(
            m_buffer, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READONLY);
        if (!lr || !lr->data)
        {
            if (lr)
                al_unlock_bitmap(m_buffer);
            wxLogError("Falha em al_lock_bitmap().");
            return;
        }

        const int src_bpp = 4;
        const int dst_bpp = 3;
        const int src_pitch = lr->pitch;
        const unsigned char* src = static_cast<const unsigned char*>(lr->data);

        std::vector<unsigned char> rgb(m_w * m_h * dst_bpp);

        for (int row = 0; row < m_h; ++row)
        {
            const unsigned char* s = src + row * src_pitch;
            unsigned char* d = rgb.data() + row * m_w * dst_bpp;
            for (int col = 0; col < m_w; ++col)
            {
                unsigned char A = s[0];
                unsigned char B = s[1];
                unsigned char G = s[2];
                unsigned char R = s[3];
                (void)A;
                d[0] = R;
                d[1] = G;
                d[2] = B;
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

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr,
                  wxID_ANY,
                  "Allegro + wxWidgets (mouse_pos adaptado)",
                  wxDefaultPosition,
                  wxSize(800, 480))
    {
        auto* panel = new AllegroPanel(this);
        auto* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(panel, 1, wxEXPAND | wxALL, 8);
        SetSizer(sizer);
        Centre();
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
