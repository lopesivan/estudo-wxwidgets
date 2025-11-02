#include "frame2.h"
#include "app.h"
#include <wx/xrc/xmlres.h>

namespace
{
constexpr const char* kFrameName   = "Frame2";
constexpr const char* kBackToF1    = "ID_BACK_F1";
}

MyFrame2::MyFrame2()
    : wxFrame()
{
    if (!wxXmlResource::Get()->LoadFrame (this, nullptr, kFrameName))
    {
        wxLogError ("Não consegui carregar o recurso XRC 'Frame2'.");
    }

    const int idBack = wxXmlResource::GetXRCID (kBackToF1);

    Bind (wxEVT_BUTTON, &MyFrame2::OnBackToF1, this, idBack);
    Bind (wxEVT_BUTTON, &MyFrame2::OnExit,     this, wxID_EXIT);
    Bind (wxEVT_MENU,   &MyFrame2::OnExit,     this, wxID_EXIT);

    CreateStatusBar();
    SetStatusText ("Frame 2 carregado via XRC");
}

void MyFrame2::OnBackToF1 (wxCommandEvent&)
{
    wxGetApp().ShowFrame1(); // volta ao Frame 1
}

void MyFrame2::OnExit (wxCommandEvent&)
{
    Close (true);
}
