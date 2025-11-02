#pragma once
#include <wx/wx.h>

class MyFrame2 final : public wxFrame {
public:
    MyFrame2();

    MyFrame2(const MyFrame2&)            = delete;
    MyFrame2& operator=(const MyFrame2&) = delete;
    MyFrame2(MyFrame2&&)                 = delete;
    MyFrame2& operator=(MyFrame2&&)      = delete;

private:
    void OnBackToF1(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);
};

