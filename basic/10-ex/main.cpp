#include <wx/wx.h>
#include <wx/listctrl.h>

// Classe principal da janela
class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Exemplo com Sizers Aninhados (wxListView + wxStaticText)", wxDefaultPosition, wxSize(600, 400))
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);

        // Lista
        auto list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        list->InsertColumn(0, "Column 1");
        list->SetColumnWidth(0, FromDIP(100));

        for (int i = 0; i < 5; i++)
        {
            list->InsertItem(i, wxString::Format("Item %d", i));
        }

        // Texto descritivo
        auto itemDesc = new wxStaticText(this, wxID_ANY,
            "Here is a description of the selected item.\n\n"
            "It can be as long as you want it to be,\n"
            "even spanning multiple lines.");

        // Sizer horizontal (lista + descrição)
        auto listPickerSizer = new wxBoxSizer(wxHORIZONTAL);
        listPickerSizer->Add(list, 1, wxEXPAND | wxRIGHT, FromDIP(10));
        listPickerSizer->Add(itemDesc, 2, wxEXPAND);

        // Botão
        auto button = new wxButton(this, wxID_ANY, "Click Me!");

        // Sizer principal
        sizer->Add(listPickerSizer, 1, wxEXPAND | wxALL, FromDIP(10));
        sizer->Add(button, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));

        this->SetSizerAndFit(sizer);

        // Eventos
        button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this);
        list->Bind(wxEVT_LIST_ITEM_SELECTED, &MyFrame::OnItemSelected, this);

        // Guardar ponteiros para acesso futuro
        this->listCtrl = list;
        this->descText = itemDesc;
    }

private:
    wxListView* listCtrl = nullptr;
    wxStaticText* descText = nullptr;

    void OnButtonClick(wxCommandEvent&)
    {
        wxMessageBox("Você clicou no botão!", "Mensagem", wxOK | wxICON_INFORMATION);
    }

    void OnItemSelected(wxListEvent& event)
    {
        int index = event.GetIndex();
        wxString text = wxString::Format(
            "Você selecionou o item %d.\n\n"
            "Esta é uma descrição detalhada para o item selecionado.\n"
            "Ela pode ter várias linhas, se necessário.",
            index);
        descText->SetLabel(text);
        Layout(); // Atualiza layout após mudar o texto
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

