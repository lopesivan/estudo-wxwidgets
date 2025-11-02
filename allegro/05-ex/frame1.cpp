#include "frame1.h"
#include "app.h"
#include <wx/xrc/xmlres.h>

namespace
{
constexpr const char* kFrameName  = "Frame1";
constexpr const char* kGoToF2Name = "ID_GOTO_F2";
}

MyFrame1::MyFrame1()
    : wxFrame()
{
    if (!wxXmlResource::Get()->LoadFrame (this, nullptr, kFrameName))
    {
        wxLogError ("Não consegui carregar o recurso XRC 'Frame1'.");
    }

    // Mapear botões/menus
    const int idGoToF2 = wxXmlResource::GetXRCID (kGoToF2Name);

    Bind (wxEVT_BUTTON, &MyFrame1::OnGoToF2, this, idGoToF2);
    Bind (wxEVT_BUTTON, &MyFrame1::OnExit,   this, wxID_EXIT);
    Bind (wxEVT_MENU,   &MyFrame1::OnExit,   this, wxID_EXIT);

    CreateStatusBar();
    SetStatusText ("Frame 1 carregado via XRC");
}

void MyFrame1::OnGoToF2 (wxCommandEvent&)
{
    wxGetApp().ShowFrame2(); // alterna para o Frame 2
}

void MyFrame1::OnExit (wxCommandEvent&)
{
    Close (true);
}
