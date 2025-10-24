#include "app.h"
#include "frame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    if ( !wxApp::OnInit() ) return false;
    auto* f = new MyFrame();
    f->Show(true);
    return true;
}

