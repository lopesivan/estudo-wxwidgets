#include "AptManagerFrame.h"

#include <wx/process.h>
#include <wx/txtstrm.h>

// --- Helpers internos (somente neste TU) ---
static bool ParseAptInstalledLine(const wxString& line,
                                  wxString& pkg,
                                  wxString& ver,
                                  wxString& arch)
{
    wxString s = line.Strip(wxString::both);
    if (s.IsEmpty())
        return false;
    if (s.StartsWith("Listing"))
        return false;  // cabeçalho do apt

    // Formato típico:
    //   "bash/jammy,now 5.1-6ubuntu1 amd64 [installed]"
    //   "libfoo/stable,now 1.2.3-1 amd64 [installed,automatic]"
    wxArrayString parts = wxSplit(s, ' ');
    if (parts.GetCount() < 3)
        return false;

    pkg = parts[0].BeforeFirst('/');
    ver = parts[1];
    arch = parts[2];
    return !(pkg.IsEmpty() || ver.IsEmpty() || arch.IsEmpty());
}

// ============================================================================
// CLASSE PRINCIPAL: AptManagerFrame
// ============================================================================
AptManagerFrame::AptManagerFrame()
    : wxFrame(nullptr,
              wxID_ANY,
              "Gerenciador de Pacotes APT",
              wxDefaultPosition,
              wxSize(900, 600))
{
    CreateControls();
    Centre();
    LoadInstalledPackages();
}

void AptManagerFrame::CreateControls()
{
    auto* panel = new wxPanel(this);
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    // ---------------- Botões (topo) ----------------
    auto* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    btnRefresh = new wxButton(
        panel, wxID_ANY, "Atualizar Lista", wxDefaultPosition, wxSize(180, -1));
    btnUpdate = new wxButton(
        panel, wxID_ANY, "APT Update", wxDefaultPosition, wxSize(180, -1));
    btnUpgrade = new wxButton(
        panel, wxID_ANY, "APT Upgrade", wxDefaultPosition, wxSize(180, -1));

    // Label com acentos em UTF-8
    btnListUpgradable = new wxButton(
        panel, wxID_ANY, wxString::FromUTF8(u8"Pacotes Atualizáveis"));
    // (opcional) deixe o sizer calcular tamanho ideal:
    // btnListUpgradable->SetMinSize(btnListUpgradable->GetBestSize());

    buttonSizer->Add(btnRefresh, 0, wxALL, 5);
    buttonSizer->Add(btnUpdate, 0, wxALL, 5);
    buttonSizer->Add(btnUpgrade, 0, wxALL, 5);
    buttonSizer->Add(btnListUpgradable, 0, wxALL, 5);

    // ---------------- Lista de pacotes ----------------
    packageList = new wxListCtrl(panel,
                                 wxID_ANY,
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 wxLC_REPORT | wxLC_SINGLE_SEL);
    packageList->AppendColumn("Pacote", wxLIST_FORMAT_LEFT, 300);
    packageList->AppendColumn("Versão", wxLIST_FORMAT_LEFT, 200);
    packageList->AppendColumn("Arquitetura", wxLIST_FORMAT_LEFT, 100);

    // ---------------- Saída (rodapé) ----------------
    outputText = new wxTextCtrl(panel,
                                wxID_ANY,
                                "",
                                wxDefaultPosition,
                                wxSize(-1, 150),
                                wxTE_MULTILINE | wxTE_READONLY);

    // Layout
    mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(new wxStaticText(panel, wxID_ANY, "Pacotes Instalados:"),
                   0,
                   wxLEFT | wxTOP,
                   10);
    mainSizer->Add(packageList, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(
        new wxStaticText(panel, wxID_ANY, wxString::FromUTF8(u8"Saída:")),
        0,
        wxLEFT | wxTOP,
        10);
    mainSizer->Add(outputText, 0, wxALL | wxEXPAND, 5);

    panel->SetSizer(mainSizer);

    // Eventos
    btnRefresh->Bind(wxEVT_BUTTON, &AptManagerFrame::OnRefresh, this);
    btnUpdate->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpdate, this);
    btnUpgrade->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpgrade, this);
    btnListUpgradable->Bind(
        wxEVT_BUTTON, &AptManagerFrame::OnListUpgradable, this);
}

void AptManagerFrame::LoadInstalledPackages()
{
    packageList->DeleteAllItems();
    outputText->AppendText("Carregando pacotes instalados (via apt)...\n");

    // Evito redirecionamento shell aqui; wxExecute trata stdout/err
    // separadamente.
    const wxString cmd = "apt list --installed";
    wxArrayString out, err;
    wxExecute(cmd, out, err, wxEXEC_SYNC);

    long count = 0;
    for (const auto& raw : out)
    {
        wxString pkg, ver, arch;
        if (!ParseAptInstalledLine(raw, pkg, ver, arch))
            continue;

        long idx = packageList->InsertItem(packageList->GetItemCount(), pkg);
        packageList->SetItem(idx, 1, ver);
        packageList->SetItem(idx, 2, arch);
        ++count;
    }

    if (!err.IsEmpty())
    {
        outputText->AppendText("[Aviso] Mensagens do apt em stderr:\n");
        for (const auto& e : err)
            outputText->AppendText(e + "\n");
    }

    outputText->AppendText(
        wxString::Format("Total: %ld pacotes instalados\n", count));
}

// =================== Eventos ===================

void AptManagerFrame::OnRefresh(wxCommandEvent&)
{
    LoadInstalledPackages();
}

void AptManagerFrame::OnUpdate(wxCommandEvent&)
{
    outputText->Clear();
    outputText->AppendText("Executando: sudo apt update\n");
    outputText->AppendText("===============================\n");

    wxString output = ExecuteCommandWithSudo("apt update");

    outputText->AppendText(output);
    outputText->AppendText("\n===============================\n");
    outputText->AppendText("Comando concluído!\n");
}

void AptManagerFrame::OnUpgrade(wxCommandEvent&)
{
    int answer = wxMessageBox(
        "Deseja realmente atualizar os pacotes? Esta operação pode demorar.",
        "Confirmar Upgrade",
        wxYES_NO | wxICON_QUESTION,
        this);
    if (answer != wxYES)
        return;

    outputText->Clear();
    outputText->AppendText("Executando: sudo apt upgrade -y\n");
    outputText->AppendText("===============================\n");

    wxString output = ExecuteCommandWithSudo("apt upgrade -y");

    outputText->AppendText(output);
    outputText->AppendText("\n===============================\n");
    outputText->AppendText("Comando concluído!\n");

    LoadInstalledPackages();
}

void AptManagerFrame::OnListUpgradable(wxCommandEvent&)
{
    outputText->Clear();
    outputText->AppendText("Pacotes que podem ser atualizados:\n");
    outputText->AppendText("===============================\n");

    // Sem root
    wxString output = ExecuteCommand("apt list --upgradable");
    outputText->AppendText(output);
    outputText->AppendText("===============================\n");
}

// =================== Execução de comandos ===================

wxString AptManagerFrame::ExecuteCommand(const wxString& command)
{
    wxArrayString output, errors;
    wxExecute(command, output, errors, wxEXEC_SYNC);

    wxString result;
    for (size_t i = 0; i < output.GetCount(); ++i)
    {
        result += output[i];
        if (!result.EndsWith("\n"))
            result += "\n";
    }
    // (Opcional) anexar stderr ao final:
    // for (size_t i = 0; i < errors.GetCount(); ++i) result += "[stderr] " +
    // errors[i] + "\n";
    return result;
}

wxString AptManagerFrame::ExecuteCommandWithSudo(const wxString& command)
{
    // pkexec = PolicyKit (prompt gráfico)
    wxString full = "pkexec " + command;
    return ExecuteCommand(full);
}
