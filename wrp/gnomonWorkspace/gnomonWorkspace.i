// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

%module gnomonworkspace

%include <dtkBase/dtkBase.i>

%import <dtkCore/dtkCore.i>

%{

#include <dtkWidgets>
#include <gnomonCore>
#include <gnomonWorkspace>

%}

// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONWORKSPACE_EXPORT
#define GNOMONWORKSPACE_EXPORT

// /////////////////////////////////////////////////////////////////
// SIP/SWIG connection
// /////////////////////////////////////////////////////////////////

%inline
%{

void setupLPY(qlonglong address)
{
    QWidget *widget = reinterpret_cast<QWidget *>(address);

    foreach(QWidget *top, qApp->topLevelWidgets()) {
        foreach(gnomonWorkspaceLSystemSimulator *simulator, top->findChildren<gnomonWorkspaceLSystemSimulator *>()) {
            simulator->fill(widget);
        }
    }
}

%}

// /////////////////////////////////////////////////////////////////
// Wrapper input
// /////////////////////////////////////////////////////////////////

%include <gnomonWorkspace/gnomonWorkspaceLSystemSimulator.h>

//
// gnomonWorkspace.i ends here
