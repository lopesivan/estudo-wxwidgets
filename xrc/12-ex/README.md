Quase — o seu código carrega o **wxWizard** certinho, mas o `ui.xrc` **não tem nenhuma página**. Sem páginas, `RunWizard(...)` não consegue rodar e o wizard fica “vazio”. Então você precisa **criar pelo menos uma página**.

Abaixo vai um exemplo 100% funcional.

# Opção A — Páginas no XRC (recomendado)

## ui.xrc

```xml
<?xml version="1.0" encoding="UTF-8"?>
<resource version="2.3.0.1" xmlns="http://www.wxwidgets.org/wxxrc">

  <object class="wxWizard" name="ID_WXWIZARD">
    <style>wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX</style>
    <exstyle>wxWS_EX_BLOCK_EVENTS|wxWIZARD_EX_HELPBUTTON</exstyle>
    <title>Mensagem</title>
  </object>

  <object class="wxWizardPageSimple" name="PAGE_1">
    <object class="wxBoxSizer">
      <orient>wxVERTICAL</orient>
      <object class="sizeritem">
        <flag>wxALL</flag><border>12</border>
        <object class="wxStaticText" name="TXT_P1">
          <label>Bem-vindo! Esta é a página 1.</label>
        </object>
      </object>
    </object>
  </object>

  <object class="wxWizardPageSimple" name="PAGE_2">
    <object class="wxBoxSizer">
      <orient>wxVERTICAL</orient>
      <object class="sizeritem">
        <flag>wxALL</flag><border>12</border>
        <object class="wxStaticText" name="TXT_P2">
          <label>Fim! Esta é a página 2.</label>
        </object>
      </object>
    </object>
  </object>

</resource>
```

## main.cpp

```cpp
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
    auto* wiz = new wxWizard();
    if (!wxXmlResource::Get()->LoadObject(wiz, nullptr, "ID_WXWIZARD", "wxWizard")) {
      wxLogError("Falha ao instanciar ID_WXWIZARD");
      wiz->Destroy();
      return false;
    }

    // Carrega páginas do XRC dando o wizard como parent
    auto* p1 = new wxWizardPageSimple();
    auto* p2 = new wxWizardPageSimple();
    if (!wxXmlResource::Get()->LoadObject(p1, wiz, "PAGE_1", "wxWizardPageSimple") ||
        !wxXmlResource::Get()->LoadObject(p2, wiz, "PAGE_2", "wxWizardPageSimple")) {
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
```

# Opção B — Páginas só no código (sem páginas no XRC)

Se quiser manter o `ui.xrc` com **apenas** o wizard, você pode criar as páginas no código:

```cpp
// depois de LoadObject(wiz, ..., "ID_WXWIZARD", "wxWizard"):
auto* p1 = new wxWizardPageSimple(wiz);
new wxStaticText(p1, wxID_ANY, "Bem-vindo! Página 1.", wxPoint(12,12));

auto* p2 = new wxWizardPageSimple(wiz);
new wxStaticText(p2, wxID_ANY, "Fim! Página 2.", wxPoint(12,12));

wxWizardPageSimple::Chain(p1, p2);
wiz->RunWizard(p1);
```

# Compilação

Use o módulo `xrc`:

```bash
g++ -std=c++17 main.cpp -o app `wx-config --cxxflags --libs std,xrc`
./app
```

Resumindo: o seu `main.cpp` está ok na forma de carregar o wizard; o que faltava eram **páginas** para o `RunWizard`. Escolha A (páginas no XRC) é a mais limpa.
