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

class dtkCoreParameter;

#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"

class GNOMONCORE_EXPORT gnomonAbstractLStringTranslation : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInputTree(gnomonTreeSeries *tree_series) { return; };
    virtual void setInputLString(gnomonLStringSeries *lString_series) { return; };

public:
    virtual gnomonTreeSeries *inputTree(void) const { return nullptr; };
    virtual gnomonTreeSeries *outputTree(void) const { return nullptr; };
    virtual gnomonLStringSeries *inputLString(void) const { return nullptr; };
    virtual gnomonLStringSeries *outputLString(void) const { return nullptr; };
};

DTK_DECLARE_OBJECT(gnomonAbstractLStringTranslation *)

DTK_DECLARE_PLUGIN(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT)

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT, lStringTranslation);
}

//
// gnomonAbstractLStringTranslation.h ends here
