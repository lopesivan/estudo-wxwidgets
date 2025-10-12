/////////////////////////////////////////////////////////////////////////////
// Name:        notebkdlg.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/21/04 17:32:17
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "notebkdlg.h"

////@begin XPM images
////@end XPM images

/*!
 * MyDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MyDialog, wxDialog )

/*!
 * MyDialog event table definition
 */

BEGIN_EVENT_TABLE( MyDialog, wxDialog )

////@begin MyDialog event table entries
////@end MyDialog event table entries

END_EVENT_TABLE()

/*!
 * MyDialog constructors
 */

MyDialog::MyDialog( )
{
}

MyDialog::MyDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * MyDialog creator
 */

bool MyDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MyDialog member initialisation
////@end MyDialog member initialisation

////@begin MyDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end MyDialog creation
    return TRUE;
}

/*!
 * Control creation for MyDialog
 */

void MyDialog::CreateControls()
{    
////@begin MyDialog content construction
    MyDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook* itemNotebook3 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_PANEL, wxDefaultPosition, wxSize(100, 80), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton7 = new wxButton( itemPanel4, ID_BUTTON, _("&Apple"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemPanel4, ID_BUTTON1, _("&Pear"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemPanel4, ID_BUTTON2, _("&Orange"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel4, wxID_STATIC, _("An edit field:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText10, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxTextCtrl* itemTextCtrl11 = new wxTextCtrl( itemPanel4, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(300, 200), wxTE_MULTILINE|wxTE_RICH );
    itemBoxSizer5->Add(itemTextCtrl11, 1, wxGROW|wxALL, 5);

    itemNotebook3->AddPage(itemPanel4, _("Page 1"));

    wxPanel* itemPanel12 = new wxPanel( itemNotebook3, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemPanel12->SetSizer(itemBoxSizer13);

    wxSplitterWindow* itemSplitterWindow14 = new wxSplitterWindow( itemPanel12, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow14->SetMinimumPaneSize(0);

    wxTreeCtrl* itemTreeCtrl15 = new wxTreeCtrl( itemSplitterWindow14, ID_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_SINGLE|wxNO_BORDER );

    wxTextCtrl* itemTextCtrl16 = new wxTextCtrl( itemSplitterWindow14, ID_TEXTCTRL1, _("This is the second window inside a splitter window."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH|wxNO_BORDER );

    itemSplitterWindow14->SplitVertically(itemTreeCtrl15, itemTextCtrl16, 100);
    itemBoxSizer13->Add(itemSplitterWindow14, 1, wxGROW|wxALL, 5);

    itemNotebook3->AddPage(itemPanel12, _("Page 2"));

    itemBoxSizer2->Add(itemNotebook3, 1, wxGROW|wxALL, 5);

////@end MyDialog content construction
}

/*!
 * Should we show tooltips?
 */

bool MyDialog::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap MyDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MyDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MyDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon MyDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MyDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MyDialog icon retrieval
}
