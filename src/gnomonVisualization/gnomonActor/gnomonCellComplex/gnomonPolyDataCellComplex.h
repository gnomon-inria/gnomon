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

#include <vtkObject.h>

#include <gnomonVisualizationExport.h>

#include "gnomonActor/gnomonActor.h"
#include "gnomonActor/gnomonPolyData/gnomonPolyData.h"

class gnomonCellComplex;

class GNOMONVISUALIZATION_EXPORT gnomonPolyDataCellComplex : public gnomonPolyData
{
public:
    vtkTypeMacro(gnomonPolyDataCellComplex, vtkPolyData);

    static gnomonPolyDataCellComplex *New(void);

public:
	void setCellComplex(gnomonCellComplex *cellComplex);
    void setScaleFactor(double scale_factor);
    void setPropertyName(const QString& property_name);
    void set8Bit(bool value);

public slots:
    void update(void) override;
    void modified(void);

public:
     gnomonPolyDataCellComplex(void);
    ~gnomonPolyDataCellComplex(void);

private:
    gnomonPolyDataCellComplex(const gnomonPolyDataCellComplex&);
    void operator = (const gnomonPolyDataCellComplex&);

private:
	class gnomonPolyDataCellComplexPrivate *d;
};


//
// gnomonPolyDataCellComplex.h ends here
