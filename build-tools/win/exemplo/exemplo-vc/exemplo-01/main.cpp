#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        auto* frame = new wxFrame(nullptr, wxID_ANY, _("wxWidgets + CMake (Release DLL)"));
        auto* panel = new wxPanel(frame);
        auto* sizer = new wxBoxSizer(wxVERTICAL);
        auto* label = new wxStaticText(panel, wxID_ANY, _("Olá de wxWidgets 3.3.1!"));
        sizer->Add(label, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 20);
        panel->SetSizerAndFit(sizer);
        frame->SetClientSize(panel->GetBestSize());
        frame->Centre();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

