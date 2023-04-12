#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"

class gnomonVtkView;

class dtkCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonAbstractBinaryImageVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
    gnomonAbstractBinaryImageVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractBinaryImageVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractBinaryImageVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractBinaryImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractBinaryImageVtkVisualization, GNOMONVISUALIZATION_EXPORT, binaryImageVtkVisualization);
}

//
// gnomonAbstractBinaryImageVtkVisualization.h ends here
