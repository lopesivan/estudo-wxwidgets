/////////////////////////////////////////////////////////////////////////////
// Name:        myframe.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/26/04 00:09:32
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYFRAME_H_
#define _MYFRAME_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
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
#define ID_FRAME 10000
#define ID_SCROLLEDWINDOW 10004
#define ID_TOOLBAR 10003
#define ID_STATUSBAR 10001
#define SYMBOL_MYFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_MYFRAME_TITLE _("Simple frame")
#define SYMBOL_MYFRAME_IDNAME ID_FRAME
#define SYMBOL_MYFRAME_SIZE wxSize(400, 300)
#define SYMBOL_MYFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

/*!
 * MyFrame class declaration
 */

class MyFrame: public wxFrame
{    
    DECLARE_DYNAMIC_CLASS( MyFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MyFrame( );
    MyFrame( wxWindow* parent, wxWindowID id = SYMBOL_MYFRAME_IDNAME, const wxString& caption = SYMBOL_MYFRAME_TITLE, const wxPoint& pos = SYMBOL_MYFRAME_POSITION, const wxSize& size = SYMBOL_MYFRAME_SIZE, long style = SYMBOL_MYFRAME_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYFRAME_IDNAME, const wxString& caption = SYMBOL_MYFRAME_TITLE, const wxPoint& pos = SYMBOL_MYFRAME_POSITION, const wxSize& size = SYMBOL_MYFRAME_SIZE, long style = SYMBOL_MYFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin MyFrame event handler declarations

////@end MyFrame event handler declarations

////@begin MyFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MyFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MyFrame member variables
////@end MyFrame member variables
};

#endif
    // _MYFRAME_H_
