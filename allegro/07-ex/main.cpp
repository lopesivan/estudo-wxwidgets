#include <wx/wx.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#ifdef __WXGTK__
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>   // GDK_WINDOW_XID (X11)
#endif

class MyPanel : public wxPanel
{
private:
    SDL_Window*   sdlWindow   = nullptr;
    SDL_Renderer* renderer    = nullptr;
    bool          sdlInitDone = false;
    bool          triedInit   = false;

public:
    MyPanel (wxWindow* parent)
        : wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (640, 480))
    {
        // Evita que o wx apague o fundo e cause flicker
        SetBackgroundStyle (wxBG_STYLE_PAINT);

        Bind (wxEVT_SHOW, &MyPanel::OnShow, this);
        Bind (wxEVT_PAINT, &MyPanel::OnPaint, this);
        Bind (wxEVT_SIZE, &MyPanel::OnSize, this);
        Bind (wxEVT_ERASE_BACKGROUND, &MyPanel::OnEraseBackground, this);
    }

    ~MyPanel() override
    {
        CleanupSDL();
        if (sdlInitDone)
            SDL_Quit();
    }

private:
    void OnEraseBackground (wxEraseEvent&) { /* evita apagar fundo */ }

    void OnShow (wxShowEvent& e)
    {
        e.Skip();
        if (e.IsShown())
            EnsureSDLReady();
    }

    void OnSize (wxSizeEvent& e)
    {
        e.Skip();
        EnsureSDLReady();

        if (renderer)
        {
            // Atualiza viewport para novo tamanho
            wxSize s = GetClientSize();
            SDL_Rect vp{0, 0, s.GetWidth(), s.GetHeight()};
            SDL_RenderSetViewport (renderer, &vp);
            Refresh();
        }
    }

    void OnPaint (wxPaintEvent& e)
    {
        wxPaintDC dc (this); // necessário no wx

        EnsureSDLReady();
        if (!renderer) return;

        // Desenho SDL
        SDL_SetRenderDrawColor (renderer, 0, 128, 255, 255); // azul
        SDL_RenderClear (renderer);

        // Exemplo de primitiva
        // SDL_RenderDrawLine(renderer, 10, 10, 200, 120);

        SDL_RenderPresent (renderer);
    }

    void EnsureSDLReady()
    {
        if (renderer) return;

        // Inicializa SDL apenas uma vez
        if (!sdlInitDone && !triedInit)
        {
            triedInit = true;
            if (SDL_Init (SDL_INIT_VIDEO) < 0)
            {
                wxLogError ("Falha ao inicializar SDL: %s", SDL_GetError());
                return;
            }
            sdlInitDone = true;
        }

        // Cria SDL_Window a partir do handle nativo do wxPanel
        if (!sdlWindow && sdlInitDone)
            InitSDLFromWxPanel();
    }

    void InitSDLFromWxPanel()
    {
        void* nativeHandle = GetNativeHandleForSDL();
        if (!nativeHandle)
        {
            // A janela ainda não está pronta/realizada
            return;
        }

        sdlWindow = SDL_CreateWindowFrom (nativeHandle);
        if (!sdlWindow)
        {
            wxLogError ("Falha ao criar SDL_Window: %s", SDL_GetError());
            return;
        }

        // Cria renderer acelerado
        renderer = SDL_CreateRenderer (sdlWindow,
                                       -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
        {
            wxLogError ("Falha ao criar SDL_Renderer: %s", SDL_GetError());
            SDL_DestroyWindow (sdlWindow);
            sdlWindow = nullptr;
            return;
        }

        // Ajusta viewport inicial
        wxSize s = GetClientSize();
        SDL_Rect vp{0, 0, s.GetWidth(), s.GetHeight()};
        SDL_RenderSetViewport (renderer, &vp);
    }

    void CleanupSDL()
    {
        if (renderer)
        {
            SDL_DestroyRenderer (renderer);
            renderer = nullptr;
        }
        if (sdlWindow)
        {
            // Importante: quando a janela veio de CreateWindowFrom,
            // o DestroyWindow é permitido e seguro.
            SDL_DestroyWindow (sdlWindow);
            sdlWindow = nullptr;
        }
    }

    void* GetNativeHandleForSDL()
    {
#ifdef __WXMSW__
        // No MSW, GetHandle() retorna HWND diretamente
        return reinterpret_cast<void*> (GetHWND());
#elif defined(__WXGTK__)
        // No GTK/X11: precisamos do XID (Window) do GdkWindow
        GtkWidget* widget = static_cast<GtkWidget*> (GetHandle());
        if (!widget)
            return nullptr;

        // Garante que o widget tenha GdkWindow
        if (!gtk_widget_get_realized (widget))
            gtk_widget_realize (widget);

        GdkWindow* gdkWin = gtk_widget_get_window (widget);
        if (!gdkWin)
            return nullptr;

        // Converte para X11 Window:
        Window xid = GDK_WINDOW_XID (gdkWin);
        return reinterpret_cast<void*> (xid);
#elif defined(__WXOSX__)
        // Embutir SDL em NSView/NSWindow não é trivial via SDL2 puro.
        // Retornamos nullptr para não tentar no macOS com este método.
        return nullptr;
#else
        return nullptr;
#endif
    }
};

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame (nullptr,
                   wxID_ANY,
                   "wxWidgets + SDL2",
                   wxDefaultPosition,
                   wxSize (660, 500))
    {
        new MyPanel (this);
        Centre();
    }
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MyFrame();
        frame->Show (true);
        return true;
    }
};

wxIMPLEMENT_APP (MyApp);
