#include <wx/wx.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <thread>
#include <atomic>

std::atomic<bool> g_running{true};

// ---------- Painel onde o Allegro desenha ----------
class PaintPanel : public wxPanel
{
  public:
    PaintPanel(wxWindow* parent);
    ~PaintPanel();

  private:
    ALLEGRO_DISPLAY*     alDisplay = nullptr;
    ALLEGRO_TIMER*       timer     = nullptr;
    ALLEGRO_EVENT_QUEUE* queue     = nullptr;
    bool                 drawing   = false;
    float                lastX     = 0;
    float                lastY     = 0;

    void InitAllegro();
    void PaintLoop();  // thread de render

    // eventos wx
    void onSize(wxSizeEvent& evt);
    void onMouseMove(wxMouseEvent& evt);
    void onLeftDown(wxMouseEvent& evt);
    void onLeftUp(wxMouseEvent& evt);
    void onErase(wxEraseEvent& evt)
    {
    }  // evita flicker

    wxDECLARE_EVENT_TABLE();
};

// ---------- Tabela de eventos wx ----------
wxBEGIN_EVENT_TABLE(PaintPanel, wxPanel)
    EVT_SIZE(PaintPanel::onSize) EVT_MOTION(PaintPanel::onMouseMove)
        EVT_LEFT_DOWN(PaintPanel::onLeftDown)
            EVT_LEFT_UP(PaintPanel::onLeftUp)
                EVT_ERASE_BACKGROUND(PaintPanel::onErase)
                    wxEND_EVENT_TABLE()

    // ---------- Implementação ----------
    PaintPanel::PaintPanel(wxWindow* parent)
    : wxPanel(parent,
              wxID_ANY,
              wxDefaultPosition,
              wxDefaultSize,
              wxFULL_REPAINT_ON_RESIZE)
{
    InitAllegro();
    std::thread([this] { PaintLoop(); }).detach();
}

PaintPanel::~PaintPanel()
{
    g_running = false;
    if(queue)
        al_destroy_event_queue(queue);
    if(timer)
        al_destroy_timer(timer);
    if(alDisplay)
        al_destroy_display(alDisplay);
}

void PaintPanel::InitAllegro()
{
#ifdef __WXMSW__
    HWND hwnd = (HWND)GetHandle();
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    alDisplay = al_create_display(1, 1);
    if(!alDisplay)
    {
        wxMessageBox("al_create_display falhou");
        return;
    }
    al_set_target_backbuffer(alDisplay);
    al_resize_display(alDisplay, GetSize().x, GetSize().y);

#elif defined(__WXGTK__)
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
    GtkWidget* gtkWidget = (GtkWidget*)GetHandle();
    gtk_widget_realize(gtkWidget);
    GdkWindow* gdkWindow = gtk_widget_get_window(gtkWidget);
    Window     xid       = GDK_WINDOW_XID(gdkWindow);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    alDisplay = al_create_display(1, 1);
    if(!alDisplay)
    {
        wxMessageBox("al_create_display falhou");
        return;
    }
    al_set_target_backbuffer(alDisplay);
    al_resize_display(alDisplay, GetSize().x, GetSize().y);

#elif defined(__WXMAC__)
    wxMessageBox("macOS: render off-screen ainda não implementado");
    return;
#endif

    al_init_primitives_addon();
    al_install_mouse();
    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();
    al_register_event_source(queue,
                             al_get_timer_event_source(timer));
    al_start_timer(timer);
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
            al_clear_to_color(al_map_rgb(30, 30, 30));

            int   w  = al_get_display_width(alDisplay);
            int   h  = al_get_display_height(alDisplay);
            float cx = w * 0.5f, cy = h * 0.5f;
            al_draw_line(
                cx, 0, cx, h, al_map_rgb(100, 100, 100), 1);
            al_draw_line(
                0, cy, w, cy, al_map_rgb(100, 100, 100), 1);

            if(drawing)
                al_draw_filled_circle(
                    lastX, lastY, 8, al_map_rgb(0, 255, 0));

            al_flip_display();
        }
    }
}

void PaintPanel::onSize(wxSizeEvent& evt)
{
    if(alDisplay)
        al_resize_display(
            alDisplay, evt.GetSize().x, evt.GetSize().y);
    evt.Skip();
}

void PaintPanel::onLeftDown(wxMouseEvent& evt)
{
    drawing = true;
    lastX   = evt.GetX();
    lastY   = evt.GetY();
}
void PaintPanel::onLeftUp(wxMouseEvent&)
{
    drawing = false;
}
void PaintPanel::onMouseMove(wxMouseEvent& evt)
{
    if(!drawing || !alDisplay)
        return;
    lastX = evt.GetX();
    lastY = evt.GetY();
}

// ---------- Aplicação wx ----------
class App : public wxApp
{
  public:
    bool OnInit() override
    {
        wxFrame* frame = new wxFrame(nullptr,
                                     wxID_ANY,
                                     "wx + Allegro Paint",
                                     wxDefaultPosition,
                                     wxSize(900, 700));
        new PaintPanel(frame);
        frame->Show();
        return true;
    }
};
wxIMPLEMENT_APP(App);
