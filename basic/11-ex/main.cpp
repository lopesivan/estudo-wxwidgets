#include <wx/wx.h>

// Classe principal da janela
class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Centralizando controles com wxBoxSizer", wxDefaultPosition, wxSize(600, 400))
    {
        // Sizer vertical principal
        auto verticalSizer = new wxBoxSizer(wxVERTICAL);

        // Sizer horizontal interno
        auto horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

        // Painel que será centralizado
        auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(400, 300)));
        panel->SetBackgroundColour(wxColour(200, 100, 100));

        // Adiciona o painel ao sizer horizontal, centralizando horizontalmente
        horizontalSizer->Add(panel, 0, wxALIGN_CENTER);

        // Adiciona o sizer horizontal dentro do vertical, centralizando verticalmente
        verticalSizer->Add(horizontalSizer, 1, wxALIGN_CENTER); // Note o "1" — dá espaço para centralizar

        // Define o sizer principal
        this->SetSizerAndFit(verticalSizer);

        // Título da janela e posicionamento central
        SetTitle("Exemplo: Centralizando em X e Y");
        Centre();
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

