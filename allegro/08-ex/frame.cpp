#include "frame.h"
#include "sdl_panel.h"
#include <wx/sizer.h>

namespace
{
constexpr int  kW = 800;
constexpr int  kH = 500;
constexpr auto kTitle = "wxWidgets + SDL2 dentro de wxPanel";
}

MyFrame::MyFrame()
    : wxFrame (nullptr, wxID_ANY, kTitle, wxDefaultPosition, wxSize{kW, kH})
{
    CreateStatusBar();
    SetStatusText ("Pronto para renderizar com SDL dentro do wxPanel.");

    auto* root  = new wxPanel (this);
    auto* sizer = new wxBoxSizer (wxVERTICAL);

    auto* sdlPanel = new SDLPanel (root);
    sizer->Add (sdlPanel, 1, wxEXPAND | wxALL, 8);

    root->SetSizer (sizer);

    auto* frameSizer = new wxBoxSizer (wxVERTICAL);
    frameSizer->Add (root, 1, wxEXPAND);
    SetSizer (frameSizer);

    Centre();
}
