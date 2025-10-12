/////////////////////////////////////////////////////////////////////////////
// Name:        form1.h
// Purpose:     
// Author:      Ivan Carlos Da Silva Lopes
// Modified by: 
// Created:     dom 12 out 2025 13:19:25
// RCS-ID:      
// Copyright:   UERJ
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FORM1_H_
#define _FORM1_H_


/*!
 * Includes
 */

////@begin includes
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
#define ID_FORM1 10000
#define ID_PANEL 10001
#define ID_BUTTON 10002
#define ID_BUTTON1 10003
#define ID_BUTTON2 10004
#define SYMBOL_FORM1_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FORM1_TITLE _("Form1")
#define SYMBOL_FORM1_IDNAME ID_FORM1
#define SYMBOL_FORM1_SIZE wxSize(400, 300)
#define SYMBOL_FORM1_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * Form1 class declaration
 */

class Form1: public wxFrame
{    
    DECLARE_CLASS( Form1 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Form1();
    Form1( wxWindow* parent, wxWindowID id = SYMBOL_FORM1_IDNAME, const wxString& caption = SYMBOL_FORM1_TITLE, const wxPoint& pos = SYMBOL_FORM1_POSITION, const wxSize& size = SYMBOL_FORM1_SIZE, long style = SYMBOL_FORM1_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FORM1_IDNAME, const wxString& caption = SYMBOL_FORM1_TITLE, const wxPoint& pos = SYMBOL_FORM1_POSITION, const wxSize& size = SYMBOL_FORM1_SIZE, long style = SYMBOL_FORM1_STYLE );

    /// Destructor
    ~Form1();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Form1 event handler declarations

////@end Form1 event handler declarations

////@begin Form1 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Form1 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Form1 member variables
////@end Form1 member variables
};

#endif
    // _FORM1_H_
