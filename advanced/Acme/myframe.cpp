/////////////////////////////////////////////////////////////////////////////
// Name:        myframe.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/26/04 00:09:32
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

#include "myframe.h"

////@begin XPM images
#include "open.xpm"
#include "save.xpm"
#include "undo.xpm"
#include "redo.xpm"
#include "copy.xpm"
#include "cut.xpm"
#include "paste.xpm"
////@end XPM images

/*!
 * MyFrame type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

/*!
 * MyFrame event table definition
 */

BEGIN_EVENT_TABLE( MyFrame, wxFrame )

////@begin MyFrame event table entries
////@end MyFrame event table entries

END_EVENT_TABLE()

/*!
 * MyFrame constructors
 */

MyFrame::MyFrame( )
{
}

MyFrame::MyFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

/*!
 * MyFrame creator
 */

bool MyFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MyFrame member initialisation
////@end MyFrame member initialisation

////@begin MyFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end MyFrame creation
    return TRUE;
}

/*!
 * Control creation for MyFrame
 */

void MyFrame::CreateControls()
{    
////@begin MyFrame content construction
    MyFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    {
        wxMenuItem* menuItem = new wxMenuItem(itemMenu3, wxID_OPEN, _("&Open...\tCtrl+O"), _("Opens a file"), wxITEM_NORMAL);
        wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("open.xpm")));
        menuItem->SetBitmap(bitmap);
        itemMenu3->Append(menuItem);
    }
    itemMenu3->Append(wxID_SAVE, _("&Save"), _("Saves a file to disk"), wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("&Exit"), _("Exits the application"), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu8 = new wxMenu;
    itemMenu8->Append(wxID_COPY, _("&Copy"), _("Copies data to the clipboard"), wxITEM_NORMAL);
    itemMenu8->Append(wxID_CUT, _("&Cut"), _("Copies to the clipboard and deletes"), wxITEM_NORMAL);
    itemMenu8->Append(wxID_PASTE, _("&Paste"), _("Pastes from the clipboard"), wxITEM_NORMAL);
    menuBar->Append(itemMenu8, _("&Edit"));
    wxMenu* itemMenu12 = new wxMenu;
    itemMenu12->Append(wxID_ABOUT, _("&About..."), _("About this application"), wxITEM_NORMAL);
    menuBar->Append(itemMenu12, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    wxScrolledWindow* itemScrolledWindow14 = new wxScrolledWindow( itemFrame1, ID_SCROLLEDWINDOW, wxDefaultPosition, wxSize(100, 100), wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    itemScrolledWindow14->SetScrollbars(1, 1, 0, 0);

    wxToolBar* itemToolBar15 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    wxBitmap itemtool16Bitmap(itemFrame1->GetBitmapResource(wxT("open.xpm")));
    wxBitmap itemtool16BitmapDisabled;
    itemToolBar15->AddTool(wxID_OPEN, wxEmptyString, itemtool16Bitmap, itemtool16BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    wxBitmap itemtool17Bitmap(itemFrame1->GetBitmapResource(wxT("save.xpm")));
    wxBitmap itemtool17BitmapDisabled;
    itemToolBar15->AddTool(wxID_SAVE, wxEmptyString, itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    itemToolBar15->AddSeparator();
    wxBitmap itemtool19Bitmap(itemFrame1->GetBitmapResource(wxT("undo.xpm")));
    wxBitmap itemtool19BitmapDisabled;
    itemToolBar15->AddTool(wxID_UNDO, wxEmptyString, itemtool19Bitmap, itemtool19BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    wxBitmap itemtool20Bitmap(itemFrame1->GetBitmapResource(wxT("redo.xpm")));
    wxBitmap itemtool20BitmapDisabled;
    itemToolBar15->AddTool(wxID_REDO, wxEmptyString, itemtool20Bitmap, itemtool20BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    itemToolBar15->AddSeparator();
    wxBitmap itemtool22Bitmap(itemFrame1->GetBitmapResource(wxT("copy.xpm")));
    wxBitmap itemtool22BitmapDisabled;
    itemToolBar15->AddTool(wxID_COPY, wxEmptyString, itemtool22Bitmap, itemtool22BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    wxBitmap itemtool23Bitmap(itemFrame1->GetBitmapResource(wxT("cut.xpm")));
    wxBitmap itemtool23BitmapDisabled;
    itemToolBar15->AddTool(wxID_CUT, wxEmptyString, itemtool23Bitmap, itemtool23BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    wxBitmap itemtool24Bitmap(itemFrame1->GetBitmapResource(wxT("paste.xpm")));
    wxBitmap itemtool24BitmapDisabled;
    itemToolBar15->AddTool(wxID_PASTE, wxEmptyString, itemtool24Bitmap, itemtool24BitmapDisabled, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    itemToolBar15->Realize();
    itemFrame1->SetToolBar(itemToolBar15);

    wxStatusBar* itemStatusBar25 = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar25->SetFieldsCount(2);
    itemStatusBar25->SetStatusText(_("Field One"), 0);
    itemStatusBar25->SetStatusText(_("Field Two"), 1);
    int itemStatusBar25Widths[2];
    itemStatusBar25Widths[0] = 200;
    itemStatusBar25Widths[1] = -1;
    itemStatusBar25->SetStatusWidths(2, itemStatusBar25Widths);
    itemFrame1->SetStatusBar(itemStatusBar25);

////@end MyFrame content construction
}

/*!
 * Should we show tooltips?
 */

bool MyFrame::ShowToolTips()
{
    return TRUE;
}

/*!
 * Get bitmap resources
 */

wxBitmap MyFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MyFrame bitmap retrieval
    wxUnusedVar(name);
    if (name == wxT("open.xpm"))
    {
        wxBitmap bitmap(open_xpm);
        return bitmap;
    }
    else if (name == wxT("save.xpm"))
    {
        wxBitmap bitmap(save_xpm);
        return bitmap;
    }
    else if (name == wxT("undo.xpm"))
    {
        wxBitmap bitmap(undo_xpm);
        return bitmap;
    }
    else if (name == wxT("redo.xpm"))
    {
        wxBitmap bitmap(redo_xpm);
        return bitmap;
    }
    else if (name == wxT("copy.xpm"))
    {
        wxBitmap bitmap(copy_xpm);
        return bitmap;
    }
    else if (name == wxT("cut.xpm"))
    {
        wxBitmap bitmap(cut_xpm);
        return bitmap;
    }
    else if (name == wxT("paste.xpm"))
    {
        wxBitmap bitmap(paste_xpm);
        return bitmap;
    }
    return wxNullBitmap;
////@end MyFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon MyFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MyFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MyFrame icon retrieval
}
