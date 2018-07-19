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

#include <gnomonTime.h>

#include <gnomonAbstractDynamicForm.h>

#include <gnomonAbstractForm.h>


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonDiscreteDynamicForm: gnomonAbstractDynamicForm
{
protected:
    QMap<gnomonTime, gnomonAbstractForm> forms;
    gnomonTime initialTime;

public:
     gnomonDiscreteDynamicForm(void);
    ~gnomonDiscreteDynamicForm(void) {};

public:
    void appendForm(gnomonAbstractForm* form, gnomonTime t);
    gnomonDiscreteDynamicForm dropForm(gnomonTime t);
    void setInitialTime(gnomonTime initialTime);
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonDiscreteDynamicForm *)
DTK_DECLARE_PLUGIN        (gnomonDiscreteDynamicForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonDiscreteDynamicForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonDiscreteDynamicForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonDiscreteDynamicForm, GNOMONCORE_EXPORT, discreteDynamicForm);
}

//
// gnomonDiscreteDynamicForm.h ends here
