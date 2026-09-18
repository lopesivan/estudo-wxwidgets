#include <wx/wx.h>
#include <wx/statline.h>

// Classe da aplicação principal
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// Classe da janela principal
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

private:
    // Componentes da interface
    wxTextCtrl* textInput;
    wxTextCtrl* textOutput;
    wxButton* btnProcess;
    wxButton* btnClear;
    wxStaticText* statusLabel;
    
    // Event handlers
    void OnProcess(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    // IDs dos eventos
    enum {
        ID_Process = 1,
        ID_Clear = 2
    };
    
    wxDECLARE_EVENT_TABLE();
};

// Tabela de eventos
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_Process, MyFrame::OnProcess)
    EVT_BUTTON(ID_Clear, MyFrame::OnClear)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

// Macro para inicializar a aplicação
wxIMPLEMENT_APP(MyApp);

// Inicialização da aplicação
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("wxWidgets Demo Application");
    frame->Show(true);
    return true;
}

// Construtor da janela
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 500)) {
    
    // Cria o menu
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT, "&Sair\tCtrl-Q", "Sair da aplicação");
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&Sobre\tF1", "Informações sobre o programa");
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Arquivo");
    menuBar->Append(menuHelp, "&Ajuda");
    SetMenuBar(menuBar);
    
    // Cria a barra de status
    CreateStatusBar();
    SetStatusText("Bem-vindo ao wxWidgets!");
    
    // Painel principal
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    
    // Sizer principal (layout vertical)
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Título
    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, 
        "🎨 Demonstração wxWidgets", 
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(16);
    titleFont.MakeBold();
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxEXPAND, 10);
    
    // Separador
    mainSizer->Add(new wxStaticLine(panel, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    
    // Label para entrada
    wxStaticText* labelInput = new wxStaticText(panel, wxID_ANY, "Digite seu texto:");
    mainSizer->Add(labelInput, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    
    // Campo de entrada
    textInput = new wxTextCtrl(panel, wxID_ANY, "", 
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    mainSizer->Add(textInput, 1, wxALL | wxEXPAND, 10);
    
    // Sizer horizontal para os botões
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    btnProcess = new wxButton(panel, ID_Process, "✓ Processar");
    btnProcess->SetBackgroundColour(wxColour(100, 200, 100));
    buttonSizer->Add(btnProcess, 1, wxALL, 5);
    
    btnClear = new wxButton(panel, ID_Clear, "✗ Limpar");
    btnClear->SetBackgroundColour(wxColour(255, 150, 150));
    buttonSizer->Add(btnClear, 1, wxALL, 5);
    
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    
    // Separador
    mainSizer->Add(new wxStaticLine(panel, wxID_ANY), 0, wxEXPAND | wxALL, 5);
    
    // Label para saída
    wxStaticText* labelOutput = new wxStaticText(panel, wxID_ANY, "Resultado:");
    mainSizer->Add(labelOutput, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    
    // Campo de saída (read-only)
    textOutput = new wxTextCtrl(panel, wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize, 
        wxTE_MULTILINE | wxTE_READONLY);
    textOutput->SetBackgroundColour(wxColour(240, 240, 240));
    mainSizer->Add(textOutput, 1, wxALL | wxEXPAND, 10);
    
    // Status label
    statusLabel = new wxStaticText(panel, wxID_ANY, 
        "💡 Dica: Digite algo e clique em 'Processar'");
    statusLabel->SetForegroundColour(wxColour(100, 100, 200));
    mainSizer->Add(statusLabel, 0, wxALL | wxEXPAND, 10);
    
    // Aplica o sizer ao painel
    panel->SetSizer(mainSizer);
    
    // Centraliza a janela na tela
    Centre();
}

// Handler do botão "Processar"
void MyFrame::OnProcess(wxCommandEvent& event) {
    wxString input = textInput->GetValue();
    
    if (input.IsEmpty()) {
        wxMessageBox("Por favor, digite algo primeiro!", 
                     "Aviso", wxOK | wxICON_WARNING);
        return;
    }
    
    // Processa o texto (converte para maiúsculas e adiciona info)
    wxString output;
    output << "Texto processado:\n";
    output << "================\n\n";
    output << input.Upper() << "\n\n";
    output << "Estatísticas:\n";
    output << "- Caracteres: " << input.Length() << "\n";
    output << "- Linhas: " << input.Freq('\n') + 1 << "\n";
    
    textOutput->SetValue(output);
    SetStatusText("Texto processado com sucesso!");
    statusLabel->SetLabel("✓ Processamento concluído!");
}

// Handler do botão "Limpar"
void MyFrame::OnClear(wxCommandEvent& event) {
    textInput->Clear();
    textOutput->Clear();
    SetStatusText("Campos limpos");
    statusLabel->SetLabel("💡 Dica: Digite algo e clique em 'Processar'");
}

// Handler do menu "Sair"
void MyFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}

// Handler do menu "Sobre"
void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox(
        "wxWidgets Demo Application\n\n"
        "Versão: 1.0\n"
        "Compilado com wxWidgets 3.2.8\n\n"
        "Este é um exemplo de aplicação GUI\n"
        "criada com wxWidgets e Conan!",
        "Sobre", 
        wxOK | wxICON_INFORMATION
    );
}
