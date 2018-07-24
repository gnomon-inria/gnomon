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

class GNOMONCORE_EXPORT gnomonDiscreteDynamicForm : public gnomonAbstractDynamicForm
{

public:
     gnomonDiscreteDynamicForm(void);
     gnomonDiscreteDynamicForm(const gnomonDiscreteDynamicForm& o);
    ~gnomonDiscreteDynamicForm(void);

    gnomonAbstractDynamicForm* clone(void) const { return new gnomonDiscreteDynamicForm(*this); };

public:
    void setInitialTime(gnomonTime initialTime) override;
	gnomonAbstractForm* atTime(gnomonTime t) override;

    void insert(gnomonAbstractForm* form, gnomonTime t) override;
    void drop(gnomonTime t) override;

    QList<gnomonTime> availableTimes(void) override;

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
