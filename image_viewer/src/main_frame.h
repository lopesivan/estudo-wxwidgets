#pragma once

#include <wx/wx.h>

class ImageCanvas;

class MainFrame : public wxFrame
{
public:
    explicit MainFrame(const wxString& imagePath);

private:
    ImageCanvas* m_canvas;
};

