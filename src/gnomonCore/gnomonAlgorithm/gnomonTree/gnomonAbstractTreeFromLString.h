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

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"

class gnomonTree;
class gnomonLString;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTreeFromLString : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractTreeFromLString(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

//public:
//    virtual void setLSystem(const QString& lsystem) = 0;

public:
    virtual void setInput(gnomonLStringSeries *) = 0;
    virtual gnomonLStringSeries *input() = 0;

    virtual gnomonTreeSeries *output() = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractTreeFromLString *)
DTK_DECLARE_PLUGIN        (gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeFromLString, GNOMONCORE_EXPORT, treeFromLString);
}

//
// gnomonAbstractTreeFromLString.h ends here
