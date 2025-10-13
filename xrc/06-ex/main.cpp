#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

// Macro para pegar controles por ID do XRC
#ifndef XRCCTRL
#define XRCCTRL(w, id, type)                                                   \
    ((type*)wxWindow::FindWindowById(wxXmlResource::GetXRCID(id), w))
#endif

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        // habilita handlers padrão do XRC (frame, dialog, controls etc.)
        wxXmlResource::Get()->InitAllHandlers();

        // carrega o arquivo XRC do disco
        if (!wxXmlResource::Get()->Load("ui.xrc"))
        {
            wxLogError("Não foi possível carregar ui.xrc");
            return false;
        }

        // instancia o frame definido no XRC (name="ID_WXFRAME")
        wxFrame* frame = wxXmlResource::Get()->LoadFrame(nullptr, "ID_WXFRAME");
        if (!frame)
        {
            wxLogError("Falha ao instanciar ID_WXFRAME a partir do XRC");
            return false;
        }

        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
