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
        // 1) painel raiz do frame
        auto* panel = new wxPanel(this);

        // 2) sizers do painel
        auto* panelSizer  = new wxBoxSizer(wxVERTICAL);
        auto* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

        // 3) botões: PARENT = panel
        auto* btnRefresh        = new wxButton(panel,
                                        wxID_ANY,
                                        "Atualizar Lista",
                                        wxDefaultPosition,
                                        wxSize(180, -1));
        auto* btnUpdate         = new wxButton(panel,
                                       wxID_ANY,
                                       "APT Update",
                                       wxDefaultPosition,
                                       wxSize(180, -1));
        auto* btnUpgrade        = new wxButton(panel,
                                        wxID_ANY,
                                        "APT Upgrade",
                                        wxDefaultPosition,
                                        wxSize(180, -1));
        auto* btnListUpgradable = new wxButton(
            panel,
            wxID_ANY,
            wxString::FromUTF8(u8"Pacotes Atualizáveis"));

        buttonSizer->Add(btnRefresh, 0, wxALL, 5);
        buttonSizer->Add(btnUpdate, 0, wxALL, 5);
        buttonSizer->Add(btnUpgrade, 0, wxALL, 5);
        buttonSizer->Add(btnListUpgradable, 0, wxALL, 5);

        // 4) RenderView: PARENT = panel (não o frame)
        auto backend =
            core::makeBackend(core::RenderBackendType::Allegro);
        auto* view = new ui::RenderView(panel, std::move(backend));

        // 5) montar sizers
        panelSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 5);
        panelSizer->Add(view, 1, wxEXPAND | wxALL, 8);
        panel->SetSizer(panelSizer);

        // 6) sizer do frame contém apenas o painel
        auto* frameSizer = new wxBoxSizer(wxVERTICAL);
        frameSizer->Add(panel, 1, wxEXPAND);
        SetSizerAndFit(frameSizer);

        Centre();
    }
};

}  // namespace ui
