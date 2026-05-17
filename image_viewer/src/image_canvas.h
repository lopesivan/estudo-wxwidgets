#pragma once

#include <wx/wx.h>

class ImageCanvas : public wxPanel
{
  public:
    explicit ImageCanvas(wxWindow* parent);

    bool LoadPng(const wxString& filename);

    void SetFitToWindow(bool enabled);
    bool IsFitToWindow() const;

  private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

  private:
    wxImage  m_image;
    wxBitmap m_bitmap;

    bool m_fitToWindow{false};

    wxDECLARE_EVENT_TABLE();
};
