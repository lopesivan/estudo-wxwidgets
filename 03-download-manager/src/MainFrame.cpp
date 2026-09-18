#include "MainFrame.hpp"

#include <wx/xrc/xmlres.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

MainFrame::MainFrame()
{
    wxXmlResource::Get()->InitAllHandlers();

    wxFileName xrcPath(wxStandardPaths::Get().GetExecutablePath());
    xrcPath.SetFullName("");
    xrcPath.AppendDir("resources");
    xrcPath.SetFullName("main.xrc");

    if (!wxXmlResource::Get()->Load(xrcPath.GetFullPath()))
    {
        wxLogError("Falha ao carregar %s", xrcPath.GetFullPath());
    }

    wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

    SetMinClientSize(FromDIP(wxSize(900, 520)));

    m_list = XRCCTRL(*this, "downloadList", wxDataViewListCtrl);
    m_categoryList = XRCCTRL(*this, "categoryList", wxListBox);

    if (m_categoryList)
    {
        m_categoryList->SetMinSize(FromDIP(wxSize(190, -1)));
        m_categoryList->SetSelection(0);
    }

    if (m_list)
    {
        // Define as colunas alinhando perfeitamente o cabeçalho e os dados
        m_list->AppendTextColumn("Arquivo",    wxDATAVIEW_CELL_INERT, FromDIP(200), wxALIGN_LEFT);
        m_list->AppendTextColumn("Tamanho",    wxDATAVIEW_CELL_INERT, FromDIP(110), wxALIGN_RIGHT);
        m_list->AppendProgressColumn("Progresso", wxDATAVIEW_CELL_INERT, FromDIP(110), wxALIGN_CENTER);
        m_list->AppendTextColumn("Velocidade", wxDATAVIEW_CELL_INERT, FromDIP(120), wxALIGN_RIGHT);
        m_list->AppendTextColumn("Estado",     wxDATAVIEW_CELL_INERT, FromDIP(120), wxALIGN_LEFT);

        PopulateFakeData();
    }

    if (wxStatusBar* status = GetStatusBar())
    {
        status->SetStatusText("3 downloads", 0);
        status->SetStatusText("17.2 MB/s", 1);
    }

    Layout();

    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::PopulateFakeData()
{
    wxVector<wxVariant> row;

    // Linha 1: ubuntu.iso
    row.clear();
    row.push_back("ubuntu.iso");
    row.push_back("5.8 GB");
    row.push_back(long(72)); // Valor numérico para a barra de progresso nativa
    row.push_back("12.4 MB/s");
    row.push_back("Baixando");
    m_list->AppendItem(row);

    // Linha 2: manual.pdf
    row.clear();
    row.push_back("manual.pdf");
    row.push_back("24 MB");
    row.push_back(long(100));
    row.push_back("--");
    row.push_back("Completo");
    m_list->AppendItem(row);

    // Linha 3: video.mp4
    row.clear();
    row.push_back("video.mp4");
    row.push_back("850 MB");
    row.push_back(long(21));
    row.push_back("4.8 MB/s");
    row.push_back("Baixando");
    m_list->AppendItem(row);
}

void MainFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent&)
{
    wxMessageBox("Download Manager - protótipo wxWidgets + XRC",
                 "Sobre", wxOK | wxICON_INFORMATION);
}
