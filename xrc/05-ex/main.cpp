#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/xrc/xh_all.h>  // handlers XRC
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        // 1) registra todos os handlers de XRC (botões, sizers, listctrl, etc.)
        wxXmlResource::Get()->InitAllHandlers();
        // redundante mas útil em alguns builds: força o link dos handlers

        // 2) carrega o arquivo .xrc
        if (!wxXmlResource::Get()->Load("ui.xrc"))
        {
            wxMessageBox("Falha ao carregar ui.xrc (confira o caminho e o "
                         "working directory).",
                         "Erro",
                         wxOK | wxICON_ERROR);
            return false;
        }

        // 3) cria o frame definido no XRC (name="ID_WXFRAME")
        wxFrame* frame = wxXmlResource::Get()->LoadFrame(nullptr, "ID_WXFRAME");
        if (!frame)
        {
            wxMessageBox(
                "Falha ao instanciar ID_WXFRAME do XRC (handlers faltando?).",
                "Erro",
                wxOK | wxICON_ERROR);
            return false;
        }

        // wxButton* btnRefresh;         // Botão "Atualizar Lista"
        // wxButton* btnUpdate;          // Botão "APT Update"
        // wxButton* btnUpgrade;         // Botão "APT Upgrade"
        // wxButton* btnListUpgradable;  // Botão "Pacotes Atualizáveis"

        /*
        auto* btnRefresh = XRCCTRL(*frame, "BTN_REFRESH", wxButton);
        if (btnRefresh)
        {
            btnRefresh->SetLabel("Atualizar Lista");
            btnRefresh->Bind(wxEVT_BUTTON, [](wxCommandEvent&) {});
        } */

        auto* btnRefresh = XRCCTRL(*frame, "BTN_REFRESH", wxButton);
        if (btnRefresh)
        {
            btnRefresh->SetLabel("Atualizar Lista");
            btnRefresh->Bind(wxEVT_BUTTON,
                             [frame](wxCommandEvent&)
                             {
                                 wxMessageBox("Atualizando a lista...",
                                              "Info",
                                              wxOK | wxICON_INFORMATION,
                                              frame);
                             });
        }

        auto* btnUpdate = XRCCTRL(*frame, "BTN_UPDATE", wxButton);
        if (btnUpdate)
        {
            btnUpdate->SetLabel("APT Update");
            btnUpdate->Bind(wxEVT_BUTTON,
                            [frame](wxCommandEvent&)
                            { wxPrintf("Atualizando a lista...\n"); });
        }

        auto* btnUpgrade = XRCCTRL(*frame, "BTN_UPGRADE", wxButton);
        if (btnUpgrade)
        {
            btnUpgrade->SetLabel("APT Upgrade");
        }

        auto* btnListUpgradable =
            XRCCTRL(*frame, "BTN_LIST_UPGRADABLE", wxButton);
        if (btnListUpgradable)
        {
            btnListUpgradable->SetLabel(
                // garante UTF-8
                wxString::FromUTF8(u8"Pacotes Atualizáveis"));
        }

        // -------------------------------------------------------------------

        auto* staticText1 = XRCCTRL(*frame, "ID_STATIC_TEXT_1", wxStaticText);
        if (staticText1)
        {
            staticText1->SetLabel(
                // garante UTF-8
                wxString::FromUTF8(
                    u8"Pacotes Atualizáveis k k k k k k k k k "));
        }

        auto* staticText2 = XRCCTRL(*frame, "ID_STATIC_TEXT_2", wxStaticText);
        if (staticText2)
        {
            staticText2->SetLabel(
                // garante UTF-8
                wxString::FromUTF8(
                    u8"Pacotes Atualizáveis t t t t t t t t t "));
        }

        auto* packageList = XRCCTRL(*frame, "ID_PACKAGE_LIST", wxListCtrl);
        if (packageList)
        {
            // Criar 3 colunas na tabela
            packageList->AppendColumn("Pacote", wxLIST_FORMAT_LEFT, 300);
            packageList->AppendColumn(
                wxString::FromUTF8(u8"Versão"), wxLIST_FORMAT_LEFT, 300);
            packageList->AppendColumn("Arquitetura", wxLIST_FORMAT_LEFT, 300);
        }

        frame->Layout();  // recalcula sizers após mudar labels
        frame->Show(true);

        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
