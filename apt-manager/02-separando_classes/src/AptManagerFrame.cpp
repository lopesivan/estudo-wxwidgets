#include "AptManagerFrame.h"

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
    CreateControls();  // Cria todos os controles da interface
    Centre();          // Centraliza a janela na tela
}

void AptManagerFrame::CreateControls()
{
    // Painel principal que contém todos os controles
    auto* panel = new wxPanel(this);

    // Sizer vertical: organiza os elementos de cima para baixo
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    // ====================================================================
    // SEÇÃO 1: BOTÕES DE AÇÃO (topo da janela)
    // ====================================================================
    auto* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Criar os 4 botões principais com tamanhos adequados
    btnRefresh = new wxButton(panel,
                              wxID_ANY,
                              "Atualizar Lista",
                              wxDefaultPosition,
                              wxSize(180, -1));  // não tem acentuação

    btnUpdate = new wxButton(panel,
                             wxID_ANY,
                             "APT Update",
                             wxDefaultPosition,
                             wxSize(180, -1));  // não tem acentuação

    btnUpgrade = new wxButton(panel,
                              wxID_ANY,
                              "APT Upgrade",
                              wxDefaultPosition,
                              wxSize(180, -1));  // garante UTF-8);

    // Este botão precisa ser maior para caber o texto completo
    btnListUpgradable = new wxButton(
        panel,
        wxID_ANY,
        wxString::FromUTF8(u8"Pacotes Atualizáveis"));  // garante UTF-8);

    // Adicionar botões ao sizer horizontal (lado a lado)
    buttonSizer->Add(btnRefresh, 0, wxALL, 5);
    buttonSizer->Add(btnUpdate, 0, wxALL, 5);
    buttonSizer->Add(btnUpgrade, 0, wxALL, 5);
    buttonSizer->Add(btnListUpgradable, 0, wxALL, 5);

    // deixe o sizer decidir o tamanho ideal:
    // btnListUpgradable->SetMinSize(btnListUpgradable->GetBestSize());

    // ====================================================================
    // SEÇÃO 2: LISTA DE PACOTES (meio da janela)
    // ====================================================================
    packageList = new wxListCtrl(panel,
                                 wxID_ANY,
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 wxLC_REPORT | wxLC_SINGLE_SEL);

    // Criar 3 colunas na tabela
    packageList->AppendColumn("Pacote", wxLIST_FORMAT_LEFT, 300);
    packageList->AppendColumn("Versão", wxLIST_FORMAT_LEFT, 200);
    packageList->AppendColumn("Arquitetura", wxLIST_FORMAT_LEFT, 100);

    // ====================================================================
    // SEÇÃO 3: ÁREA DE TEXTO PARA SAÍDA (rodapé da janela)
    // ====================================================================
    outputText = new wxTextCtrl(panel,
                                wxID_ANY,
                                "",
                                wxDefaultPosition,
                                wxSize(-1, 150),
                                wxTE_MULTILINE | wxTE_READONLY);

    // ====================================================================
    // ORGANIZAÇÃO DO LAYOUT
    // ====================================================================
    mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(new wxStaticText(panel, wxID_ANY, "Pacotes Instalados:"),
                   0,
                   wxLEFT | wxTOP,
                   10);
    mainSizer->Add(
        packageList, 1, wxALL | wxEXPAND, 5);  // 1 = expande verticalmente
    mainSizer->Add(
        new wxStaticText(panel, wxID_ANY, wxString::FromUTF8(u8"Saída:")),
        0,
        wxLEFT | wxTOP,
        10);
    mainSizer->Add(outputText, 0, wxALL | wxEXPAND, 5);

    panel->SetSizer(mainSizer);

    // ====================================================================
    // CONECTAR EVENTOS DOS BOTÕES
    // ====================================================================
    btnRefresh->Bind(wxEVT_BUTTON, &AptManagerFrame::OnRefresh, this);
    btnUpdate->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpdate, this);
    btnUpgrade->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpgrade, this);
    btnListUpgradable->Bind(
        wxEVT_BUTTON, &AptManagerFrame::OnListUpgradable, this);

    // Carregar lista de pacotes assim que a janela abrir
    LoadInstalledPackages();
}

void AptManagerFrame::LoadInstalledPackages()
{
    packageList->DeleteAllItems();
    outputText->AppendText("Carregando pacotes instalados...\n");

    auto cmd = wxString::Format("%s -W'' -f='${%s}|${%s}|${%s}'\n",
                                "dpkg-query",
                                "Package",
                                "Version",
                                "Architecture");
    auto output = ExecuteCommand(cmd);

    auto lines = wxSplit(output, '\n');
    for (size_t i = 0; i < lines.GetCount(); i++)
    {
        auto line = lines[i].Trim();
        if (!line.IsEmpty())
        {
            auto parts = wxSplit(line, '|');
            if (parts.GetCount() >= 3)
            {
                auto idx = packageList->InsertItem(packageList->GetItemCount(),
                                                   parts[0]);
                packageList->SetItem(idx, 1, parts[1]);
                packageList->SetItem(idx, 2, parts[2]);
            }
        }
    }

    outputText->AppendText(wxString::Format("Total: %d pacotes instalados\n",
                                            packageList->GetItemCount()));
}

// ========================================================================
// HANDLERS DE EVENTOS
// ========================================================================
void AptManagerFrame::OnRefresh(wxCommandEvent& event)
{
    LoadInstalledPackages();
}

void AptManagerFrame::OnUpdate(wxCommandEvent& event)
{
    outputText->Clear();
    outputText->AppendText("Executando: sudo apt update\n");
    outputText->AppendText("===============================\n");

    // Executar comando com privilégios de root (pede senha)
    auto output = ExecuteCommandWithSudo("apt update");

    outputText->AppendText(output);
    outputText->AppendText("\n===============================\n");
    outputText->AppendText("Comando concluído!\n");
}

void AptManagerFrame::OnUpgrade(wxCommandEvent& event)
{
    // Pedir confirmação antes de atualizar
    auto answer =
        wxMessageBox("Deseja realmente atualizar os pacotes? Esta operação "
                     "pode demorar.",
                     "Confirmar Upgrade",
                     wxYES_NO | wxICON_QUESTION);

    if (answer == wxYES)
    {
        outputText->Clear();
        outputText->AppendText("Executando: sudo apt upgrade -y\n");
        outputText->AppendText("===============================\n");

        // Executar upgrade com privilégios de root
        auto output = ExecuteCommandWithSudo("apt upgrade -y");

        outputText->AppendText(output);
        outputText->AppendText("\n===============================\n");
        outputText->AppendText("Comando concluído!\n");

        // Recarregar lista de pacotes após atualização
        LoadInstalledPackages();
    }
}

void AptManagerFrame::OnListUpgradable(wxCommandEvent& event)
{
    outputText->Clear();
    outputText->AppendText("Pacotes que podem ser atualizados:\n");
    outputText->AppendText("===============================\n");

    // Listar pacotes que têm atualização disponível
    wxString output = ExecuteCommand("apt list --upgradable 2>/dev/null");

    outputText->AppendText(output);
    outputText->AppendText("===============================\n");
}

// ========================================================================
// FUNÇÕES AUXILIARES
// ========================================================================
wxString AptManagerFrame::ExecuteCommand(const wxString& command)
{
    wxArrayString output;  // Array para armazenar linhas de saída
    wxArrayString errors;  // Array para armazenar erros

    // Executar comando de forma síncrona (espera terminar)
    wxExecute(command, output, errors, wxEXEC_SYNC);

    // Concatenar todas as linhas de saída em uma única string
    wxString result;
    for (size_t i = 0; i < output.GetCount(); i++)
    {
        result += output[i] + "\n";
    }

    return result;
}

wxString AptManagerFrame::ExecuteCommandWithSudo(const wxString& command)
{
    // pkexec = PolicyKit Execute (substituto gráfico do sudo)
    auto fullCommand = "pkexec " + command;
    return ExecuteCommand(fullCommand);
}
