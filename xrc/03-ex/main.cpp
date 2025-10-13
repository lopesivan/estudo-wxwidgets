#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/button.h>

class MyApp : public wxApp {
public:
    bool OnInit() override {
        wxXmlResource::Get()->InitAllHandlers();

        if (!wxXmlResource::Get()->Load("ui.xrc")) {
            wxLogError("Não foi possível carregar ui.xrc");
            return false;
        }

        wxFrame* frame = wxXmlResource::Get()->LoadFrame(nullptr, "ID_WXFRAME");
        if (!frame) {
            wxLogError("Não foi possível instanciar o frame ID_WXFRAME do XRC.");
            return false;
        }

        // 👇 Note o *frame aqui (objeto, não ponteiro) 👇
        auto* list = XRCCTRL(*frame, "ID_LISTCTRL", wxListCtrl);
        auto* out  = XRCCTRL(*frame, "ID_TEXTCTRL", wxTextCtrl);

        if (list) {
            list->AppendColumn("Pacote",  wxLIST_FORMAT_LEFT, 200);
            list->AppendColumn("Versão",  wxLIST_FORMAT_LEFT, 100);
            long idx = list->InsertItem(list->GetItemCount(), "exemplo-pkg");
            list->SetItem(idx, 1, "1.0");
        }

        // Eventos dos botões do XRC:
        frame->Bind(wxEVT_BUTTON, [out](wxCommandEvent&) {
            if (out) out->AppendText("Botão 1 clicado\n");
        }, wxXmlResource::GetXRCID("ID_BUTTON"));

        frame->Bind(wxEVT_BUTTON, [out](wxCommandEvent&) {
            if (out) out->AppendText("Botão 2 clicado\n");
        }, wxXmlResource::GetXRCID("ID_BUTTON1"));

        frame->Bind(wxEVT_BUTTON, [out](wxCommandEvent&) {
            if (out) out->AppendText("Botão 3 clicado\n");
        }, wxXmlResource::GetXRCID("ID_BUTTON2"));

        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

