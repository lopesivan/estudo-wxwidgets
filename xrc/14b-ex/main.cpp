#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/checkbox.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/wizard.h>
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxInitAllImageHandlers();

    wxXmlResource::Get()->InitAllHandlers();

    if (!wxXmlResource::Get()->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    wxWizard *wiz = new wxWizard();
    if (!wxXmlResource::Get()->LoadObject(wiz, nullptr, "ID_WXWIZARD",
                                          "wxWizard")) {
      wxLogError("Falha ao carregar ID_WXWIZARD");
      delete wiz;
      return false;
    }

    wxWizardPageSimple *p1 = new wxWizardPageSimple(wiz);
    wxWizardPageSimple *p2 = new wxWizardPageSimple(wiz);
    wxWizardPageSimple *p3 = new wxWizardPageSimple(wiz);

    wxPanel *c1 = wxXmlResource::Get()->LoadPanel(p1, "PAGE_1");
    wxPanel *c2 = wxXmlResource::Get()->LoadPanel(p2, "PAGE_2");
    wxPanel *c3 = wxXmlResource::Get()->LoadPanel(p3, "PAGE_3");

    if (!c1 || !c2 || !c3) {
      wxLogError("Falha ao carregar painéis.");
      delete wiz;
      return false;
    }

    wxBoxSizer *s1 = new wxBoxSizer(wxVERTICAL);
    s1->Add(c1, 1, wxEXPAND);
    p1->SetSizer(s1);

    wxBoxSizer *s2 = new wxBoxSizer(wxVERTICAL);
    s2->Add(c2, 1, wxEXPAND);
    p2->SetSizer(s2);

    wxBoxSizer *s3 = new wxBoxSizer(wxVERTICAL);
    s3->Add(c3, 1, wxEXPAND);
    p3->SetSizer(s3);

    wxWizardPageSimple::Chain(p1, p2);
    wxWizardPageSimple::Chain(p2, p3);

    wiz->Bind(wxEVT_WIZARD_PAGE_CHANGED, [p3, c1, c2, c3](wxWizardEvent &e) {
      if (e.GetPage() != p3) {
        e.Skip();
        return;
      }

      wxTextCtrl *userCtrl =
          dynamic_cast<wxTextCtrl *>(c2->FindWindow(XRCID("TEXT_USERNAME")));
      wxTextCtrl *mailCtrl =
          dynamic_cast<wxTextCtrl *>(c2->FindWindow(XRCID("TEXT_EMAIL")));
      wxCheckBox *termsChk =
          dynamic_cast<wxCheckBox *>(c1->FindWindow(XRCID("CHECK_TERMS")));
      wxStaticText *summary =
          dynamic_cast<wxStaticText *>(c3->FindWindow(XRCID("TEXT_SUMMARY")));

      if (summary) {
        wxString user = userCtrl ? userCtrl->GetValue() : "Não informado";
        wxString mail = mailCtrl ? mailCtrl->GetValue() : "Não informado";
        wxString terms = (termsChk && termsChk->GetValue()) ? "Sim" : "Não";

        wxString texto = "iiResumo da configuração:\n\n";
        texto += "Nome de usuário: " + user + "\n";
        texto += "Email: " + mail + "\n";
        texto += "Termos aceitos: " + terms + "\n\n";
        texto += "Clique em 'Concluir' para finalizar.";

        summary->SetLabel(texto);
        c3->Layout();
      }
      e.Skip();
    });

    wiz->FitToPage(p1);
    wiz->CentreOnScreen();

    bool ok = wiz->RunWizard(p1);
    wxLogMessage(ok ? "Sucesso" : "Cancelado");

    wiz->Destroy();

    return false;
  }
};

wxIMPLEMENT_APP(MyApp);
