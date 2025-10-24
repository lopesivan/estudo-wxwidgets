#pragma once
#include <wx/wx.h>

class SDLPanel;

class MyFrame final : public wxFrame {
public:
    MyFrame();

    MyFrame(const MyFrame&)            = delete;
    MyFrame& operator=(const MyFrame&) = delete;
    MyFrame(MyFrame&&)                 = delete;
    MyFrame& operator=(MyFrame&&)      = delete;
};

