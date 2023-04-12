#pragma once

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonVisualizations/gnomonAbstractVtkVisualization.h>

class GNOMONVISUALIZATION_EXPORT gnomonAbstractImageVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
             gnomonAbstractImageVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractImageVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractImageVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractImageVtkVisualization, GNOMONVISUALIZATION_EXPORT, imageVtkVisualization);
}

//
// gnomonAbstractImageVtkVisualization.h ends here
