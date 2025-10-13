#include "main_frame.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

// Construtor: carrega este frame a partir do XRC e prepara a UI
MainFrame::MainFrame()
    : wxFrame()  // <-- construtor vazio, NÃO cria GtkWindow ainda
{
    if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "ID_WXFRAME"))
    {
        wxLogError("Falha ao carregar ID_WXFRAME do XRC.");
        return;
    }

    // agora tudo já existe via XRC; ajuste o que quiser:
    SetSize(wxSize(900, 600));
    Centre();

    CacheControls();
    SetupLabels();
    SetupListColumns();
    BindEvents();

    Layout();
}

// ---------------------- Setup ----------------------

void MainFrame::CacheControls()
{
    packageList_ = XRCCTRL(*this, "ID_PACKAGE_LIST", wxListCtrl);
    out_ = XRCCTRL(*this, "ID_OUTPUT_TEXT", wxTextCtrl);
    btnRefresh_ = XRCCTRL(*this, "BTN_REFRESH", wxButton);
    btnUpdate_ = XRCCTRL(*this, "BTN_UPDATE", wxButton);
    btnUpgrade_ = XRCCTRL(*this, "BTN_UPGRADE", wxButton);
    btnListUpgradable_ = XRCCTRL(*this, "BTN_LIST_UPGRADABLE", wxButton);

    // (Opcional) asserts de desenvolvimento:
    // wxASSERT_MSG(packageList_, "ID_PACKAGE_LIST não encontrado");
    // wxASSERT_MSG(out_,         "ID_OUTPUT_TEXT não encontrado");
}

void MainFrame::SetupLabels()
{
    if (btnRefresh_)
        btnRefresh_->SetLabel("Atualizar Lista");
    if (btnUpdate_)
        btnUpdate_->SetLabel("APT Update");
    if (btnUpgrade_)
        btnUpgrade_->SetLabel("APT Upgrade");
    if (btnListUpgradable_)
        btnListUpgradable_->SetLabel(
            wxString::FromUTF8(u8"Pacotes Atualizáveis"));
}

void MainFrame::SetupListColumns()
{
    if (!packageList_)
        return;
    if (packageList_->GetColumnCount() > 0)
        return;  // evita duplicar

    packageList_->AppendColumn("Pacote", wxLIST_FORMAT_LEFT, 300);
    packageList_->AppendColumn(
        wxString::FromUTF8(u8"Versão"), wxLIST_FORMAT_LEFT, 200);
    packageList_->AppendColumn("Arquitetura", wxLIST_FORMAT_LEFT, 150);
}

void MainFrame::BindEvents()
{
    if (btnRefresh_)
        btnRefresh_->Bind(wxEVT_BUTTON, &MainFrame::OnRefresh, this);
    if (btnUpdate_)
        btnUpdate_->Bind(wxEVT_BUTTON, &MainFrame::OnUpdate, this);
    if (btnUpgrade_)
        btnUpgrade_->Bind(wxEVT_BUTTON, &MainFrame::OnUpgrade, this);
    if (btnListUpgradable_)
        btnListUpgradable_->Bind(
            wxEVT_BUTTON, &MainFrame::OnListUpgradable, this);
}

// ---------------------- Events ----------------------

void MainFrame::OnRefresh(wxCommandEvent&)
{
    AppendOut("Refresh: atualizar lista...\n");
    // aqui você pode chamar sua futura LoadInstalledPackages()
}

void MainFrame::OnUpdate(wxCommandEvent&)
{
    AppendOut("APT Update...\n");
    // aqui você pode chamar ExecuteCommandWithSudo("apt update");
}

void MainFrame::OnUpgrade(wxCommandEvent&)
{
    AppendOut("APT Upgrade...\n");
    // confirmação + ExecuteCommandWithSudo("apt upgrade -y");
}

void MainFrame::OnListUpgradable(wxCommandEvent&)
{
    AppendOut("Listar pacotes atualizáveis...\n");
    // ExecuteCommand("apt list --upgradable 2>/dev/null");
}

// ---------------------- Helpers ----------------------

void MainFrame::AppendOut(const wxString& s)
{
    if (out_)
        out_->AppendText(s);
}
