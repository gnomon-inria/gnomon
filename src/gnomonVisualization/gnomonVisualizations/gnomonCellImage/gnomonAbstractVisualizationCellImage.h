#pragma once

#include <gnomonVisualizationExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationCellImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationCellImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellImage(void) = default;

public:
    virtual void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) = 0;
    virtual std::shared_ptr<gnomonCellImageSeries> cellImage(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
        }
        return {};
    };
};

// /////////////////    //////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractVisualizationCellImage, GNOMONVISUALIZATION_EXPORT, visualizationCellImage);
}

//
// gnomonAbstractVisualizationCellImage.h ends here
