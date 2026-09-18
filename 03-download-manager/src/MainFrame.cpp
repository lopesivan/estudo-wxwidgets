#include "MainFrame.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

MainFrame::MainFrame()
{
    wxXmlResource::Get()->InitAllHandlers();

    wxFileName xrcPath(wxStandardPaths::Get().GetExecutablePath());
    xrcPath.SetFullName("");
    xrcPath.AppendDir("resources");
    xrcPath.SetFullName("main.xrc");

    if(!wxXmlResource::Get()->Load(xrcPath.GetFullPath()))
        wxLogError("Falha ao carregar %s", xrcPath.GetFullPath());

    wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

    SetMinClientSize(FromDIP(wxSize(900, 520)));

    m_list         = XRCCTRL(*this, "downloadList", wxListCtrl);
    m_categoryList = XRCCTRL(*this, "categoryList", wxListBox);

    if(m_categoryList)
    {
        // Define o tamanho mínimo em DPI escalado para evitar corte
        // dos textos
        m_categoryList->SetMinSize(FromDIP(wxSize(200, -1)));
        m_categoryList->SetSelection(0);
    }

    if(m_list)
    {
        // Mantenha Tamanho, Progresso e Velocidade à direita
        // (cabeçalho e dados acompanham)
        m_list->InsertColumn(
            0, "Arquivo", wxLIST_FORMAT_LEFT, FromDIP(200));
        m_list->InsertColumn(
            1, "Tamanho", wxLIST_FORMAT_RIGHT, FromDIP(110));
        m_list->InsertColumn(
            2, "Progresso", wxLIST_FORMAT_RIGHT, FromDIP(110));
        m_list->InsertColumn(
            3, "Velocidade", wxLIST_FORMAT_RIGHT, FromDIP(130));
        m_list->InsertColumn(
            4, "Estado", wxLIST_FORMAT_LEFT, FromDIP(120));

        PopulateFakeData();

        // DICA EXTRA: Auto-ajusta as colunas considerando a largura
        // do título E do conteúdo
        for(int i = 0; i < m_list->GetColumnCount(); ++i)
        {
            // Garante espaço mínimo para o cabeçalho não cortar
            m_list->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
        }
    }

    if(wxStatusBar* status = GetStatusBar())
    {
        status->SetStatusText("3 downloads", 0);
        status->SetStatusText("17.2 MB/s", 1);
    }

    // Atualiza o layout do frame para aplicar os tamanhos mínimos
    // definidos
    Layout();

    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::PopulateFakeData()
{
    struct Row
    {
        wxString file, size, prog, speed, state;
    };

    const Row rows[] = {
        {"ubuntu.iso", "5.8 GB", "72%", "12.4 MB/s", "Baixando"},
        {"manual.pdf", "24 MB", "100%", "--", "Completo"},
        {"video.mp4", "850 MB", "21%", "4.8 MB/s", "Baixando"},
    };

    for(const auto& r : rows)
    {
        long idx =
            m_list->InsertItem(m_list->GetItemCount(), r.file);
        m_list->SetItem(idx, 1, r.size);
        m_list->SetItem(idx, 2, r.prog);
        m_list->SetItem(idx, 3, r.speed);
        m_list->SetItem(idx, 4, r.state);
    }
}

void MainFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent&)
{
    wxMessageBox("Download Manager - prototipo wxWidgets + XRC",
                 "Sobre",
                 wxOK | wxICON_INFORMATION);
}
