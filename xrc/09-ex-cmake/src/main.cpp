#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <wx/xrc/xh_all.h>
#include <wx/xrc/xmlres.h>

#include "main_frame.h"

class MyApp : public wxApp {
public:
    bool OnInit() override {
        wxXmlResource::Get()->InitAllHandlers();

        if (!wxXmlResource::Get()->Load("ui.xrc")) {
            wxMessageBox("Falha ao carregar ui.xrc", "Erro", wxOK | wxICON_ERROR);
            return false;
        }

        auto* frame = new MainFrame();   // constrói a janela a partir do XRC
        frame->Show(true);               // MOSTRA AQUI (não no construtor)
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

