#include "sdl_panel.h"

#include <SDL2/SDL.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <cmath>

// ======= IMPORTANTE =======
// No Windows, não precisa de nada extra.
// No Linux (wxGTK sob X11), precisamos pegar o XID do GdkWindow.
// Isolado aqui dentro do .cpp para não "poluir" o resto do projeto.
#ifdef __WXGTK__
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
  #include <gdk/gdkx.h>   // GDK_WINDOW_XID (X11)
#endif

wxBEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT (SDLPanel::OnPaint)
    EVT_SIZE  (SDLPanel::OnSize)
    EVT_TIMER (wxID_ANY, SDLPanel::OnTimer)
    EVT_ERASE_BACKGROUND(SDLPanel::OnEraseBackground)
wxEND_EVENT_TABLE()

namespace {
int g_sdlVideoUsers = 0;

void EnsureSDLVideoInit() {
    if (g_sdlVideoUsers == 0) {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            wxLogError("SDL_InitSubSystem(SDL_INIT_VIDEO) falhou: %s", SDL_GetError());
        }
    }
    ++g_sdlVideoUsers;
}

void EnsureSDLVideoQuit() {
    if (g_sdlVideoUsers > 0) {
        --g_sdlVideoUsers;
        if (g_sdlVideoUsers == 0) {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
    }
}
} // anon

SDLPanel::SDLPanel(wxWindow* parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
  timer_(this, wxID_HIGHEST+1)   // timer id dedicado
{
    // Evita que o wx pinte o fundo por cima do SDL
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Inicialização tardia: só quando a janela nativa já existir
    CallAfter([this]{
        InitSDLIfNeeded();
        timer_.Start(16); // ~60 FPS
    });
}

SDLPanel::~SDLPanel() {
    timer_.Stop();
    DestroySDL();
}

void* SDLPanel::NativeHandle() const {
#ifdef __WXMSW__
    // No MSW, GetHWND() retorna HWND diretamente
    return reinterpret_cast<void*>(GetHWND());
#elif defined(__WXGTK__)
    // No GTK/X11, precisamos do XID (Window) do GdkWindow
    GtkWidget* widget = static_cast<GtkWidget*>(GetHandle());
    if (!widget) return nullptr;
    if (!gtk_widget_get_realized(widget))
        gtk_widget_realize(widget);

    GdkWindow* gdkWin = gtk_widget_get_window(widget);
    if (!gdkWin) return nullptr;

    Window xid = GDK_WINDOW_XID(gdkWin); // X11 Window (inteiro)
    // Convertemos para ponteiro sem sinal intermediário para evitar truncamentos
    return reinterpret_cast<void*>(static_cast<uintptr_t>(xid));
#else
    // macOS/Wayland puros: SDL_CreateWindowFrom pode não ser suportado
    return nullptr;
#endif
}

void SDLPanel::InitSDLIfNeeded() {
    if (initialized_) return;

    EnsureSDLVideoInit();

    void* native = NativeHandle();
    if (!native) {
        wxLogWarning("Handle nativo indisponível (a janela ainda não está pronta?). Tentando novamente no próximo ciclo.");
        return;
    }

    sdlWin_ = SDL_CreateWindowFrom(native);
    if (!sdlWin_) {
        wxLogError("SDL_CreateWindowFrom falhou: %s", SDL_GetError());
        EnsureSDLVideoQuit();
        return;
    }

    // Tenta acelerado + vsync; se falhar, cai para software
    renderer_ = SDL_CreateRenderer(sdlWin_, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        wxLogWarning("Renderer acelerado indisponível, tentando software...");
        renderer_ = SDL_CreateRenderer(sdlWin_, -1, 0);
    }
    if (!renderer_) {
        wxLogError("SDL_CreateRenderer falhou: %s", SDL_GetError());
        SDL_DestroyWindow(sdlWin_);
        sdlWin_ = nullptr;
        EnsureSDLVideoQuit();
        return;
    }

    const auto sz = GetClientSize();
    SDL_RenderSetLogicalSize(renderer_, sz.GetWidth(), sz.GetHeight());

    initialized_ = true;
}

void SDLPanel::DestroySDL() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (sdlWin_) {
        // DestroyWindow aqui só destrói o wrapper do SDL (não a janela gtk)
        SDL_DestroyWindow(sdlWin_);
        sdlWin_ = nullptr;
    }
    if (initialized_) {
        EnsureSDLVideoQuit();
        initialized_ = false;
    }
}

void SDLPanel::OnPaint(wxPaintEvent&) {
    wxAutoBufferedPaintDC dc(this); // evita garbage
    if (!initialized_) InitSDLIfNeeded();
    Render();
}

void SDLPanel::OnSize(wxSizeEvent& evt) {
    evt.Skip();
    if (!initialized_) return;
    const auto sz = GetClientSize();
    SDL_RenderSetLogicalSize(renderer_, sz.GetWidth(), sz.GetHeight());
    Render();
}

void SDLPanel::OnTimer(wxTimerEvent&) {
    if (!initialized_) return;
    ++t_;
    Render();
}

void SDLPanel::Render() {
    if (!renderer_) return;

    const auto sz = GetClientSize();
    const int W = sz.GetWidth();
    const int H = sz.GetHeight();

    // Fundo pulsante
    const Uint8 r = static_cast<Uint8>((std::sin(t_ * 0.05) * 0.5 + 0.5) * 50);
    const Uint8 g = static_cast<Uint8>((std::sin(t_ * 0.07 + 1.0) * 0.5 + 0.5) * 120);
    const Uint8 b = static_cast<Uint8>((std::sin(t_ * 0.09 + 2.0) * 0.5 + 0.5) * 200);

    SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
    SDL_RenderClear(renderer_);

    // Quadrado animado
    int box = std::max(8, std::min(W, H) / 6);
    int x   = (t_ * 3) % std::max(1, (W - box));
    int y   = (H - box) / 2;

    SDL_Rect rect{ x, y, box, box };
    SDL_SetRenderDrawColor(renderer_, 255 - r, 255 - g, 255 - b, 255);
    SDL_RenderFillRect(renderer_, &rect);

    // Linhas diagonais
    SDL_SetRenderDrawColor(renderer_, 240, 240, 240, 255);
    for (int i = 0; i < 4; ++i) {
        SDL_RenderDrawLine(renderer_, 0, i, W - 1, H - 1 - i);
    }

    SDL_RenderPresent(renderer_);
}

