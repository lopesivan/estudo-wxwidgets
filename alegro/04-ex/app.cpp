#include "app.h"
#include "frame.h"
#include <wx/xrc/xmlres.h>

wxIMPLEMENT_APP (MyApp);

bool MyApp::OnInit()
{
    // Carrega recursos XRC (XML) — pode ser .xrc, .xrs, ou dentro de um .zip
    wxXmlResource::Get()->InitAllHandlers();
    if (!wxXmlResource::Get()->Load ("resources.xrc"))
    {
        wxLogError ("Falha ao carregar resources.xrc");
        return false;
    }
    auto* frame = wxXmlResource::Get()->LoadFrame (nullptr, "MainFrame");
    // auto* frame = new MyFrame();   // O construtor vai instanciar via XRC
    frame->Show (true);
    return true;
}
