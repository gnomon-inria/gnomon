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
#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractCellComplexVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
             gnomonAbstractCellComplexVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractCellComplexVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractCellComplexVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellComplexVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexVtkVisualization, GNOMONVISUALIZATION_EXPORT, cellComplexVtkVisualization, gnomonVisualization)


//
// gnomonAbstractCellComplexVtkVisualization.h ends here
