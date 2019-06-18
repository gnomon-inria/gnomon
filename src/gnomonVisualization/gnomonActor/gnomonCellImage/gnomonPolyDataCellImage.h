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

class gnomonCellImage;

class GNOMONVISUALIZATION_EXPORT gnomonPolyDataCellImage : public gnomonPolyData
{
public:
    vtkTypeMacro(gnomonPolyDataCellImage, vtkPolyData);

    static gnomonPolyDataCellImage *New(void);

public:
	void setCellImage(gnomonCellImage *cellimage);

public slots:
    void update(void) override;
    void modified(void);

    void setPropertyName(const QString& value);
    void setSliceRanges(const QList<double>& x_value, const QList<double>& y_value, const QList<double>& z_value);

public:
    long cellId(long vtkId);

public:
     gnomonPolyDataCellImage(void);
    ~gnomonPolyDataCellImage(void);

private:
    gnomonPolyDataCellImage(const gnomonPolyDataCellImage&);
    void operator = (const gnomonPolyDataCellImage&);

private:
	class gnomonPolyDataCellImagePrivate *d;
};


//
// gnomonPolyDataCellImage.h ends here