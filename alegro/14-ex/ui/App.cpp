// ui/App.cpp
#include <wx/wx.h>
#include "ui/AppFrame.hpp"

class MyApp : public wxApp {
public:
    bool OnInit() override {
        auto* f = new ui::AppFrame();
        f->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);

