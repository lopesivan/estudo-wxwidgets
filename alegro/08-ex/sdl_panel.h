#pragma once
#include <wx/panel.h>
#include <wx/timer.h>

// Encapsula a integração SDL dentro de um wxPanel
struct SDL_Window;
struct SDL_Renderer;

class SDLPanel final : public wxPanel {
public:
    explicit SDLPanel(wxWindow* parent);
    ~SDLPanel() override;

    SDLPanel(const SDLPanel&)            = delete;
    SDLPanel& operator=(const SDLPanel&) = delete;
    SDLPanel(SDLPanel&&)                 = delete;
    SDLPanel& operator=(SDLPanel&&)      = delete;

private:
    void InitSDLIfNeeded();
    void DestroySDL();

    void OnPaint(wxPaintEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnTimer(wxTimerEvent& evt);

    void Render(); // desenha uma cena simples

    SDL_Window*   sdlWin_     = nullptr;
    SDL_Renderer* renderer_   = nullptr;
    bool          initialized_ = false;

    wxTimer timer_;    // animação
    int     t_ = 0;    // contador simples para animar cor/posição

    // Auxiliar para pegar o handle nativo como ponteiro (void*)
    void* NativeHandle() const;

    wxDECLARE_EVENT_TABLE();
};

