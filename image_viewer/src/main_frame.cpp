#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <wx/filename.h>

#include "image_canvas.h"
#include "main_frame.h"

enum
{
    ID_Open = wxID_HIGHEST + 1,
    ID_FitToWindow
};

MainFrame::MainFrame(const wxString& imagePath)
    : wxFrame(nullptr,
              wxID_ANY,
              "PNG Viewer",
              wxDefaultPosition,
              wxSize(900, 700))
{
    CreateMenus();
    CreateToolbar();
    CreateStatusBar();

    m_canvas = new ImageCanvas(this);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_canvas, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);

    if(!imagePath.empty())
        m_canvas->LoadPng(imagePath);

    SetStatusText("Pronto");

    Centre();
}

void MainFrame::CreateMenus()
{
    auto* fileMenu = new wxMenu;

    fileMenu->Append(
        ID_Open, "&Open...\tCtrl+O", "Carregar imagem PNG");

    fileMenu->AppendSeparator();

    fileMenu->AppendCheckItem(ID_FitToWindow,
                              "&Fit to Window\tCtrl+F",
                              "Reescalar imagem conforme a janela");
    fileMenu->Append(
        wxID_EXIT, "E&xit\tCtrl+Q", "Sair do programa");

    auto* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About", "Sobre o programa");

    auto* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, ID_Open);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& event)
        {
            m_canvas->SetFitToWindow(event.IsChecked());

            if(event.IsChecked())
                SetStatusText("Modo Fit to Window ativado");
            else
                SetStatusText("Imagem em tamanho original");
        },
        ID_FitToWindow);
}

void MainFrame::CreateToolbar()
{
    auto* toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);

    toolbar->AddTool(
        ID_Open,
        "Open",
        wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),
        "Carregar imagem PNG");

    toolbar->AddSeparator();

    // toolbar->AddTool(
    //     wxID_EXIT,
    //     "Exit",
    //     wxArtProvider::GetBitmap(wxART_QUIT, wxART_TOOLBAR),
    //     "Sair");

    toolbar->AddCheckTool(
        ID_FitToWindow,
        "Fit",
        wxArtProvider::GetBitmap(wxART_FIND, wxART_TOOLBAR),
        wxNullBitmap,
        "Reescalar imagem conforme a janela");

    toolbar->Realize();
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog dialog(this,
                        "Escolha uma imagem PNG",
                        wxEmptyString,
                        wxEmptyString,
                        "PNG files (*.png)|*.png",
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(dialog.ShowModal() == wxID_CANCEL)
        return;

    const wxString path = dialog.GetPath();

    if(m_canvas->LoadPng(path))
    {
        SetStatusText("Imagem carregada: " + path);
        SetTitle("PNG Viewer - " + wxFileName(path).GetFullName());
    }
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(
        "Visualizador simples de PNG usando wxWidgets.\n\n"
        "Baseado na ideia do sample image/canvas do wxWidgets.",
        "About PNG Viewer",
        wxOK | wxICON_INFORMATION,
        this);
}
