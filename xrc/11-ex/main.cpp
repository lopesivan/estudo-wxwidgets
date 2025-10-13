#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/checkbox.h>
#include <wx/image.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/wizard.h>

#include <wx/xrc/xh_wizrd.h> // handler do wizard
#include <wx/xrc/xmlres.h>

// ------------------------------------------------------------
// Wizard personalizado (só lógica de evento)
// ------------------------------------------------------------
class MyWizard : public wxWizard {
public:
  explicit MyWizard(wxWindow *parent)
      : wxWizard() // construtor vazio; XRC vai popular
  {
    if (!wxXmlResource::Get()->LoadObject(this, parent, "ID_WXWIZARD",
                                          "wxWizard")) {
      wxLogError("Falha ao carregar o wizard do XRC (ID_WXWIZARD).");
      return;
    }

    Bind(wxEVT_WIZARD_PAGE_CHANGED, &MyWizard::OnPageChanged, this);
    Bind(wxEVT_WIZARD_FINISHED, &MyWizard::OnFinished, this);
  }

  bool ChainPagesFromXrc(wxWizardPageSimple *&p1, wxWizardPageSimple *&p2,
                         wxWizardPageSimple *&p3) {
    p1 = wxDynamicCast(FindWindow("PAGE_1"), wxWizardPageSimple);
    p2 = wxDynamicCast(FindWindow("PAGE_2"), wxWizardPageSimple);
    p3 = wxDynamicCast(FindWindow("PAGE_3"), wxWizardPageSimple);
    if (!(p1 && p2 && p3)) {
      wxLogError("Falha ao localizar PAGE_1, PAGE_2 ou PAGE_3 no XRC.");
      return false;
    }
    wxWizardPageSimple::Chain(p1, p2);
    wxWizardPageSimple::Chain(p2, p3);
    return true;
  }

private:
  void OnPageChanged(wxWizardEvent &event) {
    if (event.GetPage() && event.GetPage()->GetName() == "PAGE_3") {
      UpdateSummaryPage();
    }
    event.Skip();
  }

  void OnFinished(wxWizardEvent &event) {
    wxMessageBox("Wizard concluído com sucesso!", "Concluído",
                 wxOK | wxICON_INFORMATION, this);
    event.Skip();
  }

  void UpdateSummaryPage() {
    auto *summaryText = wxDynamicCast(FindWindow("TEXT_SUMMARY"), wxStaticText);
    auto *termsCheck = wxDynamicCast(FindWindow("CHECK_TERMS"), wxCheckBox);
    const wxString user = GetFieldText("TEXT_USERNAME");
    const wxString mail = GetFieldText("TEXT_EMAIL");
    const wxString termsAccepted =
        (termsCheck && termsCheck->GetValue()) ? "Sim" : "Não";

    if (summaryText) {
      summaryText->SetLabel(
          wxString::Format("Resumo da configuração:\n\n"
                           "Nome de usuário: %s\n"
                           "Email: %s\n"
                           "Termos aceitos: %s\n\n"
                           "Clique em 'Concluir' para finalizar.",
                           user, mail, termsAccepted));
      if (auto *page = wxDynamicCast(FindWindow("PAGE_3"), wxWizardPage))
        if (page->GetSizer())
          page->GetSizer()->Layout();
    }
  }

  wxString GetFieldText(const wxString &name) {
    if (auto *t = wxDynamicCast(FindWindow(name), wxTextCtrl))
      return t->GetValue();
    return "Não informado";
  }
};

// ------------------------------------------------------------
// App
// ------------------------------------------------------------
class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxLog::SetActiveTarget(new wxLogStderr());
    wxInitAllImageHandlers();

    auto *xr = wxXmlResource::Get();
    xr->InitAllHandlers();
    xr->AddHandler(new wxWizardXmlHandler()); // <-- só este já basta

    if (!xr->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    auto *wiz = new MyWizard(nullptr);

    wxWizardPageSimple *p1 = nullptr, *p2 = nullptr, *p3 = nullptr;
    if (!wiz->ChainPagesFromXrc(p1, p2, p3)) {
      wiz->Destroy();
      return false;
    }

    wiz->CentreOnScreen();
    const bool ok = wiz->RunWizard(p1);
    wxLogMessage("Wizard finalizado: %s", ok ? "Sucesso" : "Cancelado");
    wiz->Destroy();
    return false;
  }
};

wxIMPLEMENT_APP(MyApp);
