#include "image_canvas.h"

#include <algorithm>

wxBEGIN_EVENT_TABLE(ImageCanvas, wxPanel)
    EVT_PAINT(ImageCanvas::OnPaint) EVT_SIZE(ImageCanvas::OnSize)
        wxEND_EVENT_TABLE()

            ImageCanvas::ImageCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxWHITE);
}

bool ImageCanvas::LoadPng(const wxString& filename)
{
    if(!m_image.LoadFile(filename, wxBITMAP_TYPE_PNG))
    {
        wxLogError("Não foi possível carregar a imagem PNG: %s",
                   filename);
        return false;
    }

    m_bitmap = wxBitmap(m_image);
    Refresh();

    return true;
}

void ImageCanvas::SetFitToWindow(bool enabled)
{
    m_fitToWindow = enabled;
    Refresh();
}

bool ImageCanvas::IsFitToWindow() const
{
    return m_fitToWindow;
}

void ImageCanvas::OnSize(wxSizeEvent& event)
{
    Refresh();
    event.Skip();
}

void ImageCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.Clear();

    if(!m_image.IsOk())
        return;

    if(!m_fitToWindow)
    {
        dc.DrawBitmap(m_bitmap, 0, 0, true);
        return;
    }

    const wxSize clientSize = GetClientSize();

    const int panelW = clientSize.GetWidth();
    const int panelH = clientSize.GetHeight();

    const int imgW = m_image.GetWidth();
    const int imgH = m_image.GetHeight();

    if(panelW <= 0 || panelH <= 0 || imgW <= 0 || imgH <= 0)
        return;

    const double scaleX = static_cast<double>(panelW) / imgW;
    const double scaleY = static_cast<double>(panelH) / imgH;
    const double scale  = std::min(scaleX, scaleY);

    const int newW = static_cast<int>(imgW * scale);
    const int newH = static_cast<int>(imgH * scale);

    wxImage scaledImage =
        m_image.Scale(newW, newH, wxIMAGE_QUALITY_HIGH);
    wxBitmap scaledBitmap(scaledImage);

    const int x = (panelW - newW) / 2;
    const int y = (panelH - newH) / 2;

    dc.DrawBitmap(scaledBitmap, x, y, true);
}
