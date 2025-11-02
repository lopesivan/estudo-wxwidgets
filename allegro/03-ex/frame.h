#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>

class MyFrame final : public wxFrame
{
public:
    MyFrame();

    // Evitar cópia/movimentação acidental (a janela pertence ao wxWidgets)
    MyFrame (const MyFrame&)            = delete;
    MyFrame& operator= (const MyFrame&) = delete;
    MyFrame (MyFrame&&)                 = delete;
    MyFrame& operator= (MyFrame&&)      = delete;

private:
    void OnQuit (wxCommandEvent& evt);
    void OnAbout (wxCommandEvent& evt);
};

#endif // MYFRAME_H
