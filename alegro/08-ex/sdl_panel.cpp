#include "sdl_panel.h"

#include <SDL2/SDL.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>      // ← ADICIONE ESTA LINHA

wxBEGIN_EVENT_TABLE (SDLPanel, wxPanel)
    EVT_PAINT (SDLPanel::OnPaint)
    EVT_SIZE (SDLPanel::OnSize)
    EVT_TIMER (wxID_ANY, SDLPanel::OnTimer)
wxEND_EVENT_TABLE()

namespace
{
// Controla ref-count do subsistema de vídeo do SDL para inicializar/fechar uma vez.
int g_sdlVideoUsers = 0;

void EnsureSDLVideoInit()
{
    if (g_sdlVideoUsers == 0)
    {
        if (SDL_InitSubSystem (SDL_INIT_VIDEO) != 0)
        {
            wxLogError ("SDL_InitSubSystem(SDL_INIT_VIDEO) falhou: %s", SDL_GetError());
        }
    }
    ++g_sdlVideoUsers;
}

void EnsureSDLVideoQuit()
{
    if (g_sdlVideoUsers > 0)
    {
        --g_sdlVideoUsers;
        if (g_sdlVideoUsers == 0)
        {
            SDL_QuitSubSystem (SDL_INIT_VIDEO);
        }
    }
}
}

SDLPanel::SDLPanel (wxWindow* parent)
    : wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
      timer_ (this)
{
    // Evita flicker porque o SDL vai pintar por cima
    SetBackgroundStyle (wxBG_STYLE_PAINT);

    // Inicialização adiada: precisamos que o painel tenha handle nativo válido.
    // CallAfter garante que rode depois que o loop criar a janela nativa.
    CallAfter ([this]
    {
        InitSDLIfNeeded();
        // inicia animação (60 FPS aproximado)
        timer_.Start (16);
    });
}

SDLPanel::~SDLPanel()
{
    timer_.Stop();
    DestroySDL();
}

void* SDLPanel::NativeHandle() const
{
#ifdef __WXMSW__
    return reinterpret_cast<void*> (GetHandle()); // HWND
#else
    // Em GTK/X11/Wayland, wxWidgets ainda expõe um "window id" via GetHandle().
    // O SDL interpreta isso conforme o backend. Mantemos cast para void*.
    return reinterpret_cast<void*> (GetHandle());
#endif
}

void SDLPanel::InitSDLIfNeeded()
{
    if (initialized_) return;

    EnsureSDLVideoInit();

    // Cria um SDL_Window "a partir" do handle do wxPanel (janela já existente)
    sdlWin_ = SDL_CreateWindowFrom (NativeHandle());
    if (!sdlWin_)
    {
        wxLogError ("SDL_CreateWindowFrom falhou: %s", SDL_GetError());
        EnsureSDLVideoQuit();
        return;
    }

    // Cria renderer acelerado (podemos pedir VSYNC para suavisar)
    renderer_ = SDL_CreateRenderer (sdlWin_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        wxLogWarning ("Renderer acelerado indisponível, tentando software...");
        renderer_ = SDL_CreateRenderer (sdlWin_, -1, 0);
    }
    if (!renderer_)
    {
        wxLogError ("SDL_CreateRenderer falhou: %s", SDL_GetError());
        SDL_DestroyWindow (sdlWin_);
        sdlWin_ = nullptr;
        EnsureSDLVideoQuit();
        return;
    }

    // Ajusta viewport inicial
    const auto sz = GetClientSize();
    SDL_RenderSetLogicalSize (renderer_, sz.GetWidth(), sz.GetHeight());

    initialized_ = true;
}

void SDLPanel::DestroySDL()
{
    if (renderer_)
    {
        SDL_DestroyRenderer (renderer_);
        renderer_ = nullptr;
    }
    if (sdlWin_)
    {
        // Importante: SDL_DestroyWindow NÃO destrói a janela nativa original,
        // apenas libera a "view" SDL criada a partir dela.
        SDL_DestroyWindow (sdlWin_);
        sdlWin_ = nullptr;
    }
    if (initialized_)
    {
        EnsureSDLVideoQuit();
        initialized_ = false;
    }
}

void SDLPanel::OnPaint (wxPaintEvent&)
{
    // Evita apagar o fundo; usamos DoubleBuffer só para o wx não borrar
    wxAutoBufferedPaintDC dc (this);
    if (!initialized_) InitSDLIfNeeded();
    Render();
}

void SDLPanel::OnSize (wxSizeEvent& evt)
{
    evt.Skip();
    if (!initialized_) return;
    const auto sz = GetClientSize();
    // Mantém o renderer em sincronia com o painel
    SDL_RenderSetLogicalSize (renderer_, sz.GetWidth(), sz.GetHeight());
    Render();
}

void SDLPanel::OnTimer (wxTimerEvent&)
{
    if (!initialized_) return;
    ++t_;
    Render();
}

void SDLPanel::Render()
{
    if (!renderer_) return;

    // Animação simples: fundo oscilando e um retângulo andando
    const auto sz = GetClientSize();
    const int W = sz.GetWidth();
    const int H = sz.GetHeight();

    // Cor de fundo pulsante
    const Uint8 r = static_cast<Uint8> ((std::sin (t_ * 0.05) * 0.5 + 0.5) * 50);
    const Uint8 g = static_cast<Uint8> ((std::sin (t_ * 0.07 + 1.0) * 0.5 + 0.5) * 120);
    const Uint8 b = static_cast<Uint8> ((std::sin (t_ * 0.09 + 2.0) * 0.5 + 0.5) * 200);

    SDL_SetRenderDrawColor (renderer_, r, g, b, 255);
    SDL_RenderClear (renderer_);

    // Quadrado animado
    int boxSize = std::min (W, H) / 6;
    int x = (t_ * 3) % std::max (1, (W - boxSize));
    int y = (H - boxSize) / 2;

    SDL_Rect rect { x, y, boxSize, boxSize };
    SDL_SetRenderDrawColor (renderer_, 255 - r, 255 - g, 255 - b, 255);
    SDL_RenderFillRect (renderer_, &rect);

    // Linha diagonal
    SDL_SetRenderDrawColor (renderer_, 240, 240, 240, 255);
    for (int i = 0; i < 4; ++i)
    {
        SDL_RenderDrawLine (renderer_, 0, i, W - 1, H - 1 - i);
    }

    SDL_RenderPresent (renderer_);
}
