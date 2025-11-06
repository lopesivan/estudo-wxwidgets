#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    void OnQuit(wxCommandEvent&);
    void OnAbout(wxCommandEvent&);
};

#endif // MYFRAME_H

