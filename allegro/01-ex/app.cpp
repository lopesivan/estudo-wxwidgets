#include <wx/wx.h>

// Implementada em frame.cpp
wxFrame* CreateMainFrame();

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP (MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    auto* frame = CreateMainFrame(); // vindo de frame.cpp
    frame->Show (true);
    return true;
}
