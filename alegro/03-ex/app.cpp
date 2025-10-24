#include "app.h"
#include "frame.h"

wxIMPLEMENT_APP (MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    auto* frame = new MyFrame();
    frame->Show (true);
    return true;
}
