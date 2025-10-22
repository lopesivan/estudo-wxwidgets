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
    wxLog::SetActiveTarget(new wxLogStderr());
    wxInitAllImageHandlers();

    auto *xr = wxXmlResource::Get();
    xr->InitAllHandlers();

    if (!xr->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    // 1) Instancia o wizard a partir do XRC
    auto *wiz = new wxWizard();
    if (!xr->LoadObject(wiz, nullptr, "ID_WXWIZARD", "wxWizard")) {
      wxLogError("Falha ao carregar ID_WXWIZARD");
      wiz->Destroy();
      return false;
    }

    // 2) Cria páginas simples no código
    auto *p1 = new wxWizardPageSimple(wiz);
    auto *p2 = new wxWizardPageSimple(wiz);
    auto *p3 = new wxWizardPageSimple(wiz);

    // 3) Carrega o conteúdo XRC de cada página como wxPanel
    wxPanel *c1 = xr->LoadPanel(p1, "PAGE_1");
    wxPanel *c2 = xr->LoadPanel(p2, "PAGE_2");
    wxPanel *c3 = xr->LoadPanel(p3, "PAGE_3");

    if (!(c1 && c2 && c3)) {
      wxLogError("Falha ao carregar PAGE_1, PAGE_2 ou PAGE_3 (como painéis).");
      wiz->Destroy();
      return false;
    }

    // Anexa os painéis às páginas
    auto attach_panel = [](wxWizardPageSimple *page, wxPanel *panel) {
      if (!panel)
        return;
      auto *s = new wxBoxSizer(wxVERTICAL);
      s->Add(panel, 1, wxEXPAND);
      page->SetSizerAndFit(s);
    };

    attach_panel(p1, c1);
    attach_panel(p2, c2);
    attach_panel(p3, c3);

    // 4) Encadeia as páginas
    wxWizardPageSimple::Chain(p1, p2);
    wxWizardPageSimple::Chain(p2, p3);

    // 5) Atualiza o resumo quando entrar na PAGE_3
    // CORREÇÃO: Usar os painéis como contexto de busca, não o wizard
    wiz->Bind(wxEVT_WIZARD_PAGE_CHANGED, [c1, c2, c3, p3](wxWizardEvent &e) {
      if (e.GetPage() != p3) {
        e.Skip();
        return;
      }

      // Busca os controles nos painéis corretos
      auto *userCtrl =
          wxDynamicCast(c2->FindWindow(XRCID("TEXT_USERNAME")), wxTextCtrl);
      auto *mailCtrl =
          wxDynamicCast(c2->FindWindow(XRCID("TEXT_EMAIL")), wxTextCtrl);
      auto *termsChk =
          wxDynamicCast(c1->FindWindow(XRCID("CHECK_TERMS")), wxCheckBox);
      auto *summary =
          wxDynamicCast(c3->FindWindow(XRCID("TEXT_SUMMARY")), wxStaticText);

      const wxString user = userCtrl ? userCtrl->GetValue() : "Não informado";
      const wxString mail = mailCtrl ? mailCtrl->GetValue() : "Não informado";
      const wxString terms = (termsChk && termsChk->GetValue()) ? "Sim" : "Não";

      if (summary) {
        summary->SetLabel(
            wxString::Format("Resumo da configuração:\n\n"
                             "Nome de usuário: %s\n"
                             "Email: %s\n"
                             "Termos aceitos: %s\n\n"
                             "Clique em 'Concluir' para finalizar.",
                             user, mail, terms));

        // Força o layout do painel para ajustar o texto
        c3->Layout();
        if (auto *s = p3->GetSizer()) {
          s->Layout();
        }
      }
      e.Skip();
    });

    // 6) Roda o wizard
    wiz->CentreOnScreen();
    const bool ok = wiz->RunWizard(p1);
    wxLogMessage("Wizard finalizado: %s", ok ? "Sucesso" : "Cancelado");
    wiz->Destroy();

    return false;
  }
};

wxIMPLEMENT_APP(MyApp);
