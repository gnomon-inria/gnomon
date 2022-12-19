#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractCellImageConstructor : public gnomonAbstractAlgorithm
{
public:
    virtual std::shared_ptr<gnomonCellImageSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT, cellImageConstructor);
}

//
// gnomonAbstractCellImageConstructor.h ends here
