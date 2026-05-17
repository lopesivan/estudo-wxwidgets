#include "main_frame.h"
#include "image_canvas.h"

#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/xrc/xmlres.h>

MainFrame::MainFrame(const wxString& imagePath)
{
    wxXmlResource::Get()->LoadFrame(this, nullptr, "MainFrame");

    CreateCanvas();
    BindEvents();
    LoadInitialImage(imagePath);

    SetStatusText("Pronto");
    Centre();
}

void MainFrame::CreateCanvas()
{
    auto* container = XRCCTRL(*this, "ID_CANVAS_CONTAINER", wxPanel);

    m_canvas = new ImageCanvas(container);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_canvas, 1, wxEXPAND);
    container->SetSizer(sizer);
    container->Layout();
}

void MainFrame::BindEvents()
{
    Bind(wxEVT_MENU, &MainFrame::OnOpen, this, XRCID("ID_OPEN"));
    Bind(wxEVT_MENU, &MainFrame::OnFitToWindow, this, XRCID("ID_FIT_TO_WINDOW"));
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

void MainFrame::LoadInitialImage(const wxString& imagePath)
{
    if (!imagePath.empty())
    {
        if (m_canvas->LoadPng(imagePath))
        {
            SetTitle("PNG Viewer - " + wxFileName(imagePath).GetFullName());
            SetStatusText("Imagem carregada: " + imagePath);
        }
    }
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog dialog(this,
                        "Escolha uma imagem PNG",
                        wxEmptyString,
                        wxEmptyString,
                        "PNG files (*.png)|*.png",
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL)
        return;

    const wxString path = dialog.GetPath();

    if (m_canvas->LoadPng(path))
    {
        SetStatusText("Imagem carregada: " + path);
        SetTitle("PNG Viewer - " + wxFileName(path).GetFullName());
    }
}

void MainFrame::OnFitToWindow(wxCommandEvent& event)
{
    m_canvas->SetFitToWindow(event.IsChecked());

    if (event.IsChecked())
        SetStatusText("Modo Fit to Window ativado");
    else
        SetStatusText("Imagem em tamanho original");
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(
        "Visualizador simples de PNG usando wxWidgets + XRC.\n\n"
        "A interface vem do arquivo resources/main_frame.xrc.",
        "About PNG Viewer",
        wxOK | wxICON_INFORMATION,
        this);
}

