#pragma once

#include <wx/wx.h>

class ImageCanvas;

class MainFrame : public wxFrame {
public:
  explicit MainFrame(const wxString &imagePath);

private:
  void CreateMenus();
  void CreateToolbar();

  void OnOpen(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);

private:
  ImageCanvas *m_canvas{};
};
