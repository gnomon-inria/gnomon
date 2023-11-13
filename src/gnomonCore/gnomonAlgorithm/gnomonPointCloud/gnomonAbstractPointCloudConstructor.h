#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"

class GNOMONCORE_EXPORT gnomonAbstractPointCloudConstructor : public gnomonAbstractAlgorithm
{
public:
    virtual std::shared_ptr<gnomonPointCloudSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudConstructor, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudConstructor, GNOMONCORE_EXPORT, pointCloudConstructor, gnomonCore)


//
// gnomonAbstractPointCloudConstructor.h ends here
