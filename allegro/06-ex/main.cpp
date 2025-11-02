#include <wx/wx.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

class MyPanel : public wxPanel
{
private:
    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
    bool sdlInitialized;

public:
    MyPanel (wxWindow* parent)
        : wxPanel (parent, wxID_ANY, wxDefaultPosition, wxSize (640, 480))
        , sdlWindow (nullptr)
        , renderer (nullptr)
        , sdlInitialized (false)
    {
        // Inicializa SDL
        if (SDL_Init (SDL_INIT_VIDEO) < 0)
        {
            wxLogError ("Falha ao inicializar SDL: %s", SDL_GetError());
            return;
        }

        sdlInitialized = true;

        Bind (wxEVT_PAINT, &MyPanel::OnPaint, this);
        Bind (wxEVT_SIZE, &MyPanel::OnSize, this);
    }

    ~MyPanel()
    {
        CleanupSDL();
        if (sdlInitialized)
        {
            SDL_Quit();
        }
    }

private:
    void InitSDL()
    {
        if (sdlWindow) return; // Já inicializado

        // Cria janela SDL a partir do handle nativo do wxWidgets
        sdlWindow = SDL_CreateWindowFrom ((void*)GetHandle());
        if (!sdlWindow)
        {
            wxLogError ("Falha ao criar SDL_Window: %s", SDL_GetError());
            return;
        }

        // Cria renderer
        renderer = SDL_CreateRenderer (sdlWindow, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
        {
            wxLogError ("Falha ao criar SDL_Renderer: %s", SDL_GetError());
            SDL_DestroyWindow (sdlWindow);
            sdlWindow = nullptr;
            return;
        }
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
            SDL_DestroyWindow (sdlWindow);
            sdlWindow = nullptr;
        }
    }

    void OnPaint (wxPaintEvent& event)
    {
        // wxPaintDC é necessário para evitar loops infinitos de paint
        wxPaintDC dc (this);

        // Inicializa SDL na primeira pintura (quando a janela já existe)
        if (!sdlWindow)
        {
            InitSDL();
        }

        if (!renderer) return;

        // Renderiza com SDL
        SDL_SetRenderDrawColor (renderer, 0, 128, 255, 255); // azul
        SDL_RenderClear (renderer);

        // Aqui você pode desenhar mais coisas...

        SDL_RenderPresent (renderer);
    }

    void OnSize (wxSizeEvent& event)
    {
        event.Skip();
        Refresh(); // Força redesenho ao redimensionar
    }
};

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame (nullptr, wxID_ANY, "wxWidgets + SDL2",
                   wxDefaultPosition, wxSize (660, 500))
    {
        new MyPanel (this);
        Centre();
    }
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        MyFrame* frame = new MyFrame();
        frame->Show (true);
        return true;
    }
};

wxIMPLEMENT_APP (MyApp);
