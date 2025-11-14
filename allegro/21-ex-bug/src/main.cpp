#include <wx/wx.h>

/* GTK / X11 headers */
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
extern "C" {
#include <X11/Xlib.h>
}

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/* ---------- Painel que hospeda o Allegro ---------- */
class PaintPanel : public wxPanel
{
public:
    PaintPanel(wxWindow* parent);
    ~PaintPanel() { Cleanup(); }

private:
    ALLEGRO_DISPLAY* alDisplay = nullptr;
    ALLEGRO_TIMER*   timer     = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;

    void InitAllegro();
    void Cleanup();
    void OnShow(wxShowEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnPaint(wxPaintEvent& evt);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(PaintPanel, wxPanel)
    EVT_SHOW(PaintPanel::OnShow)
    EVT_SIZE(PaintPanel::OnSize)
    EVT_PAINT(PaintPanel::OnPaint)
wxEND_EVENT_TABLE()

/* ---------- wxApp ---------- */
class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxFrame* frame = new wxFrame(nullptr, wxID_ANY,
                                     "wx + Allegro (GTK/X11)",
                                     wxDefaultPosition,
                                     wxSize(900,700));
        new PaintPanel(frame);
        frame->Show();
        return true;
    }
};
wxIMPLEMENT_APP(MyApp);

/* ---------- Implementação ---------- */
PaintPanel::PaintPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxFULL_REPAINT_ON_RESIZE)
{
    /* NÃO cria display aqui – aguarda EVT_SHOW */
}

void PaintPanel::OnShow(wxShowEvent& evt)
{
    if(evt.IsShown() && !alDisplay) InitAllegro();
    evt.Skip();
}

void PaintPanel::InitAllegro()
{
#if defined(__WXGTK__)
    GtkWidget* gtkWidget = (GtkWidget*)GetHandle();
    /* Garante que o widget foi realizado */
    if(!GTK_WIDGET_REALIZED(gtkWidget))
        gtk_widget_realize(gtkWidget);

    GdkWindow* gdkWindow = gtk_widget_get_window(gtkWidget);
    Window xid = GDK_WINDOW_XID(gdkWindow);

    /* Cria display “attached” ao XID */
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplay = al_create_display(1, 1);
    if(!alDisplay){ wxMessageBox("al_create_display falhou"); return; }

    al_resize_display(alDisplay, GetSize().x, GetSize().y);
#elif defined(__WXMSW__)
    HWND hwnd = (HWND)GetHandle();
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    alDisplay = al_create_display(1, 1);
    if(!alDisplay){ wxMessageBox("al_create_display falhou"); return; }
    al_resize_display(alDisplay, GetSize().x, GetSize().y);
#elif defined(__WXMAC__)
    wxMessageBox("macOS: use off-screen ou patch Allegro");
    return;
#endif

    al_init_primitives_addon();
    al_install_mouse();
    timer = al_create_timer(1.0/60.0);
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
}

void PaintPanel::Cleanup()
{
    if(queue){ al_destroy_event_queue(queue); queue=nullptr; }
    if(timer){ al_destroy_timer(timer); timer=nullptr; }
    if(alDisplay){ al_destroy_display(alDisplay); alDisplay=nullptr; }
}

void PaintPanel::OnSize(wxSizeEvent& evt)
{
    if(alDisplay) al_resize_display(alDisplay, evt.GetSize().x, evt.GetSize().y);
    evt.Skip();
}

void PaintPanel::OnPaint(wxPaintEvent&)
{
    if(!alDisplay) return;

    /* Allegro render loop simples */
    ALLEGRO_EVENT ev;
    while(!al_is_event_queue_empty(queue)){
        al_wait_for_event(queue, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            al_set_target_backbuffer(alDisplay);
            al_clear_to_color(al_map_rgb(30,30,30));

            int w = al_get_display_width(alDisplay);
            int h = al_get_display_height(alDisplay);
            float cx = w*0.5f, cy = h*0.5f;
            al_draw_line(cx,0,cx,h, al_map_rgb(100,100,100),1);
            al_draw_line(0,cy,w,cy, al_map_rgb(100,100,100),1);

            al_flip_display();
        }
    }
}
