#pragma once

#include <gnomonVisualizationExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>
#include "gnomonVisualizations/gnomonAbstractQmlVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractDataDictQmlVisualization : public gnomonAbstractQmlVisualization
{
    Q_OBJECT

public:
    gnomonAbstractDataDictQmlVisualization(void) : gnomonAbstractQmlVisualization() {}
    virtual ~gnomonAbstractDataDictQmlVisualization(void) = default;

public:
    virtual void setDataDict(std::shared_ptr<gnomonDataDictSeries> dataDict) = 0;
    virtual std::shared_ptr<gnomonDataDictSeries> dataDict(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"setDataDict"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"dataDict"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractDataDictQmlVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataDictQmlVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictQmlVisualization, GNOMONVISUALIZATION_EXPORT, dataDictQmlVisualization, gnomonVisualization)


//
// gnomonAbstractDataDictQmlVisualization.h ends here
