#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class gnomonViewForm;

class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationBinaryImage : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationBinaryImage(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationBinaryImage(void) = default;

public:
    virtual void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> image) = 0;
    virtual std::shared_ptr<gnomonBinaryImageSeries> binaryImage(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"setBinaryImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"binaryImage"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationBinaryImage *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationBinaryImage, GNOMONVISUALIZATION_EXPORT, visualizationBinaryImage);
}

//
// gnomonAbstractVisualizationBinaryImage.h ends here
