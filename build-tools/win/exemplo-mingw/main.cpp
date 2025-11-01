#include <wx/wx.h>
class MyApp : public wxApp {
public:
  bool OnInit() override {
    auto* f = new wxFrame(nullptr, wxID_ANY, "Hello wx");
    f->SetClientSize(320,120); f->Centre(); f->Show();
    return true;
  }
};
wxIMPLEMENT_APP(MyApp);

