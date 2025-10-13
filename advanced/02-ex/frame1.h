/////////////////////////////////////////////////////////////////////////////
// Name:        frame1.h
// Purpose:     
// Author:      Ivan Carlos Da Silva Lopes
// Modified by: 
// Created:     seg 13 out 2025 20:05:53
// RCS-ID:      
// Copyright:   UERJ
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FRAME1_H_
#define _FRAME1_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/frame.h"
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
#define ID_FRAME1 10000
#define SYMBOL_FRAME1_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FRAME1_TITLE _("frame1")
#define SYMBOL_FRAME1_IDNAME ID_FRAME1
#define SYMBOL_FRAME1_SIZE wxSize(400, 300)
#define SYMBOL_FRAME1_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * frame1 class declaration
 */

class frame1: public wxFrame
{    
    DECLARE_CLASS( frame1 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    frame1();
    frame1( wxWindow* parent, wxWindowID id = SYMBOL_FRAME1_IDNAME, const wxString& caption = SYMBOL_FRAME1_TITLE, const wxPoint& pos = SYMBOL_FRAME1_POSITION, const wxSize& size = SYMBOL_FRAME1_SIZE, long style = SYMBOL_FRAME1_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FRAME1_IDNAME, const wxString& caption = SYMBOL_FRAME1_TITLE, const wxPoint& pos = SYMBOL_FRAME1_POSITION, const wxSize& size = SYMBOL_FRAME1_SIZE, long style = SYMBOL_FRAME1_STYLE );

    /// Destructor
    ~frame1();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin frame1 event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
    void OnButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
    void OnButton1Click( wxCommandEvent& event );

////@end frame1 event handler declarations

////@begin frame1 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end frame1 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin frame1 member variables
////@end frame1 member variables
};

#endif
    // _FRAME1_H_
