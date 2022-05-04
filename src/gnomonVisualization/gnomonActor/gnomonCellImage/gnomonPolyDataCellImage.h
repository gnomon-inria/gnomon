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
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

#include "gnomonActor/gnomonActor.h"
#include "gnomonActor/gnomonPolyData/gnomonPolyData.h"

class gnomonCellImage;

class GNOMONVISUALIZATION_EXPORT gnomonPolyDataCellImage : public gnomonPolyData
{
public:
    vtkTypeMacro(gnomonPolyDataCellImage, vtkPolyData);

    static gnomonPolyDataCellImage *New(void);

public:
	void setCellImage(std::shared_ptr<gnomonCellImage> cellimage);

public slots:
    void update(void) override;
    void modified(void);

    void setPropertyName(const QString& value);
    void setSliceRanges(const std::array<double, 2>& x_value, const std::array<double, 2>& y_value, const std::array<double, 2>& z_value);

    void set8Bit(bool value);

public:
    long cellId(long vtkId);

public:
    void setResamplingSpacing(double);

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