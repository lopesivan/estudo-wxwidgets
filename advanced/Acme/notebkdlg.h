/////////////////////////////////////////////////////////////////////////////
// Name:        notebkdlg.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/21/04 17:32:17
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NOTEBKDLG_H_
#define _NOTEBKDLG_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/splitter.h"
#include "wx/treectrl.h"
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
#define ID_NOTEBOOK_DIALOG 10000
#define ID_NOTEBOOK 10001
#define ID_PANEL 10002
#define ID_BUTTON 10004
#define ID_BUTTON1 10005
#define ID_BUTTON2 10006
#define ID_TEXTCTRL 10007
#define ID_PANEL1 10003
#define ID_SPLITTERWINDOW 10008
#define ID_TREECTRL 10010
#define ID_TEXTCTRL1 10009
#define SYMBOL_MYDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MYDIALOG_TITLE _("Notebook dialog")
#define SYMBOL_MYDIALOG_IDNAME ID_NOTEBOOK_DIALOG
#define SYMBOL_MYDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_MYDIALOG_POSITION wxDefaultPosition
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
 * MyDialog class declaration
 */

class MyDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( MyDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MyDialog( );
    MyDialog( wxWindow* parent, wxWindowID id = SYMBOL_MYDIALOG_IDNAME, const wxString& caption = SYMBOL_MYDIALOG_TITLE, const wxPoint& pos = SYMBOL_MYDIALOG_POSITION, const wxSize& size = SYMBOL_MYDIALOG_SIZE, long style = SYMBOL_MYDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYDIALOG_IDNAME, const wxString& caption = SYMBOL_MYDIALOG_TITLE, const wxPoint& pos = SYMBOL_MYDIALOG_POSITION, const wxSize& size = SYMBOL_MYDIALOG_SIZE, long style = SYMBOL_MYDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin MyDialog event handler declarations

////@end MyDialog event handler declarations

////@begin MyDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MyDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MyDialog member variables
////@end MyDialog member variables
};

#endif
    // _NOTEBKDLG_H_
