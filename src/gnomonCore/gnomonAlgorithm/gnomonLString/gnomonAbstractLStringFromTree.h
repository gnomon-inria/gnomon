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

#include <QtCore>

#include <dtkCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonCoreParameter;

#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"

class GNOMONCORE_EXPORT gnomonAbstractLStringFromTree : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonTreeSeries *tree_serie) = 0;

public:
    virtual gnomonTreeSeries *input(void) const = 0;
    virtual gnomonLStringSeries *output(void) const = 0;
};

DTK_DECLARE_OBJECT(gnomonAbstractLStringFromTree *)

DTK_DECLARE_PLUGIN(gnomonAbstractLStringFromTree, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringFromTree, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringFromTree, GNOMONCORE_EXPORT)

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractLStringFromTree, GNOMONCORE_EXPORT, lStringFromTree);
}

//
// gnomonAbstractLStringFromTree.h ends here
