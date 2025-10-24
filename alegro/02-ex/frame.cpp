#include "frame.h"

enum {
    ID_Hello = wxID_HIGHEST + 1
};

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY,
              "Exemplo wxWidgets - Separação em headers",
              wxDefaultPosition, wxSize(600, 400))
{
    // --- Menu ---
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Diga olá");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    // --- Status bar ---
    CreateStatusBar();
    SetStatusText("Bem-vindo ao wxWidgets!");

    // --- Eventos ---
    Bind(wxEVT_MENU, &MyFrame::OnQuit,  this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, [this](wxCommandEvent&) {
        wxLogMessage("Olá de wxWidgets!");
    }, ID_Hello);
}

void MyFrame::OnQuit(wxCommandEvent&) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent&) {
    wxMessageBox("Exemplo com wxWidgets\nSeparação em app.h/app.cpp e frame.h/frame.cpp",
                 "Sobre", wxOK | wxICON_INFORMATION, this);
}

