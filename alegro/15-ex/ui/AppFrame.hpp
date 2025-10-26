// ui/AppFrame.hpp
#pragma once
#include <wx/wx.h>

#include "core/BackendFactory.hpp"
#include "ui/RenderView.hpp"

namespace ui
{

class AppFrame : public wxFrame
{
  public:
    AppFrame()
        : wxFrame(
              nullptr,
              wxID_ANY,
              "Bridge: wx (Abstraction) + Allegro (Implementor)",
              wxDefaultPosition,
              wxSize(core::DEFAULT_WIDTH, core::DEFAULT_HEIGHT))
    {
        auto backend =
            core::makeBackend(core::RenderBackendType::Allegro);
        auto* view  = new ui::RenderView(this, std::move(backend));
        auto* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(view, 1, wxEXPAND | wxALL, 8);
        SetSizer(sizer);
        Centre();
    }
};

}  // namespace ui
