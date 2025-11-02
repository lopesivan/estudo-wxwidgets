#pragma once
#include <wx/wx.h>

class MyFrame1 final : public wxFrame {
public:
    MyFrame1();

    MyFrame1(const MyFrame1&)            = delete;
    MyFrame1& operator=(const MyFrame1&) = delete;
    MyFrame1(MyFrame1&&)                 = delete;
    MyFrame1& operator=(MyFrame1&&)      = delete;

private:
    void OnGoToF2(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);
};

