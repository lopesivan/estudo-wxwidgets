/////////////////////////////////////////////////////////////////////////////
// Name:        acme.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     11/21/04 17:33:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ACME_H_
#define _ACME_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "newproject.h"
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
////@end control identifiers

/*!
 * AcmeApp class declaration
 */

class AcmeApp: public wxApp
{    
    DECLARE_CLASS( AcmeApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    AcmeApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin AcmeApp event handler declarations

////@end AcmeApp event handler declarations

////@begin AcmeApp member function declarations

////@end AcmeApp member function declarations

////@begin AcmeApp member variables
////@end AcmeApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(AcmeApp)
////@end declare app

#endif
    // _ACME_H_
