#include <wx/wx.h>
#include "MainFrame.hpp"

class DownloadManagerApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(DownloadManagerApp);

bool DownloadManagerApp::OnInit()
{
    auto* frame = new MainFrame();
    frame->Show(true);
    return true;
}
