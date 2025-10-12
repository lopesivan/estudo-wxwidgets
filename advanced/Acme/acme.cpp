/////////////////////////////////////////////////////////////////////////////
// Name:        acme.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/21/04 17:33:50
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

#include "acme.h"

////@begin XPM images
////@end XPM images

/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( AcmeApp )
////@end implement app

/*!
 * AcmeApp type definition
 */

IMPLEMENT_CLASS( AcmeApp, wxApp )

/*!
 * AcmeApp event table definition
 */

BEGIN_EVENT_TABLE( AcmeApp, wxApp )

////@begin AcmeApp event table entries
////@end AcmeApp event table entries

END_EVENT_TABLE()

/*!
 * Constructor for AcmeApp
 */

AcmeApp::AcmeApp()
{
////@begin AcmeApp member initialisation
////@end AcmeApp member initialisation
}

/*!
 * Initialisation for AcmeApp
 */

bool AcmeApp::OnInit()
{    
////@begin AcmeApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

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
	AcmeNewProject* mainWindow = new AcmeNewProject(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
////@end AcmeApp initialisation

    return true;
}

/*!
 * Cleanup for AcmeApp
 */
int AcmeApp::OnExit()
{    
////@begin AcmeApp cleanup
	return wxApp::OnExit();
////@end AcmeApp cleanup
}

