#pragma once
#include <wx/wx.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class PaintPanel : public wxPanel
{
public:
    PaintPanel(wxWindow* parent);
    ~PaintPanel();

private:
    ALLEGRO_DISPLAY* alDisplay = nullptr;
    ALLEGRO_TIMER*   timer     = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;
    bool  drawing = false;
    float lastX = 0, lastY = 0;

    void onSize(wxSizeEvent& evt);
    void onMouseMove(wxMouseEvent& evt);
    void onLeftDown(wxMouseEvent& evt);
    void onLeftUp(wxMouseEvent& evt);
    void onErase(wxEraseEvent& evt) {}  // evita flicker
    void InitAllegro();
    void PaintLoop();   // thread Allegro

    wxDECLARE_EVENT_TABLE();
};
