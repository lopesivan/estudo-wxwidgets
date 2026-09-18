#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void PopulateFakeData();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxDataViewListCtrl* m_list = nullptr;
    wxListBox*          m_categoryList = nullptr;
};
