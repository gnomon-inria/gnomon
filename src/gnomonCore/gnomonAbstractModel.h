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

#include "gnomonTime.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonAbstractDynamicForm;

class GNOMONCORE_EXPORT gnomonAbstractModel
{
public:
             gnomonAbstractModel(void) = default;
    virtual ~gnomonAbstractModel(void) {};

public:
    void setForm(gnomonAbstractDynamicForm *);

    virtual void run(const gnomonTime&, unsigned long) = 0;

protected:
    class gnomonAbstractDynamicForm *dynamic_form = nullptr;
};

typedef QSharedPointer<gnomonAbstractModel> gnomonAbstractModelPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractModel *)
// DTK_DECLARE_PLUGIN        (gnomonAbstractModel, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractModel, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractModel, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

// namespace gnomonCore {
//     DTK_DECLARE_CONCEPT(gnomonAbstractModel, GNOMONCORE_EXPORT, form);
// }

//
// gnomonAbstractModel.h ends here
