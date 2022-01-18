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

#include "gnomonLandmark.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonImage/gnomonImage.h"

class dtkCoreParameter;


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageFusion : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageFusion(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void addImage(gnomonImageSeries *) = 0;
    virtual void removeImages(void) = 0;

    virtual void addLandmarks(const std::vector<gnomonLandmark>&) = 0;
    virtual void removeLandmarks(void) = 0;

public:
    virtual gnomonImageSeries *output() = 0;

public:
    // TODO: check later with Guillaume
    static inline QString defaultSetter(QString formName) {
        dtkWarn() << Q_FUNC_INFO << "Do not use decorators to implement virtual void addImage(gnomonImageSeries *) since this method accept multiple images of the same type";
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        dtkWarn() << Q_FUNC_INFO << "No getter defined";
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"output"};
        }
        return {};
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageFusion *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageFusion, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageFusion, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageFusion, GNOMONCORE_EXPORT, imageFusion);
}

//
// gnomonAbstractImageFusion.h ends here
