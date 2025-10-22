#include "main_frame.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/process.h>  // wxExecute
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

// Construtor: carrega este frame a partir do XRC e prepara a UI
MainFrame::MainFrame()
    : wxFrame()  // construtor vazio (não cria GtkWindow ainda)
{
    // 1) Constrói este frame a partir do recurso XRC (ID_WXFRAME)
    if (!wxXmlResource::Get()->LoadFrame(this, nullptr, "ID_WXFRAME"))
    {
        wxLogError("Falha ao carregar ID_WXFRAME do XRC.");
        return;
    }

    // 2) Ajustes visuais preferidos
    SetSize(wxSize(900, 600));
    Centre();

    // 3) Organiza a UI
    CacheControls();
    SetupLabels();
    SetupListColumns();
    BindEvents();

    // 4) Carrega a lista ao abrir (como no seu código original)
    LoadInstalledPackages();

    // 5) Recalcula layout (a janela será mostrada no main.cpp)
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

// ---------------------- Lógica (suas funções) ----------------------

void MainFrame::LoadInstalledPackages()
{
    if (!packageList_ || !out_)
        return;

    packageList_->DeleteAllItems();
    out_->AppendText("Carregando pacotes instalados...\n");

    auto cmd = wxString::Format("%s -W'' -f='${%s}|${%s}|${%s}'\n",
                                "dpkg-query",
                                "Package",
                                "Version",
                                "Architecture");

    wxString output = ExecuteCommand(cmd);

    auto lines = wxSplit(output, '\n');
    for (size_t i = 0; i < lines.GetCount(); ++i)
    {
        auto line = lines[i].Trim();
        if (line.IsEmpty())
            continue;

        auto parts = wxSplit(line, '|');
        if (parts.GetCount() >= 3)
        {
            long idx = packageList_->InsertItem(packageList_->GetItemCount(),
                                                parts[0]);
            packageList_->SetItem(idx, 1, parts[1]);
            packageList_->SetItem(idx, 2, parts[2]);
        }
    }

    out_->AppendText(wxString::Format("Total: %d pacotes instalados\n",
                                      packageList_->GetItemCount()));
}

void MainFrame::OnRefresh(wxCommandEvent&)
{
    LoadInstalledPackages();
}

void MainFrame::OnUpdate(wxCommandEvent&)
{
    if (!out_)
        return;
    out_->Clear();
    out_->AppendText("Executando: sudo apt update\n");
    out_->AppendText("===============================\n");

    // Executar comando com privilégios de root (pede senha)
    wxString result = ExecuteCommandWithSudo("apt update");

    out_->AppendText(result);
    out_->AppendText("\n===============================\n");
    out_->AppendText("Comando concluído!\n");
}

void MainFrame::OnUpgrade(wxCommandEvent&)
{
    if (!out_)
        return;

    auto answer = wxMessageBox(
        "Deseja realmente atualizar os pacotes? Esta operação pode demorar.",
        "Confirmar Upgrade",
        wxYES_NO | wxICON_QUESTION,
        this);

    if (answer == wxYES)
    {
        out_->Clear();
        out_->AppendText("Executando: sudo apt upgrade -y\n");
        out_->AppendText("===============================\n");

        // Executar upgrade com privilégios de root
        wxString result = ExecuteCommandWithSudo("apt upgrade -y");

        out_->AppendText(result);
        out_->AppendText("\n===============================\n");
        out_->AppendText("Comando concluído!\n");

        // Recarregar lista de pacotes após atualização
        LoadInstalledPackages();
    }
}

void MainFrame::OnListUpgradable(wxCommandEvent&)
{
    if (!out_)
        return;

    out_->Clear();
    out_->AppendText("Pacotes que podem ser atualizados:\n");
    out_->AppendText("===============================\n");

    // Listar pacotes que têm atualização disponível
    wxString result = ExecuteCommand("apt list --upgradable 2>/dev/null");

    out_->AppendText(result);
    out_->AppendText("===============================\n");
}

// ---------------------- Helpers ----------------------

wxString MainFrame::ExecuteCommand(const wxString& command)
{
    wxArrayString output;
    wxArrayString errors;

    // Executa sincronicamente (bloqueia a UI enquanto roda)
    wxExecute(command, output, errors, wxEXEC_SYNC);

    wxString result;
    for (size_t i = 0; i < output.GetCount(); ++i)
        result += output[i] + "\n";

    // (Opcional) anexar stderr ao final:
    // for (size_t i = 0; i < errors.GetCount(); ++i)
    //     result += "[err] " + errors[i] + "\n";

    return result;
}

wxString MainFrame::ExecuteCommandWithSudo(const wxString& command)
{
    // pkexec = PolicyKit Execute (substituto gráfico do sudo)
    wxString fullCommand = "pkexec " + command;
    return ExecuteCommand(fullCommand);
}

void MainFrame::AppendOut(const wxString& s)
{
    if (out_)
        out_->AppendText(s);
}
