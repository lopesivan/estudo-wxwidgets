#include "frame.h"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/stattext.h>

namespace
{
// Constantes modernas
constexpr int   kWidth  = 640;
constexpr int   kHeight = 420;
constexpr auto  kTitle  = "Exemplo wxWidgets — C++ moderno";

// IDs seguros com enum class
enum class Id : int
{
    Hello = wxID_HIGHEST + 1
};

// Helper para cast quando necessário
constexpr int as_int (Id v)
{
    return static_cast<int> (v);
}
}

MyFrame::MyFrame()
    : wxFrame (nullptr, wxID_ANY, kTitle, wxDefaultPosition, wxSize{kWidth, kHeight})
{
    // --- Menu moderno com auto e enum class ---
    auto* menuFile = new wxMenu;
    menuFile->Append (as_int (Id::Hello), "&Hello...\tCtrl-H", "Diga olá");
    menuFile->AppendSeparator();
    menuFile->Append (wxID_EXIT, "E&xit\tAlt-F4", "Sair");

    auto* menuHelp = new wxMenu;
    menuHelp->Append (wxID_ABOUT, "&About\tF1", "Sobre este app");

    auto* menuBar = new wxMenuBar;
    menuBar->Append (menuFile, "&File");
    menuBar->Append (menuHelp, "&Help");
    SetMenuBar (menuBar);

    // --- Barra de status ---
    CreateStatusBar();
    SetStatusText ("Bem-vindo ao wxWidgets (C++ moderno)!");

    // --- Conteúdo central com sizers ---
    auto* root = new wxPanel (this);
    auto* vbox = new wxBoxSizer (wxVERTICAL);

    auto* title = new wxStaticText (root, wxID_ANY, "UI com padrões modernos de C++");
    title->SetFont (title->GetFont().MakeBold().Scale (1.2));

    auto* btnHello = new wxButton (root, as_int (Id::Hello), "Hello (lambda)");
    auto* btnQuit  = new wxButton (root, wxID_EXIT, "Sair");

    vbox->Add (title,    0, wxALL | wxALIGN_CENTER_HORIZONTAL, 12);
    vbox->Add (btnHello, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 8);
    vbox->Add (btnQuit,  0, wxALL | wxALIGN_CENTER_HORIZONTAL, 8);

    root->SetSizerAndFit (vbox);
    this->SetSizerAndFit (new wxBoxSizer (wxVERTICAL));
    this->GetSizer()->Add (root, 1, wxEXPAND);

    // --- Eventos com lambdas e override claros ---
    Bind (wxEVT_MENU, &MyFrame::OnQuit,  this, wxID_EXIT);
    Bind (wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    // Lambda para o menu "Hello"
    Bind (wxEVT_MENU, [this] (wxCommandEvent&)
    {
        wxLogMessage ("Olá de wxWidgets! ✅");
        SetStatusText ("Lambda executada com sucesso.");
    }, as_int (Id::Hello));

    // Botões reutilizando as mesmas rotas de evento
    btnHello->Bind (wxEVT_BUTTON, [this] (wxCommandEvent&)
    {
        wxPostEvent (this, wxCommandEvent (wxEVT_MENU, as_int (Id::Hello)));
    });
    btnQuit->Bind (wxEVT_BUTTON, [this] (wxCommandEvent&)
    {
        wxPostEvent (this, wxCommandEvent (wxEVT_MENU, wxID_EXIT));
    });
}

void MyFrame::OnQuit (wxCommandEvent& /*evt*/)
{
    Close (true);
}

void MyFrame::OnAbout (wxCommandEvent& /*evt*/)
{
    wxMessageBox (
        "Exemplo com organização moderna em C++:\n"
        " - override/final\n"
        " - enum class para IDs\n"
        " - constexpr para constantes\n"
        " - deletes para cópia/movimento\n"
        " - lambdas para eventos",
        "Sobre", wxOK | wxICON_INFORMATION, this
    );
}
