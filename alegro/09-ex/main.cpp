#include <wx/wx.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// Painel customizado com Allegro embutido
class AllegroPanel : public wxPanel {
public:
    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_BITMAP* buffer = nullptr;

    AllegroPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 300)) 
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &AllegroPanel::OnPaint, this);
        InitAllegro();
    }

    ~AllegroPanel() {
        if (buffer) al_destroy_bitmap(buffer);
        if (display) al_destroy_display(display);
        al_uninstall_system();
    }

private:
    void InitAllegro() {
        al_init();
        al_init_primitives_addon();

        // Cria display offscreen
        display = al_create_display(400, 300);
        buffer = al_create_bitmap(400, 300);

        al_set_target_bitmap(buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_filled_circle(200, 150, 60, al_map_rgb(255, 0, 0));
    }

    void OnPaint(wxPaintEvent&) {
        wxPaintDC dc(this);

        al_set_target_bitmap(buffer);
        al_draw_filled_circle(rand() % 400, rand() % 300, 10, al_map_rgb(0, 255, 0));

        // Copia bitmap Allegro para wxWidgets
        ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(buffer, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READONLY);
        wxImage image(400, 300, (unsigned char*)lr->data, true);
        al_unlock_bitmap(buffer);
        wxBitmap bmp(image);
        dc.DrawBitmap(bmp, 0, 0, false);
    }
};

// Janela principal
class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Allegro + wxWidgets", wxDefaultPosition, wxSize(500, 400)) {
        auto* panel = new AllegroPanel(this);
        auto* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(panel, 1, wxEXPAND | wxALL, 10);
        SetSizerAndFit(sizer);
    }
};

// Aplicativo
class MyApp : public wxApp {
public:
    bool OnInit() override {
        auto* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

