#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>


//this decorator will display the same thing in 2d and in 3d
class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorStreamTracer : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorStreamTracer(void);
    ~gnomonVtkDecoratorStreamTracer(void);

    QString name(void) const override {return "Stream Tracer";}
    void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) override;
    void setView(gnomonVtkView *) override;
    friend struct gnomonVtkDecoratorStreamTracerPrivate;

protected:
    struct gnomonVtkDecoratorStreamTracerPrivate *d = nullptr;
};
