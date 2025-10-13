/////////////////////////////////////////////////////////////////////////////
// Name:        project1app.cpp
// Purpose:     
// Author:      Ivan Carlos Da Silva Lopes
// Modified by: 
// Created:     seg 13 out 2025 20:04:50
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
#include "wx/fs_zip.h"
////@end includes

#include "project1app.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( Project1App )
////@end implement app


/*
 * Project1App type definition
 */

IMPLEMENT_CLASS( Project1App, wxApp )


/*
 * Project1App event table definition
 */

BEGIN_EVENT_TABLE( Project1App, wxApp )

////@begin Project1App event table entries
////@end Project1App event table entries

END_EVENT_TABLE()


/*
 * Constructor for Project1App
 */

Project1App::Project1App()
{
    Init();
}


/*
 * Member initialisation
 */

void Project1App::Init()
{
////@begin Project1App member initialisation
////@end Project1App member initialisation
}

/*
 * Initialisation for Project1App
 */

bool Project1App::OnInit()
{    
////@begin Project1App initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

	wxXmlResource::Get()->InitAllHandlers();
	wxFileSystem::AddHandler(new wxZipFSHandler);
	wxXmlResource::Get()->Load(wxT("ui.xrc"));

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	frame1* mainWindow = new frame1( NULL );
	mainWindow->Show(true);
////@end Project1App initialisation

    return true;
}


/*
 * Cleanup for Project1App
 */

int Project1App::OnExit()
{    
////@begin Project1App cleanup
	return wxApp::OnExit();
////@end Project1App cleanup
}

