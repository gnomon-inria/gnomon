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

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractForm;
class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void) = default;
    virtual ~gnomonAbstractDynamicForm(void) {};

public:
    virtual void setInitialTime(gnomonTime T_i) = 0;
    virtual gnomonAbstractForm* atTime(gnomonTime t) = 0;

};

typedef QSharedPointer<gnomonAbstractDynamicForm> gnomonAbstractDynamicFormPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractDynamicForm *)
// DTK_DECLARE_PLUGIN        (gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

// namespace gnomonCore {
//     DTK_DECLARE_CONCEPT(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT, dynamicForm);
// }

//
// gnomonAbstractDynamicForm.h ends here
