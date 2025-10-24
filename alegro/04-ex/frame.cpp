#include "frame.h"
#include <wx/xrc/xmlres.h>
#include <wx/accel.h>

namespace {
    // IDs estáveis: use os nomes definidos no XRC
    // Em XRC, itens com "name" viram IDs consultáveis por nome:
    // wxXmlResource::GetXRCID("ID_HELLO")
    constexpr const char* kIdHello      = "ID_HELLO";
    constexpr const char* kIdHelloBtn   = "ID_HELLO_BTN";
    constexpr const char* kIdExit       = "wxID_EXIT";
    constexpr const char* kIdAbout      = "wxID_ABOUT";
    constexpr const char* kFrameName    = "MainFrame";
    constexpr const char* kRootPanel    = "rootPanel";
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "") // título/tamanho vêm do XRC
{
    // Carrega o wxFrame diretamente do XRC e "substitui" esta instância
    wxXmlResource::Get()->LoadFrame(this, nullptr, kFrameName);

    CreateStatusBar();
    SetStatusText("Bem-vindo — UI carregada de XRC!");

    // Ligações de eventos usando IDs do XRC
    Bind(wxEVT_MENU, &MyFrame::OnQuit,  this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    const int idHello = wxXmlResource::GetXRCID(kIdHello);
    Bind(wxEVT_MENU, &MyFrame::OnHello, this, idHello);

    // Botão "Hello" dispara o mesmo handler do menu
    if (auto* btnHello = wxDynamicCast(FindWindowById(wxXmlResource::GetXRCID(kIdHelloBtn), this), wxButton)) {
        btnHello->Bind(wxEVT_BUTTON, [this, idHello](wxCommandEvent&) {
            wxPostEvent(this, wxCommandEvent(wxEVT_MENU, idHello));
        });
    }

    // Botão "Sair" (wxID_EXIT) já mapeia para OnQuit via Bind acima

    // Aceleradores adicionais (os do menu já funcionam por "\tCtrl-H", "\tF1"...)
    BuildAccelerators();

    // (Opcional) acessar widgets criados no XRC:
    if (auto* panel = FindWindowByName(kRootPanel, this)) {
        panel->SetFocus();
    }
}

void MyFrame::BuildAccelerators() {
    // Ex.: adicionar ESC para sair além do Alt+F4 já colocado no menu
    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, wxID_EXIT);
    SetAcceleratorTable(wxAcceleratorTable(1, entries));
}

void MyFrame::OnQuit(wxCommandEvent&) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent&) {
    wxMessageBox("Janela construída via XRC (XML Resource).", "Sobre",
                 wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnHello(wxCommandEvent&) {
    wxLogMessage("Olá de XRC + C++ moderno!");
    SetStatusText("Handler disparado por menu/botão/atalho.");
}

