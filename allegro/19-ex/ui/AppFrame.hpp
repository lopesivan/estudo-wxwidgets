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
        : wxFrame(nullptr,
                  wxID_ANY,
                  "Bridge: wx + Allegro",
                  wxDefaultPosition,
                  wxSize(core::DEFAULT_WIDTH, core::DEFAULT_HEIGHT))
    {
        auto* sizer       = new wxBoxSizer(wxVERTICAL);
        auto* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

        // parent = this (frame)
        auto* btnRefresh        = new wxButton(this,
                                        wxID_ANY,
                                        "Atualizar Lista",
                                        wxDefaultPosition,
                                        wxSize(180, -1));
        auto* btnUpdate         = new wxButton(this,
                                       wxID_ANY,
                                       "APT Update",
                                       wxDefaultPosition,
                                       wxSize(180, -1));
        auto* btnUpgrade        = new wxButton(this,
                                        wxID_ANY,
                                        "APT Upgrade",
                                        wxDefaultPosition,
                                        wxSize(180, -1));
        auto* btnListUpgradable = new wxButton(
            this,
            wxID_ANY,
            wxString::FromUTF8(u8"Pacotes Atualizáveis"));

        buttonSizer->Add(btnRefresh, 0, wxALL, 5);
        buttonSizer->Add(btnUpdate, 0, wxALL, 5);
        buttonSizer->Add(btnUpgrade, 0, wxALL, 5);
        buttonSizer->Add(btnListUpgradable, 0, wxALL, 5);

        auto backend =
            core::makeBackend(core::RenderBackendType::Allegro);
        auto* view = new ui::RenderView(
            this, std::move(backend));  // parent = frame

        sizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 5);
        sizer->Add(view, 1, wxEXPAND | wxALL, 8);
        SetSizerAndFit(sizer);
        Centre();
    }
};
}  // namespace ui
