// ============================================================================
// Includes necessários para wxWidgets
// ============================================================================
#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/wx.h>

// ============================================================================
// CLASSE PRINCIPAL: AptManagerFrame
// Esta classe representa a janela principal do aplicativo
// ============================================================================
class AptManagerFrame : public wxFrame
{
public:
    // Construtor: cria a janela com título e tamanho inicial
    AptManagerFrame()
        : wxFrame(nullptr,
                  wxID_ANY,
                  "Gerenciador de Pacotes APT",
                  wxDefaultPosition,
                  wxSize(900, 600))
    {
        CreateControls();  // Cria todos os controles da interface
        Centre();          // Centraliza a janela na tela
    }

private:
    // ------------------------------------------------------------------------
    // COMPONENTES DA INTERFACE (Widgets)
    // ------------------------------------------------------------------------
    wxListCtrl* packageList;      // Tabela que mostra os pacotes instalados
    wxTextCtrl* outputText;       // Área de texto para saída dos comandos
    wxButton* btnRefresh;         // Botão "Atualizar Lista"
    wxButton* btnUpdate;          // Botão "APT Update"
    wxButton* btnUpgrade;         // Botão "APT Upgrade"
    wxButton* btnListUpgradable;  // Botão "Pacotes Atualizáveis"

    // ------------------------------------------------------------------------
    // MÉTODO: CreateControls
    // Cria e organiza todos os componentes da interface gráfica
    // ------------------------------------------------------------------------
    void CreateControls()
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
        mainSizer->Add(packageList,
                       1,
                       wxALL | wxEXPAND,
                       5);  // 1 = expande verticalmente
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

    void LoadInstalledPackages()
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
                    auto idx = packageList->InsertItem(
                        packageList->GetItemCount(), parts[0]);
                    packageList->SetItem(idx, 1, parts[1]);
                    packageList->SetItem(idx, 2, parts[2]);
                }
            }
        }

        outputText->AppendText(wxString::Format(
            "Total: %d pacotes instalados\n", packageList->GetItemCount()));
    }

    // ========================================================================
    // HANDLERS DE EVENTOS (Funções chamadas quando botões são
    // clicados)
    // ========================================================================

    // ------------------------------------------------------------------------
    // EVENT HANDLER: OnRefresh
    // Chamado quando o botão "Atualizar Lista" é clicado
    // ------------------------------------------------------------------------
    void OnRefresh(wxCommandEvent& event) { LoadInstalledPackages(); }

    // ------------------------------------------------------------------------
    // EVENT HANDLER: OnUpdate
    // Chamado quando o botão "APT Update" é clicado
    // Executa: sudo apt update
    // ------------------------------------------------------------------------
    void OnUpdate(wxCommandEvent& event)
    {
        outputText->Clear();
        outputText->AppendText("Executando: sudo apt update\n");
        outputText->AppendText("===============================\n");

        // Executar comando com privilégios de root (pede senha)
        wxString output = ExecuteCommandWithSudo("apt update");

        outputText->AppendText(output);
        outputText->AppendText("\n===============================\n");
        outputText->AppendText("Comando concluído!\n");
    }

    // ------------------------------------------------------------------------
    // EVENT HANDLER: OnUpgrade
    // Chamado quando o botão "APT Upgrade" é clicado
    // Executa: sudo apt upgrade -y
    // ------------------------------------------------------------------------
    void OnUpgrade(wxCommandEvent& event)
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

    // ------------------------------------------------------------------------
    // EVENT HANDLER: OnListUpgradable
    // Chamado quando o botão "Pacotes Atualizáveis" é clicado
    // Executa: apt list --upgradable
    // ------------------------------------------------------------------------
    void OnListUpgradable(wxCommandEvent& event)
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
    // FUNÇÕES AUXILIARES (Executam comandos no sistema)
    // ========================================================================

    // ------------------------------------------------------------------------
    // FUNÇÃO: ExecuteCommand
    // Executa um comando shell e retorna sua saída como string
    // Parâmetro: command - o comando a ser executado
    // Retorno: saída do comando em formato de texto
    // ------------------------------------------------------------------------
    wxString ExecuteCommand(const wxString& command)
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

    // ------------------------------------------------------------------------
    // FUNÇÃO: ExecuteCommandWithSudo
    // Executa um comando com privilégios de root usando pkexec
    // pkexec abre uma janela gráfica pedindo a senha do usuário
    // Parâmetro: command - o comando a ser executado
    // Retorno: saída do comando em formato de texto
    // ------------------------------------------------------------------------
    wxString ExecuteCommandWithSudo(const wxString& command)
    {
        // pkexec = PolicyKit Execute (substituto gráfico do sudo)
        wxString fullCommand = "pkexec " + command;
        return ExecuteCommand(fullCommand);
    }
};

// ============================================================================
// CLASSE DA APLICAÇÃO: AptManagerApp
// Esta é a classe principal que inicializa o programa
// ============================================================================
class AptManagerApp : public wxApp
{
public:
    // ------------------------------------------------------------------------
    // MÉTODO: OnInit
    // Chamado automaticamente quando o programa inicia
    // Retorna: true se inicialização foi bem-sucedida
    // ------------------------------------------------------------------------
    virtual bool OnInit()
    {
        // Criar a janela principal
        auto* frame = new AptManagerFrame();

        // Mostrar a janela na tela
        frame->Show(true);

        return true;  // Inicialização bem-sucedida
    }
};

// ============================================================================
// MACRO: wxIMPLEMENT_APP
// Esta macro cria o ponto de entrada do programa (função main)
// Ela inicializa o wxWidgets e chama OnInit()
// ============================================================================
wxIMPLEMENT_APP(AptManagerApp);
