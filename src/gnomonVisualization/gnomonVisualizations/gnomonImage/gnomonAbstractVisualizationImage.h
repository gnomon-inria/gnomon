#pragma once

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonVisualizations/gnomonAbstractVisualization.h>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
             gnomonAbstractVisualizationImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationImage(void) = default;

public:
    virtual void setImage(std::shared_ptr<gnomonImageSeries> image) = 0;
    virtual std::shared_ptr<gnomonImageSeries> image(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationImage, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationImage, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationImage, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractVisualizationImage, GNOMONVISUALIZATION_EXPORT, visualizationImage);
}

//
// gnomonAbstractVisualizationImage.h ends here
