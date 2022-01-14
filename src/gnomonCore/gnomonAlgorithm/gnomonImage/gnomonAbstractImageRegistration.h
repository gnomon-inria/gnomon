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

#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

class dtkCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageRegistration : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageRegistration(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void addImage(gnomonImageSeries *) = 0;
    virtual void removeImages(void) = 0;

    virtual gnomonImageSeries* output() = 0;
    virtual gnomonDataDictSeries* outputTransformation() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        dtkWarn() << Q_FUNC_INFO << "Do not use decorators to implement virtual void addImage(gnomonImageSeries *)";
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        dtkWarn() << Q_FUNC_INFO << "No getter defined";
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"output"};
        } else if(formName == "gnomonDataDict") {
            return {"outputTransformation"};
        }
        return {};
    };


};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageRegistration *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageRegistration, GNOMONCORE_EXPORT, imageRegistration);
}

//
// gnomonAbstractImageRegistration.h ends here
