// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationCellComplex : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
             gnomonAbstractVisualizationCellComplex(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationCellComplex(void) = default;

public:
    virtual void setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplex) = 0;
    virtual std::shared_ptr<gnomonCellComplexSeries> cellComplex(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"setCellComplex"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"cellComplex"};
        }
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationCellComplex *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractVisualizationCellComplex, GNOMONVISUALIZATION_EXPORT, visualizationCellComplex);
}

//
// gnomonAbstractVisualizationCellComplex.h ends here
