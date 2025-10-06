#pragma once
// ============================================================================
// Includes necessários para wxWidgets
// ============================================================================
#include <wx/listctrl.h>
#include <wx/msgdlg.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/wx.h>

// ============================================================================
// CLASSE PRINCIPAL: AptManagerFrame
// Esta classe representa a janela principal do aplicativo
// ============================================================================
class AptManagerFrame : public wxFrame
{
public:
    // Construtor: cria a janela com título e tamanho inicial
    AptManagerFrame();

private:
    // ------------------------------------------------------------------------
    // COMPONENTES DA INTERFACE (Widgets)
    // ------------------------------------------------------------------------
    wxListCtrl* packageList;      // Tabela que mostra os pacotes instalados
    wxTextCtrl* outputText;       // Área de texto para saída dos comandos
    wxButton* btnRefresh;         // Botão "Atualizar Lista"
    wxButton* btnUpdate;          // Botão "APT Update"
    wxButton* btnUpgrade;         // Botão "APT Upgrade"
    wxButton* btnListUpgradable;  // Botão "Pacotes Atualizáveis"

    // ------------------------------------------------------------------------
    // MÉTODO: CreateControls
    // Cria e organiza todos os componentes da interface gráfica
    // ------------------------------------------------------------------------
    void CreateControls();

    void LoadInstalledPackages();

    // ========================================================================
    // HANDLERS DE EVENTOS (Funções chamadas quando botões são clicados)
    // ========================================================================
    void OnRefresh(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);
    void OnUpgrade(wxCommandEvent& event);
    void OnListUpgradable(wxCommandEvent& event);

    // ========================================================================
    // FUNÇÕES AUXILIARES (Executam comandos no sistema)
    // ========================================================================
    wxString ExecuteCommand(const wxString& command);
    wxString ExecuteCommandWithSudo(const wxString& command);
};
