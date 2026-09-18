#include "MainFrame.hpp"

#include <wx/xrc/xmlres.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

MainFrame::MainFrame()
{
    wxXmlResource::Get()->InitAllHandlers();

    // resources/main.xrc fica ao lado do executável
    wxFileName xrcPath(wxStandardPaths::Get().GetExecutablePath());
    xrcPath.SetFullName("");
    xrcPath.AppendDir("resources");
    xrcPath.SetFullName("main.xrc");

    if (!wxXmlResource::Get()->Load(xrcPath.GetFullPath()))
        wxLogError("Falha ao carregar %s", xrcPath.GetFullPath());

    wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

    // Aumentado o tamanho mínimo da janela
    SetMinClientSize(FromDIP(wxSize(850, 500)));

    m_list = XRCCTRL(*this, "downloadList", wxListCtrl);
    m_categoryList = XRCCTRL(*this, "categoryList", wxListBox);

    if (m_categoryList)
    {
        m_categoryList->SetSelection(0);
    }

    // Larguras ajustadas das colunas para evitar cortes de texto
    if (m_list)
    {
        m_list->InsertColumn(0, "Arquivo",    wxLIST_FORMAT_LEFT,  FromDIP(180));
        m_list->InsertColumn(1, "Tamanho",    wxLIST_FORMAT_RIGHT, FromDIP(90));
        m_list->InsertColumn(2, "Progresso",  wxLIST_FORMAT_RIGHT, FromDIP(90));
        m_list->InsertColumn(3, "Velocidade", wxLIST_FORMAT_RIGHT, FromDIP(110));
        m_list->InsertColumn(4, "Estado",     wxLIST_FORMAT_LEFT,  FromDIP(110));

        PopulateFakeData();
    }

    if (wxStatusBar* status = GetStatusBar())
    {
        status->SetStatusText("3 downloads", 0);
        status->SetStatusText("17.2 MB/s", 1);
    }

    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::PopulateFakeData()
{
    struct Row { wxString file, size, prog, speed, state; };

    const Row rows[] = {
        {"ubuntu.iso", "5.8 GB", "72%",  "12.4 MB/s", "Baixando"},
        {"manual.pdf", "24 MB",  "100%", "--",        "Completo"},
        {"video.mp4",  "850 MB", "21%",  "4.8 MB/s",  "Baixando"},
    };

    for (const auto& r : rows)
    {
        long idx = m_list->InsertItem(m_list->GetItemCount(), r.file);
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
                 "Sobre", wxOK | wxICON_INFORMATION);
}
