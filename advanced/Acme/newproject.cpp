/////////////////////////////////////////////////////////////////////////////
// Name:        newproject.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     
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
////@end includes

#include "newproject.h"

////@begin XPM images
#include "image0001.xpm"
////@end XPM images

/*!
 * AcmeNewProject type definition
 */

IMPLEMENT_CLASS( AcmeNewProject, wxDialog )

/*!
 * AcmeNewProject event table definition
 */

BEGIN_EVENT_TABLE( AcmeNewProject, wxDialog )

////@begin AcmeNewProject event table entries
////@end AcmeNewProject event table entries

END_EVENT_TABLE()

/*!
 * AcmeNewProject constructor
 */

AcmeNewProject::AcmeNewProject( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AcmeNewProject member initialisation
////@end AcmeNewProject member initialisation

////@begin AcmeNewProject creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AcmeNewProject creation
}

/*!
 * Control creation for AcmeNewProject
 */

void AcmeNewProject::CreateControls()
{    
////@begin AcmeNewProject content construction
    AcmeNewProject* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Please enter details\nfor the new project.\n\nDescription is optional."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBitmap* itemStaticBitmap7 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemDialog1->GetBitmapResource(wxT("image0001.png")), wxDefaultPosition, wxSize(114, 76), 0 );
    itemBoxSizer4->Add(itemStaticBitmap7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine8 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer3->Add(itemStaticLine8, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Title:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALL, 5);

    wxTextCtrl* itemTextCtrl10 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(260, -1), 0 );
    itemBoxSizer3->Add(itemTextCtrl10, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Description:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALL, 5);

    wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize(-1, 80), wxTE_MULTILINE );
    itemBoxSizer3->Add(itemTextCtrl12, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer13, 0, wxGROW, 5);

    wxCheckBox* itemCheckBox14 = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("&Cool"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox14->SetValue(false);
    itemBoxSizer13->Add(itemCheckBox14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer13->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("&Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice17Strings;
    itemChoice17Strings.Add(_("Apple"));
    itemChoice17Strings.Add(_("Banana"));
    itemChoice17Strings.Add(_("Orange"));
    wxChoice* itemChoice17 = new wxChoice( itemDialog1, ID_CHOICE, wxDefaultPosition, wxSize(100, -1), itemChoice17Strings, 0 );
    itemChoice17->SetStringSelection(_("Apple"));
    itemBoxSizer13->Add(itemChoice17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine18 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer3->Add(itemStaticLine18, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer19, 0, wxGROW, 5);

    itemBoxSizer19->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton21 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton22 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton23 = new wxButton( itemDialog1, wxID_HELP, _("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemButton23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end AcmeNewProject content construction
}

/*!
 * Should we show tooltips?
 */

bool AcmeNewProject::ShowToolTips()
{
  return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap AcmeNewProject::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AcmeNewProject bitmap retrieval
    wxUnusedVar(name);
    if (name == wxT("image0001.png"))
    {
        wxBitmap bitmap(image0001_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end AcmeNewProject bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AcmeNewProject::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AcmeNewProject icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AcmeNewProject icon retrieval
}
