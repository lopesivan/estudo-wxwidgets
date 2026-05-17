#pragma once

#include <wx/wx.h>

class ImageCanvas : public wxPanel
{
public:
    explicit ImageCanvas(wxWindow* parent);

    bool LoadPng(const wxString& filename);

private:
    void OnPaint(wxPaintEvent& event);

private:
    wxImage  m_image;
    wxBitmap m_bitmap;

    wxDECLARE_EVENT_TABLE();
};

