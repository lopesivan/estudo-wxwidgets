#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/wx.h>

class AptManagerFrame : public wxFrame
{
public:
    AptManagerFrame()
        : wxFrame(nullptr, wxID_ANY, "Gerenciador de Pacotes APT",
                  wxDefaultPosition, wxSize(900, 600))
    {
        CreateControls();
        Centre();
    }

private:
    wxListCtrl* packageList;
    wxTextCtrl* outputText;
    wxButton* btnRefresh;
    wxButton* btnUpdate;
    wxButton* btnUpgrade;
    wxButton* btnListUpgradable;

    void CreateControls()
    {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        // Botões superiores
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

        btnRefresh = new wxButton(panel, wxID_ANY, "Atualizar Lista");
        btnUpdate = new wxButton(panel, wxID_ANY, "APT Update");
        btnUpgrade = new wxButton(panel, wxID_ANY, "APT Upgrade");
        btnListUpgradable =
            new wxButton(panel, wxID_ANY, "Pacotes Atualizáveis");

        buttonSizer->Add(btnRefresh, 0, wxALL, 5);
        buttonSizer->Add(btnUpdate, 0, wxALL, 5);
        buttonSizer->Add(btnUpgrade, 0, wxALL, 5);
        buttonSizer->Add(btnListUpgradable, 0, wxALL, 5);

        // Lista de pacotes
        packageList =
            new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxLC_REPORT | wxLC_SINGLE_SEL);
        packageList->AppendColumn("Pacote", wxLIST_FORMAT_LEFT, 300);
        packageList->AppendColumn("Versão", wxLIST_FORMAT_LEFT, 200);
        packageList->AppendColumn("Arquitetura", wxLIST_FORMAT_LEFT, 100);

        // Área de saída de texto
        outputText =
            new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition,
                           wxSize(-1, 150), wxTE_MULTILINE | wxTE_READONLY);

        mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 5);
        mainSizer->Add(
            new wxStaticText(panel, wxID_ANY, "Pacotes Instalados:"), 0,
            wxLEFT | wxTOP, 10);
        mainSizer->Add(packageList, 1, wxALL | wxEXPAND, 5);
        mainSizer->Add(new wxStaticText(panel, wxID_ANY, "Saída:"), 0,
                       wxLEFT | wxTOP, 10);
        mainSizer->Add(outputText, 0, wxALL | wxEXPAND, 5);

        panel->SetSizer(mainSizer);

        // Eventos
        btnRefresh->Bind(wxEVT_BUTTON, &AptManagerFrame::OnRefresh, this);
        btnUpdate->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpdate, this);
        btnUpgrade->Bind(wxEVT_BUTTON, &AptManagerFrame::OnUpgrade, this);
        btnListUpgradable->Bind(wxEVT_BUTTON,
                                &AptManagerFrame::OnListUpgradable, this);

        // Carregar lista inicial
        LoadInstalledPackages();
    }

    void LoadInstalledPackages()
    {
        packageList->DeleteAllItems();
        outputText->AppendText("Carregando pacotes instalados...\n");

        wxString output = ExecuteCommand(
            "dpkg-query -W -f='${Package}|${Version}|${Architecture}\\n'");

        wxArrayString lines = wxSplit(output, '\n');
        for (size_t i = 0; i < lines.GetCount(); i++)
        {
            wxString line = lines[i].Trim();
            if (!line.IsEmpty())
            {
                wxArrayString parts = wxSplit(line, '|');
                if (parts.GetCount() >= 3)
                {
                    long idx = packageList->InsertItem(
                        packageList->GetItemCount(), parts[0]);
                    packageList->SetItem(idx, 1, parts[1]);
                    packageList->SetItem(idx, 2, parts[2]);
                }
            }
        }

        outputText->AppendText(wxString::Format(
            "Total: %d pacotes instalados\n", packageList->GetItemCount()));
    }

    void OnRefresh(wxCommandEvent& event) { LoadInstalledPackages(); }

    void OnUpdate(wxCommandEvent& event)
    {
        outputText->Clear();
        outputText->AppendText("Executando: sudo apt update\n");
        outputText->AppendText("===============================\n");

        wxString output = ExecuteCommandWithSudo("apt update");
        outputText->AppendText(output);
        outputText->AppendText("\n===============================\n");
        outputText->AppendText("Comando concluído!\n");
    }

    void OnUpgrade(wxCommandEvent& event)
    {
        int answer =
            wxMessageBox("Deseja realmente atualizar os pacotes? "
                         "Esta operação pode demorar.",
                         "Confirmar Upgrade", wxYES_NO | wxICON_QUESTION);

        if (answer == wxYES)
        {
            outputText->Clear();
            outputText->AppendText("Executando: sudo apt upgrade -y\n");
            outputText->AppendText("===============================\n");

            wxString output = ExecuteCommandWithSudo("apt upgrade -y");
            outputText->AppendText(output);
            outputText->AppendText("\n===============================\n");
            outputText->AppendText("Comando concluído!\n");

            LoadInstalledPackages();
        }
    }

    void OnListUpgradable(wxCommandEvent& event)
    {
        outputText->Clear();
        outputText->AppendText("Pacotes que podem ser atualizados:\n");
        outputText->AppendText("===============================\n");

        wxString output = ExecuteCommand("apt list --upgradable 2>/dev/null");
        outputText->AppendText(output);
        outputText->AppendText("===============================\n");
    }

    wxString ExecuteCommand(const wxString& command)
    {
        wxArrayString output;
        wxArrayString errors;

        wxExecute(command, output, errors, wxEXEC_SYNC);

        wxString result;
        for (size_t i = 0; i < output.GetCount(); i++)
        {
            result += output[i] + "\n";
        }

        return result;
    }

    wxString ExecuteCommandWithSudo(const wxString& command)
    {
        wxString fullCommand = "pkexec " + command;
        return ExecuteCommand(fullCommand);
    }
};

class AptManagerApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        AptManagerFrame* frame = new AptManagerFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(AptManagerApp);
