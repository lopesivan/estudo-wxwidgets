#include <wx/wx.h>
#include "PaintPanel.hpp"
#include <thread>
#include <atomic>

std::atomic<bool> g_running{true};

wxBEGIN_EVENT_TABLE(PaintPanel, wxPanel)
    EVT_SIZE(PaintPanel::onSize)
    EVT_MOTION(PaintPanel::onMouseMove)
    EVT_LEFT_DOWN(PaintPanel::onLeftDown)
    EVT_LEFT_UP(PaintPanel::onLeftUp)
    EVT_ERASE_BACKGROUND(PaintPanel::onErase)
wxEND_EVENT_TABLE()

class App : public wxApp
{
public:
    bool OnInit() override
    {
        wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "wx + Allegro Paint",
                                     wxDefaultPosition, wxSize(900,700));
        PaintPanel* panel = new PaintPanel(frame);
        frame->Show();
        return true;
    }
}; wxIMPLEMENT_APP(App);

/* ---------- PaintPanel ---------- */
PaintPanel::PaintPanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxFULL_REPAINT_ON_RESIZE)
{
    InitAllegro();
    // inicia thread de render
    std::thread([this]{ PaintLoop(); }).detach();
}

PaintPanel::~PaintPanel()
{
    g_running = false;
    if(queue) al_destroy_event_queue(queue);
    if(timer) al_destroy_timer(timer);
    if(alDisplay) al_destroy_display(alDisplay);
}

void PaintPanel::InitAllego()
{
#ifdef __WXMSW__
    HWND hwnd = (HWND)GetHandle();
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    al_set_new_window_position(0, 0);
    alDisplay = al_create_display(1, 1);  // dummy
    if(!alDisplay) { wxMessageBox("al_create_display falhou"); return; }
    /* anexa ao hwnd real */
    al_set_target_backbuffer(alDisplay);
    al_resize_display(alDisplay, GetSize().x, GetSize().y);
#elif defined(__WXGTK__)
    /* GTK: obtemos XID via gtk_widget_get_window + GDK_WINDOW_XID */
    gtk_widget_realize(GTK_WIDGET(GetHandle()));
    GdkWindow* gdk = gtk_widget_get_window(GTK_WIDGET(GetHandle()));
    Window xid = GDK_WINDOW_XID(gdk);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplay = al_create_display(1,1);
    al_set_target_backbuffer(alDisplay);
    al_resize_display(alDisplay, GetSize().x, GetSize().y);
#elif defined(__WXMAC__)
    /* macOS: NSView*  ->  al_set_new_display_option(ALLEGRO_FRAMELESS, 1, ALLEGRO_SUGGEST); */
    /* Exige patch do Allegro ou render off-screen. */
    wxMessageBox("macOS: implementação off-screen sugerida.");
    return;
#endif

    al_init_primitives_addon();
    al_install_mouse();
    timer   = al_create_timer(1.0/60.0);
    queue   = al_create_event_queue();
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
}

void PaintPanel::onSize(wxSizeEvent& evt)
{
    if(alDisplay)
        al_resize_display(alDisplay, evt.GetSize().x, evt.GetSize().y);
}

void PaintPanel::onLeftDown(wxMouseEvent& evt)
{
    drawing = true;
    lastX = evt.GetX();
    lastY = evt.GetY();
}
void PaintPanel::onLeftUp(wxMouseEvent&)  { drawing = false; }
void PaintPanel::onMouseMove(wxMouseEvent& evt)
{
    if(!drawing || !alDisplay) return;
    lastX = evt.GetX();
    lastY = evt.GetY();
}

void PaintPanel::PaintLoop()
{
    while(g_running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            al_set_target_backbuffer(alDisplay);
            al_clear_to_color(al_map_rgb(30,30,30));

            /* grade simples */
            int w = al_get_display_width(alDisplay);
            int h = al_get_display_height(alDisplay);
            float cx = w*0.5f, cy = h*0.5f;
            al_draw_line(cx,0,cx,h, al_map_rgb(100,100,100),1);
            al_draw_line(0,cy,w,cy, al_map_rgb(100,100,100),1);

            /* "pincel" */
            if(drawing)
                al_draw_filled_circle(lastX, lastY, 8, al_map_rgb(0,255,0));

            al_flip_display();
        }
    }
}
