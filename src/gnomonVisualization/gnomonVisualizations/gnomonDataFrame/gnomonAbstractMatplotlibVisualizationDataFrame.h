#pragma once

#include <gnomonVisualizationExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractMatplotlibVisualizationDataFrame : public gnomonAbstractMatplotlibVisualization
{
    Q_OBJECT

public:
    gnomonAbstractMatplotlibVisualizationDataFrame(void) : gnomonAbstractMatplotlibVisualization() {}
    virtual ~gnomonAbstractMatplotlibVisualizationDataFrame(void) = default;

public:
    virtual void setDataFrame(std::shared_ptr<gnomonDataFrame> dataFrame) = 0;
    virtual std::shared_ptr<gnomonDataFrame> dataFrame(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonDataFrame") {
            return {"setDataFrame"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonDataFrame") {
            return {"dataFrame"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMatplotlibVisualizationDataFrame *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractMatplotlibVisualizationDataFrame, GNOMONVISUALIZATION_EXPORT, matplotlibVisualizationDataFrame);
}

//
// gnomonAbstractMatplotlibVisualizationDataFrame.h ends here
