#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <cstdlib>
#include <string>

static void adb_key(int key)
{
    std::string cmd = "adb shell input keyevent " + std::to_string(key);
    std::system(cmd.c_str());
}

class MainFrame : public wxFrame
{
public:
    MainFrame()
    {
        wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(19); }, wxXmlResource::GetXRCID("btnUp"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(20); }, wxXmlResource::GetXRCID("btnDown"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(21); }, wxXmlResource::GetXRCID("btnLeft"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(22); }, wxXmlResource::GetXRCID("btnRight"));
        Bind(wxEVT_BUTTON, [](wxCommandEvent&) { adb_key(23); }, wxXmlResource::GetXRCID("btnOk"));
    }
};

class App : public wxApp
{
public:
    bool OnInit() override
    {
        wxXmlResource::Get()->InitAllHandlers();
        wxXmlResource::Get()->Load("main.xrc");

        auto* frame = new MainFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);
