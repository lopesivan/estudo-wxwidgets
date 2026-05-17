#include "image_canvas.h"

wxBEGIN_EVENT_TABLE(ImageCanvas, wxPanel)
    EVT_PAINT(ImageCanvas::OnPaint)
wxEND_EVENT_TABLE()

ImageCanvas::ImageCanvas(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxWHITE);
}

bool ImageCanvas::LoadPng(const wxString& filename)
{
    if (!m_image.LoadFile(filename, wxBITMAP_TYPE_PNG))
    {
        wxLogError("Não foi possível carregar a imagem PNG: %s", filename);
        return false;
    }

    m_bitmap = wxBitmap(m_image);

    SetMinSize(wxSize(m_bitmap.GetWidth(), m_bitmap.GetHeight()));
    Refresh();

    return true;
}

void ImageCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.Clear();

    if (!m_bitmap.IsOk())
        return;

    dc.DrawBitmap(m_bitmap, 0, 0, true);
}

