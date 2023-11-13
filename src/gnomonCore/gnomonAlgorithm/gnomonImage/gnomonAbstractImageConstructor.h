#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractImageConstructor : public gnomonAbstractAlgorithm
{
    // Outputs
public:
    virtual std::shared_ptr<gnomonImageSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractImageConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT, imageConstructor, gnomonCore)


//
// gnomonAbstractImageConstructor.h ends here
