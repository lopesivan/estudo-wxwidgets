#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cstring>
#include <vector>
#include <wx/wx.h>

class AllegroPanel : public wxPanel {
public:
  AllegroPanel(wxWindow *parent)
      : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 300)),
        m_w(400), m_h(300) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &AllegroPanel::OnPaint, this);

    if (!al_init()) {
      wxLogError("Falha em al_init()");
      return;
    }
    if (!al_init_primitives_addon()) {
      wxLogError("Falha em al_init_primitives_addon()");
      return;
    }

    // Garante criação de bitmap em memória (sem janela do Allegro)
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    m_buffer = al_create_bitmap(m_w, m_h);
    if (!m_buffer) {
      wxLogError("Falha em al_create_bitmap()");
      return;
    }

    // Fundo inicial e um círculo
    al_set_target_bitmap(m_buffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_circle(m_w / 2.0f, m_h / 2.0f, 60.0f, al_map_rgb(255, 0, 0));
  }

  ~AllegroPanel() override {
    if (m_buffer) {
      al_destroy_bitmap(m_buffer);
      m_buffer = nullptr;
    }
    al_shutdown_primitives_addon();
    al_uninstall_system();
  }

private:
  int m_w, m_h;
  ALLEGRO_BITMAP *m_buffer = nullptr;

  void OnPaint(wxPaintEvent &) {
    wxPaintDC dc(this);
    if (!m_buffer) {
      dc.Clear();
      return;
    }

    // Desenho “dinâmico” no bitmap do Allegro
    al_set_target_bitmap(m_buffer);
    int x = rand() % m_w;
    int y = rand() % m_h;
    al_draw_filled_circle((float)x, (float)y, 8.0f, al_map_rgb(0, 255, 0));

    // Lock para leitura em formato conhecido
    ALLEGRO_LOCKED_REGION *lr = al_lock_bitmap(
        m_buffer, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READONLY);
    if (!lr || !lr->data) {
      wxLogError("Falha em al_lock_bitmap()");
      if (lr)
        al_unlock_bitmap(m_buffer);
      return;
    }

    // Copiamos e convertimos ABGR(32bpp) -> RGB(24bpp), respeitando o pitch
    const int src_bpp = 4;
    const int dst_bpp = 3;
    const int src_pitch = lr->pitch;
    const unsigned char *src = static_cast<const unsigned char *>(lr->data);

    std::vector<unsigned char> rgb(m_w * m_h * dst_bpp);

    for (int row = 0; row < m_h; ++row) {
      const unsigned char *s = src + row * src_pitch;
      unsigned char *d = rgb.data() + row * m_w * dst_bpp;
      for (int col = 0; col < m_w; ++col) {
        // ABGR -> RGB
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
    std::memcpy(image.GetData(), rgb.data(),
                rgb.size()); // wxImage aloca o próprio buffer
    wxBitmap bmp(image);
    dc.DrawBitmap(bmp, 0, 0, false);
  }
};

class MyFrame : public wxFrame {
public:
  MyFrame()
      : wxFrame(nullptr, wxID_ANY, "Allegro + wxWidgets (estável)",
                wxDefaultPosition, wxSize(500, 400)) {
    auto *panel = new AllegroPanel(this);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel, 1, wxEXPAND | wxALL, 10);
    SetSizer(sizer);
    Centre();
  }
};

class MyApp : public wxApp {
public:
  bool OnInit() override {
    auto *frame = new MyFrame();
    frame->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);
