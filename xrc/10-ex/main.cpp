#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wizard.h>
#include <wx/xrc/xh_all.h>
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxXmlResource::Get()->InitAllHandlers();
    if (!wxXmlResource::Get()->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    // Instância vazia do wizard e popula pelo XRC
    auto *wiz = new wxWizard();
    if (!wxXmlResource::Get()->LoadObject(wiz, nullptr, "ID_WXWIZARD",
                                          "wxWizard")) {
      wxLogError("Falha ao instanciar ID_WXWIZARD");
      wiz->Destroy();
      return false;
    }

    // Carrega páginas do XRC dando o wizard como parent
    auto *p1 = new wxWizardPageSimple();
    auto *p2 = new wxWizardPageSimple();
    if (!wxXmlResource::Get()->LoadObject(p1, wiz, "PAGE_1",
                                          "wxWizardPageSimple") ||
        !wxXmlResource::Get()->LoadObject(p2, wiz, "PAGE_2",
                                          "wxWizardPageSimple")) {
      wxLogError("Falha ao carregar páginas do wizard");
      wiz->Destroy();
      return false;
    }

    // Encadeia e executa
    wxWizardPageSimple::Chain(p1, p2);
    wiz->CentreOnScreen();
    wiz->RunWizard(p1);
    wiz->Destroy();
    return false; // fecha o app ao sair do wizard
  }
};

wxIMPLEMENT_APP(MyApp);
