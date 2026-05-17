#pragma once

#include <wx/wx.h>

class ImageCanvas;

class MainFrame : public wxFrame
{
public:
    explicit MainFrame(const wxString& imagePath);

private:
    void BindEvents();
    void CreateCanvas();
    void LoadInitialImage(const wxString& imagePath);

    void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnFitToWindow(wxCommandEvent& event);

private:
    ImageCanvas* m_canvas {};
};

