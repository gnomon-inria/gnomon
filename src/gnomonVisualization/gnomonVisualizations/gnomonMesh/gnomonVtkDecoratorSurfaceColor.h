#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorSurfaceColor : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorSurfaceColor(void);
    ~gnomonVtkDecoratorSurfaceColor(void);

    QString name(void) const override {return "Surface Color";}
    void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) override;

    friend struct gnomonVtkDecoratorSurfaceColorPrivate;

protected:
    struct gnomonVtkDecoratorSurfaceColorPrivate *d = nullptr;

};
