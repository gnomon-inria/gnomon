#pragma once

#include <gnomonVisualizationExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractDataFrameMplVisualization : public gnomonAbstractMplVisualization
{
    Q_OBJECT

public:
    gnomonAbstractDataFrameMplVisualization(void) : gnomonAbstractMplVisualization() {}
    virtual ~gnomonAbstractDataFrameMplVisualization(void) = default;

public:
    virtual void setDataFrame(std::shared_ptr<gnomonDataFrameSeries> dataFrame) = 0;
    virtual std::shared_ptr<gnomonDataFrameSeries> dataFrame(void) = 0;

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

DTK_DECLARE_OBJECT        (gnomonAbstractDataFrameMplVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataFrameMplVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataFrameMplVisualization, GNOMONVISUALIZATION_EXPORT, dataFrameMplVisualization, gnomonVisualization)


//
// gnomonAbstractDataFrameMplVisualization.h ends here
