#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wizard.h>
#include <wx/xrc/xh_all.h> // garante handlers, inclusive wizard
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxXmlResource::Get()->InitAllHandlers();

    if (!wxXmlResource::Get()->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    // 1) Cria a instância vazia do wizard
    wxWizard *wiz = new wxWizard(); // construtor vazio

    // 2) Popula a instância a partir do XRC (class deve ser "wxWizard")
    if (!wxXmlResource::Get()->LoadObject(wiz, nullptr, "ID_WXWIZARD",
                                          "wxWizard")) {
      wxLogError(
          "Não foi possível instanciar 'ID_WXWIZARD' como wxWizard (XRC).");
      wiz->Destroy();
      return false;
    }

    // 3) Tentar descobrir uma primeira página automaticamente
    wxWizardPage *firstPage = nullptr;
    for (wxWindowList::compatibility_iterator node =
             wiz->GetChildren().GetFirst();
         node; node = node->GetNext()) {
      if (auto *page = wxDynamicCast(node->GetData(), wxWizardPage)) {
        firstPage = page;
        break;
      }
    }

    // 4) Executa o wizard do jeito certo, se achou página inicial
    if (firstPage) {
      // RunWizard exibe modal até concluir/cancelar
      wiz->RunWizard(firstPage);
      wiz->Destroy();
    } else {
      // fallback: mostra não-modal (útil durante montagem do XRC)
      wiz->Show(true);
    }
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);
