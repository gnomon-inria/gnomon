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

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractStaticForm
{
public:
             gnomonAbstractStaticForm(void) = default;
    virtual ~gnomonAbstractStaticForm(void) {};

    virtual gnomonAbstractStaticForm* clone(void) const = 0;

public:
    
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractStaticForm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractStaticForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractStaticForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractStaticForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractStaticForm, GNOMONCORE_EXPORT, staticForm);
}

//
// gnomonAbstractStaticForm.h ends here
