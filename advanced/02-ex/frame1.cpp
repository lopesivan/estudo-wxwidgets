/////////////////////////////////////////////////////////////////////////////
// Name:        frame1.cpp
// Purpose:     
// Author:      Ivan Carlos Da Silva Lopes
// Modified by: 
// Created:     seg 13 out 2025 20:05:53
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

#include "frame1.h"

////@begin XPM images
////@end XPM images


/*
 * frame1 type definition
 */

IMPLEMENT_CLASS( frame1, wxFrame )


/*
 * frame1 event table definition
 */

BEGIN_EVENT_TABLE( frame1, wxFrame )

////@begin frame1 event table entries
    EVT_BUTTON( XRCID("ID_BUTTON"), frame1::OnButtonClick )
    EVT_BUTTON( XRCID("ID_BUTTON1"), frame1::OnButton1Click )
////@end frame1 event table entries

END_EVENT_TABLE()


/*
 * frame1 constructors
 */

frame1::frame1()
{
    Init();
}

frame1::frame1( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * frame1 creator
 */

bool frame1::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin frame1 creation
    SetParent(parent);
    CreateControls();
    Centre();
////@end frame1 creation
    return true;
}


/*
 * frame1 destructor
 */

frame1::~frame1()
{
////@begin frame1 destruction
////@end frame1 destruction
}


/*
 * Member initialisation
 */

void frame1::Init()
{
////@begin frame1 member initialisation
////@end frame1 member initialisation
}


/*
 * Control creation for frame1
 */

void frame1::CreateControls()
{    
////@begin frame1 content construction
    if (!wxXmlResource::Get()->LoadFrame(this, GetParent(), wxT("ID_FRAME1")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
////@end frame1 content construction

    // Create custom windows not generated automatically here.
////@begin frame1 content initialisation
////@end frame1 content initialisation
}


/*
 * Should we show tooltips?
 */

bool frame1::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap frame1::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin frame1 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end frame1 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon frame1::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin frame1 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end frame1 icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void frame1::OnButtonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in frame1.
    // Before editing this code, remove the block markers.
    wxMessageBox(_("kiko 1"));
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in frame1. 
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
 */

void frame1::OnButton1Click( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1 in frame1.
    // Before editing this code, remove the block markers.
    wxMessageBox(_("kiko 2"));
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1 in frame1. 
}

