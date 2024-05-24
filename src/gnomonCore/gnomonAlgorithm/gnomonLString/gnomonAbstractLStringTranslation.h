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
// TODO: revoir tout ça :
#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class dtkCoreParameter;

#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"

class GNOMONCORE_EXPORT gnomonAbstractLStringTranslation : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual int run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInputTree(std::shared_ptr<gnomonTreeSeries> tree_series) { return; };
    virtual void setInputLString(std::shared_ptr<gnomonLStringSeries> lString_series) { return; };

public:
    virtual std::shared_ptr<gnomonTreeSeries> inputTree(void) const { return nullptr; };
    virtual std::shared_ptr<gnomonTreeSeries> outputTree(void) const { return nullptr; };
    virtual std::shared_ptr<gnomonLStringSeries> inputLString(void) const { return nullptr; };
    virtual std::shared_ptr<gnomonLStringSeries> outputLString(void) const { return nullptr; };

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"setInputLString"};
        } else if (formName == "gnomonTree") {
            return {"setInputTree"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"inputLString"};
        } else if (formName == "gnomonTree") {
            return {"inputTree"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonLString") {
            return {"outputLString"};
        } else if (formName == "gnomonTree") {
            return {"outputTree"};
        }
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractLStringTranslation *)

DTK_DECLARE_PLUGIN(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringTranslation, GNOMONCORE_EXPORT, lStringTranslation, gnomonCore)


//
// gnomonAbstractLStringTranslation.h ends here
