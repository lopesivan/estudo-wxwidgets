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

class MyWizard {
private:
  wxWizard *wiz;
  wxPanel *c1;
  wxPanel *c2;
  wxPanel *c3;
  wxWizardPageSimple *p1;
  wxWizardPageSimple *p2;
  wxWizardPageSimple *p3;

public:
  MyWizard()
      : wiz(nullptr), c1(nullptr), c2(nullptr), c3(nullptr), p1(nullptr),
        p2(nullptr), p3(nullptr) {}

  bool Create(wxXmlResource *xr) {
    // 1) Instancia o wizard a partir do XRC
    wiz = new wxWizard();
    if (!xr->LoadObject(wiz, nullptr, "ID_WXWIZARD", "wxWizard")) {
      wxLogError("Falha ao carregar ID_WXWIZARD");
      if (wiz)
        wiz->Destroy();
      return false;
    }

    wiz->SetMinSize(wxSize(500, 400));

    // 2) Cria páginas simples
    p1 = new wxWizardPageSimple(wiz);
    p2 = new wxWizardPageSimple(wiz);
    p3 = new wxWizardPageSimple(wiz);

    // 3) Carrega painéis do XRC
    c1 = xr->LoadPanel(p1, "PAGE_1");
    c2 = xr->LoadPanel(p2, "PAGE_2");
    c3 = xr->LoadPanel(p3, "PAGE_3");

    if (!(c1 && c2 && c3)) {
      wxLogError("Falha ao carregar painéis.");
      wiz->Destroy();
      return false;
    }

    // Anexa painéis
    AttachPanel(p1, c1);
    AttachPanel(p2, c2);
    AttachPanel(p3, c3);

    // 4) Encadeia páginas
    wxWizardPageSimple::Chain(p1, p2);
    wxWizardPageSimple::Chain(p2, p3);

    // 5) Conecta evento
    wiz->Bind(wxEVT_WIZARD_PAGE_CHANGED, &MyWizard::OnPageChanged, this);

    // 6) Ajusta tamanho
    wiz->GetPageAreaSizer()->Add(p1);
    wiz->FitToPage(p1);

    return true;
  }

  void AttachPanel(wxWizardPageSimple *page, wxPanel *panel) {
    if (!panel)
      return;
    panel->SetMinSize(wxSize(450, 300));
    auto *s = new wxBoxSizer(wxVERTICAL);
    s->Add(panel, 1, wxEXPAND | wxALL, 5);
    page->SetSizer(s);
    s->SetSizeHints(page);
  }

  void OnPageChanged(wxWizardEvent &e) {
    if (e.GetPage() != p3) {
      e.Skip();
      return;
    }

    // Busca controles
    wxTextCtrl *userCtrl = nullptr;
    wxTextCtrl *mailCtrl = nullptr;
    wxCheckBox *termsChk = nullptr;
    wxStaticText *summary = nullptr;

    wxWindow *found = c2->FindWindow(XRCID("TEXT_USERNAME"));
    if (found)
      userCtrl = wxDynamicCast(found, wxTextCtrl);

    found = c2->FindWindow(XRCID("TEXT_EMAIL"));
    if (found)
      mailCtrl = wxDynamicCast(found, wxTextCtrl);

    found = c1->FindWindow(XRCID("CHECK_TERMS"));
    if (found)
      termsChk = wxDynamicCast(found, wxCheckBox);

    found = c3->FindWindow(XRCID("TEXT_SUMMARY"));
    if (found)
      summary = wxDynamicCast(found, wxStaticText);

    wxString user = userCtrl ? userCtrl->GetValue() : wxString("Não informado");
    wxString mail = mailCtrl ? mailCtrl->GetValue() : wxString("Não informado");
    wxString terms =
        (termsChk && termsChk->GetValue()) ? wxString("Sim") : wxString("Não");

    if (summary) {
      wxString resumo =
          wxString::Format("Resumo da configuração:\n\n"
                           "Nome de usuário: %s\n"
                           "Email: %s\n"
                           "Termos aceitos: %s\n\n"
                           "Clique em 'Concluir' para finalizar.",
                           user.c_str(), mail.c_str(), terms.c_str());
      summary->SetLabel(resumo);

      summary->Wrap(400);
      c3->Layout();
      p3->Layout();
      if (auto *s = p3->GetSizer()) {
        s->Layout();
      }
    }
    e.Skip();
  }

  bool Run() {
    if (!wiz)
      return false;
    wiz->CentreOnScreen();
    return wiz->RunWizard(p1);
  }

  void Destroy() {
    if (wiz) {
      wiz->Unbind(wxEVT_WIZARD_PAGE_CHANGED, &MyWizard::OnPageChanged, this);
      wiz->Destroy();
      wiz = nullptr;
    }
  }

  ~MyWizard() { Destroy(); }
};

class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxLog::SetActiveTarget(new wxLogStderr());
    wxInitAllImageHandlers();

    auto *xr = wxXmlResource::Get();
    xr->InitAllHandlers();

    if (!xr->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    MyWizard wizard;
    if (!wizard.Create(xr)) {
      return false;
    }

    bool ok = wizard.Run();
    wxLogMessage("Wizard finalizado: %s", ok ? "Sucesso" : "Cancelado");
    wizard.Destroy();

    return false;
  }
};

wxIMPLEMENT_APP(MyApp);
