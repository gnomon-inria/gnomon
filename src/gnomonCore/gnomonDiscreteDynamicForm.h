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

class GNOMONCORE_EXPORT gnomonDiscreteDynamicForm : gnomonAbstractDynamicForm
{

public:
     gnomonDiscreteDynamicForm(void);
     gnomonDiscreteDynamicForm(const gnomonDiscreteDynamicForm& o);
    ~gnomonDiscreteDynamicForm(void);

    gnomonAbstractDynamicForm* clone(void) const { return new gnomonDiscreteDynamicForm(*this); };

public:
	gnomonAbstractForm* atTime(gnomonTime t);
    void appendForm(gnomonAbstractForm* form, gnomonTime t);
    void dropForm(gnomonTime t);
    void setInitialTime(gnomonTime initialTime);

private:
    class gnomonDiscreteDynamicFormPrivate *d;

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
