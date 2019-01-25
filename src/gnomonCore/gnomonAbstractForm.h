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

#include <gnomonCoreExport>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractForm
{
public:
             gnomonAbstractForm(void) = default;
    virtual ~gnomonAbstractForm(void) = default;

//public:
//    virtual void accept(class gnomonFormVisitor *) {};
//
//protected:
//    void record(void);

public:
    virtual QString name(void) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractForm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractForm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractForm, GNOMONCORE_EXPORT, abstractForm);
}

//
// gnomonAbstractForm.h ends here
