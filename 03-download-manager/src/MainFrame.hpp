#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void PopulateFakeData();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxListCtrl* m_list = nullptr;
    wxListBox*  m_categoryList = nullptr;
};
