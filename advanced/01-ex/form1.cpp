/////////////////////////////////////////////////////////////////////////////
// Name:        form1.cpp
// Purpose:     
// Author:      Ivan Carlos Da Silva Lopes
// Modified by: 
// Created:     dom 12 out 2025 13:19:25
// RCS-ID:      
// Copyright:   UERJ
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
////@end includes

#include "form1.h"

////@begin XPM images
////@end XPM images


/*
 * Form1 type definition
 */

IMPLEMENT_CLASS( Form1, wxFrame )


/*
 * Form1 event table definition
 */

BEGIN_EVENT_TABLE( Form1, wxFrame )

////@begin Form1 event table entries
////@end Form1 event table entries

END_EVENT_TABLE()


/*
 * Form1 constructors
 */

Form1::Form1()
{
    Init();
}

Form1::Form1( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * Form1 creator
 */

bool Form1::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Form1 creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end Form1 creation
    return true;
}


/*
 * Form1 destructor
 */

Form1::~Form1()
{
////@begin Form1 destruction
////@end Form1 destruction
}


/*
 * Member initialisation
 */

void Form1::Init()
{
////@begin Form1 member initialisation
////@end Form1 member initialisation
}


/*
 * Control creation for Form1
 */

void Form1::CreateControls()
{    
////@begin Form1 content construction
    Form1* itemFrame1 = this;

    wxPanel* itemPanel1 = new wxPanel( itemFrame1, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemPanel1->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton( itemPanel1, ID_BUTTON, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemPanel1, ID_BUTTON1, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemPanel1, ID_BUTTON2, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end Form1 content construction
}


/*
 * Should we show tooltips?
 */

bool Form1::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap Form1::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Form1 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Form1 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon Form1::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Form1 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Form1 icon retrieval
}
