#include <wx/wx.h>
#include <wx/listctrl.h>

// Classe principal da janela
class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame (nullptr, wxID_ANY, "Exemplo com wxListView e wxBoxSizer", wxDefaultPosition, wxSize (600, 400))
    {
        auto sizer = new wxBoxSizer (wxVERTICAL);

        // Painel principal
        auto panel = new wxPanel (this, wxID_ANY, wxDefaultPosition, FromDIP (wxSize (400, 250)));

        // Lista de itens
        auto list = new wxListView (panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        list->InsertColumn (0, "Column 1");
        list->SetColumnWidth (0, FromDIP (100));

        for (int i = 0; i < 5; i++)
        {
            list->InsertItem (i, wxString::Format ("Item %d", i));
        }

        // Texto descritivo
        auto itemDesc = new wxStaticText (panel, wxID_ANY,
                                          "Here is a description of the selected item.\n\n"
                                          "It can be as long as you want it to be,\n"
                                          "even spanning multiple lines.");

        // Sizer interno do painel
        auto panelSizer = new wxBoxSizer (wxHORIZONTAL);
        panelSizer->Add (list, 1, wxEXPAND | wxRIGHT, FromDIP (10));
        panelSizer->Add (itemDesc, 2, wxEXPAND);

        panel->SetSizerAndFit (panelSizer);

        // Botão
        auto button = new wxButton (this, wxID_ANY, "Click Me!");

        // Layout principal
        sizer->Add (panel, 1, wxEXPAND | wxALL, FromDIP (10));
        sizer->Add (button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP (10));

        this->SetSizerAndFit (sizer);

        // Eventos
        button->Bind (wxEVT_BUTTON, &MyFrame::OnButtonClick, this);
        list->Bind (wxEVT_LIST_ITEM_SELECTED, &MyFrame::OnItemSelected, this);

        // Guardar ponteiros para uso posterior
        this->listCtrl = list;
        this->descText = itemDesc;
    }

private:
    wxListView* listCtrl = nullptr;
    wxStaticText* descText = nullptr;

    void OnButtonClick (wxCommandEvent& event)
    {
        wxMessageBox ("Você clicou no botão!", "Mensagem", wxOK | wxICON_INFORMATION);
    }

    void OnItemSelected (wxListEvent& event)
    {
        int index = event.GetIndex();
        wxString text = wxString::Format ("Você selecionou o item %d.\n\nEste é o texto descritivo para o item.", index);
        descText->SetLabel (text);
        Layout(); // Atualiza o layout após mudar o texto
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

// Macro que inicializa a aplicação
wxIMPLEMENT_APP (MyApp);
