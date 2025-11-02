#pragma once
#include <wx/wx.h>

class MyFrame1;
class MyFrame2;

class MyApp final : public wxApp {
public:
    bool OnInit() override;

    void ShowFrame1();
    void ShowFrame2();

private:
    MyFrame1* f1_ = nullptr;
    MyFrame2* f2_ = nullptr;
};

wxDECLARE_APP(MyApp); // habilita wxGetApp()

