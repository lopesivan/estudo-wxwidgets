#include <wx/wx.h>

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnQuit (wxCommandEvent&);
    void OnAbout (wxCommandEvent&);
};

enum
{
    ID_Hello = wxID_HIGHEST + 1
};

MyFrame::MyFrame()
    : wxFrame (nullptr, wxID_ANY,
               "Exemplo wxWidgets - Arquivos separados",
               wxDefaultPosition, wxSize (600, 400))
{
    // --- Menu ---
    auto* menuFile = new wxMenu;
    menuFile->Append (ID_Hello, "&Hello...\tCtrl-H", "Diga olá");
    menuFile->AppendSeparator();
    menuFile->Append (wxID_EXIT);

    auto* menuHelp = new wxMenu;
    menuHelp->Append (wxID_ABOUT);

    auto* menuBar = new wxMenuBar;
    menuBar->Append (menuFile, "&File");
    menuBar->Append (menuHelp, "&Help");
    SetMenuBar (menuBar);

    // --- Status bar ---
    CreateStatusBar();
    SetStatusText ("Bem-vindo ao wxWidgets!");

    // --- Eventos ---
    Bind (wxEVT_MENU, &MyFrame::OnQuit,  this, wxID_EXIT);
    Bind (wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind (wxEVT_MENU, [this] (wxCommandEvent&)
    {
        wxLogMessage ("Olá de wxWidgets!");
    }, ID_Hello);
}

void MyFrame::OnQuit (wxCommandEvent&)
{
    Close (true);
}

void MyFrame::OnAbout (wxCommandEvent&)
{
    wxMessageBox ("Exemplo básico com wxWidgets\nArquivos: app.cpp e frame.cpp",
                  "Sobre", wxOK | wxICON_INFORMATION, this);
}

// Factory function visível para app.cpp
wxFrame* CreateMainFrame()
{
    return new MyFrame();
}
