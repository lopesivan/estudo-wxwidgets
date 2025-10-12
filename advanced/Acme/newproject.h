/////////////////////////////////////////////////////////////////////////////
// Name:        newproject.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NEWPROJECT_H_
#define _NEWPROJECT_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_NEW_PROJECT_DIALOG 10011
#define ID_TEXTCTRL1 10012
#define ID_TEXTCTRL_DESCRIPTION 10013
#define ID_CHECKBOX 10014
#define ID_CHOICE 10015
#define SYMBOL_ACMENEWPROJECT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ACMENEWPROJECT_TITLE _("New Project Dialog")
#define SYMBOL_ACMENEWPROJECT_IDNAME ID_NEW_PROJECT_DIALOG
#define SYMBOL_ACMENEWPROJECT_SIZE wxSize(400, 300)
#define SYMBOL_ACMENEWPROJECT_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * AcmeNewProject class declaration
 */

class AcmeNewProject: public wxDialog
{    
    DECLARE_CLASS( AcmeNewProject )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    AcmeNewProject( wxWindow* parent, wxWindowID id = -1, const wxString& caption = _("New Project Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU );

    /// Creates the controls and sizers
    void CreateControls();

////@begin AcmeNewProject event handler declarations

////@end AcmeNewProject event handler declarations

////@begin AcmeNewProject member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AcmeNewProject member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AcmeNewProject member variables
////@end AcmeNewProject member variables
};

#endif
    // _NEWPROJECT_H_
