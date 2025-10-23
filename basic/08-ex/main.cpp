#include <wx/wx.h>

// Classe principal da janela
class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Exemplo com wxBoxSizer e wxPanel", wxDefaultPosition, wxSize(500, 350))
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);

        // Painel principal com tamanho fixo
        auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 250)));
        panel->SetBackgroundColour(wxColour(200, 100, 100));

        // Botão
        auto button = new wxButton(this, wxID_ANY, "Click Me!");

        // Adicionando os elementos ao sizer
        sizer->Add(panel, 1, wxEXPAND | wxALL, FromDIP(10));
        sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));

        // Define o sizer e ajusta o tamanho da janela
        this->SetSizerAndFit(sizer);

        // Evento para o botão
        button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this);
    }

private:
    void OnButtonClick(wxCommandEvent& event)
    {
        wxMessageBox("Você clicou no botão!", "Mensagem", wxOK | wxICON_INFORMATION);
    }
};

// Classe da aplicação
class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

// Macro que inicializa a aplicação
wxIMPLEMENT_APP(MyApp);

