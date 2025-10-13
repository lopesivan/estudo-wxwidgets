#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

class MyApp : public wxApp {
public:
  bool OnInit() override {
    wxXmlResource::Get()->InitAllHandlers();

    if (!wxXmlResource::Get()->Load("ui.xrc")) {
      wxLogError("Não foi possível carregar ui.xrc");
      return false;
    }

    wxFrame *frame = wxXmlResource::Get()->LoadFrame(nullptr, "ID_WXFRAME");
    if (!frame) {
      wxLogError("Não foi possível instanciar o frame ID_WXFRAME do XRC.");
      return false;
    }

    frame->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);
