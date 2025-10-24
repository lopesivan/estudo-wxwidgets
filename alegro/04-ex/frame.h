#pragma once
#include <wx/wx.h>

class MyFrame final : public wxFrame {
public:
    MyFrame();

    MyFrame(const MyFrame&)            = delete;
    MyFrame& operator=(const MyFrame&) = delete;
    MyFrame(MyFrame&&)                 = delete;
    MyFrame& operator=(MyFrame&&)      = delete;

private:
    void OnQuit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnHello(wxCommandEvent& evt);

    void BuildAccelerators(); // opcional: além dos atalhos declarados no menu
};

