#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>

class AptManagerFrame : public wxFrame
{
public:
    AptManagerFrame();

private:
    // Widgets
    wxListCtrl* packageList{};
    wxTextCtrl* outputText{};
    wxButton* btnRefresh{};
    wxButton* btnUpdate{};
    wxButton* btnUpgrade{};
    wxButton* btnListUpgradable{};

    // UI
    void CreateControls();

    // Ações
    void LoadInstalledPackages();
    void OnRefresh(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);
    void OnUpgrade(wxCommandEvent& event);
    void OnListUpgradable(wxCommandEvent& event);

    // Execução de comandos
    wxString ExecuteCommand(const wxString& command);
    wxString ExecuteCommandWithSudo(const wxString& command);
};
