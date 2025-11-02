// ui/RenderView.hpp
#pragma once
#include <wx/wx.h>

#include <cstring>
#include <memory>
#include <vector>

#include "core/IRenderBackend.hpp"
#include "core/RenderModel.hpp"

namespace ui
{

class RenderView : public wxPanel
{
  public:
    explicit RenderView(
        wxWindow*                             parent,
        std::unique_ptr<core::IRenderBackend> backend)
        : wxPanel(
              parent,
              wxID_ANY,
              wxDefaultPosition,
              wxSize(core::DEFAULT_WIDTH, core::DEFAULT_HEIGHT)),
          backend_(std::move(backend))
    {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &RenderView::OnPaint, this);
        Bind(wxEVT_ERASE_BACKGROUND,
             [](wxEraseEvent&) { /* evita clear do wx */ });
        Bind(wxEVT_SIZE, &RenderView::OnResize, this);
        Bind(wxEVT_MOTION, &RenderView::OnMouseMove, this);
        Bind(wxEVT_CHAR_HOOK, &RenderView::OnCharHook, this);

        model_.width  = GetSize().x;
        model_.height = GetSize().y;

        backend_->init(
            /*native_handle*/ nullptr, model_.width, model_.height);
    }

    ~RenderView() override
    {
        if(backend_)
            backend_->shutdown();
    }

  private:
    std::unique_ptr<core::IRenderBackend> backend_;
    core::RenderModel                     model_;
    std::vector<unsigned char>
        frameRGB_;  // buffer temporário para wxImage

    void OnResize(wxSizeEvent& e)
    {
        const auto sz = e.GetSize();
        model_.width  = std::max(1, sz.GetWidth());
        model_.height = std::max(1, sz.GetHeight());
        backend_->resize(model_.width, model_.height);
        Refresh(false);
        e.Skip();
    }

    void OnMouseMove(wxMouseEvent& e)
    {
        model_.mouse_x_px = e.GetX();
        model_.mouse_y_px = e.GetY();
        Refresh(false);
    }

    void OnCharHook(wxKeyEvent& e)
    {
        const int code = e.GetKeyCode();
        if(code == WXK_ESCAPE)
        {
            if(auto* tlw = wxGetTopLevelParent(this))
                tlw->Close(false);
            return;
        }
        else if(code == WXK_SPACE)
        {
            if(model_.points_stored < 2)
            {
                core::armazenar_posicao_mouse(model_.mouse_x_px,
                                              model_.mouse_y_px,
                                              model_.width,
                                              model_.height,
                                              model_.pos_km_x,
                                              model_.pos_km_y,
                                              model_.points_stored);
                model_.points_stored++;
            }
            else
            {
                model_.points_stored = 0;
                model_.resetar_posicoes();
            }
            Refresh(false);
            return;
        }
        e.Skip();
    }

    void OnPaint(wxPaintEvent&)
    {
        wxPaintDC dc(this);
        if(!backend_)
            return;

        backend_->render(model_);

        int w = 0, h = 0;
        if(!backend_->getFrameRGB(frameRGB_, w, h))
            return;

        wxImage img(w, h);
        std::memcpy(
            img.GetData(), frameRGB_.data(), frameRGB_.size());
        wxBitmap bmp(img);
        dc.DrawBitmap(bmp, 0, 0, false);
    }
};

}  // namespace ui
