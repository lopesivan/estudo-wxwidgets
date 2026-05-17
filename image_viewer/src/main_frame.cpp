#include "main_frame.h"
#include "image_canvas.h"

MainFrame::MainFrame(const wxString& imagePath)
    : wxFrame(nullptr, wxID_ANY, "PNG Viewer", wxDefaultPosition, wxSize(800, 600))
{
    m_canvas = new ImageCanvas(this);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_canvas, 1, wxEXPAND);
    SetSizer(sizer);

    m_canvas->LoadPng(imagePath);

    Centre();
}
