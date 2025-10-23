#include <wx/wx.h>

// Classe principal da janela
class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame (nullptr, wxID_ANY, "Exemplo com wxBoxSizer", wxDefaultPosition, wxSize (500, 400))
    {
        auto sizer = new wxBoxSizer (wxVERTICAL);

        auto panel = new wxPanel (this, wxID_ANY, wxDefaultPosition, FromDIP (wxSize (400, 100)));
        panel->SetBackgroundColour (wxColour (200, 100, 100));

        auto otherPanel = new wxPanel (this);
        otherPanel->SetBackgroundColour (wxColour (100, 200, 100));

        auto button = new wxButton (this, wxID_ANY, "Click Me!");

        sizer->Add (panel, 1, wxEXPAND | wxALL, FromDIP (10));
        sizer->Add (otherPanel, 2, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP (10));
        sizer->Add (button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP (10));

        this->SetSizerAndFit (sizer);

        // Evento simples para o botão
        button->Bind (wxEVT_BUTTON, &MyFrame::OnButtonClick, this);
    }

private:
    void OnButtonClick (wxCommandEvent& event)
    {
        wxMessageBox ("Você clicou no botão!", "Informação", wxOK | wxICON_INFORMATION);
    }
};

// Classe da aplicação
class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new MyFrame();
        frame->Show (true);
        return true;
    }
};

// Macro para inicializar a aplicação
wxIMPLEMENT_APP (MyApp);
