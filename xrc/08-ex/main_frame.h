#pragma once
#include <wx/frame.h>

class wxListCtrl;
class wxTextCtrl;
class wxButton;
class wxCommandEvent;

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    // Controles da UI
    wxListCtrl* packageList_{nullptr};
    wxTextCtrl* out_{nullptr};
    wxButton* btnRefresh_{nullptr};
    wxButton* btnUpdate_{nullptr};
    wxButton* btnUpgrade_{nullptr};
    wxButton* btnListUpgradable_{nullptr};

    // Setup
    void CacheControls();
    void SetupLabels();
    void SetupListColumns();
    void BindEvents();

    // === Lógica (portada do seu código) ===
    void LoadInstalledPackages();
    void OnRefresh(wxCommandEvent& e);
    void OnUpdate(wxCommandEvent& e);
    void OnUpgrade(wxCommandEvent& e);
    void OnListUpgradable(wxCommandEvent& e);

    // Helpers (execução de comandos e saída)
    wxString ExecuteCommand(const wxString& command);
    wxString ExecuteCommandWithSudo(const wxString& command);
    void AppendOut(const wxString& s);
};
