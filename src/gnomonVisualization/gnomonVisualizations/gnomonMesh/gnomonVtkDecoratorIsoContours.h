#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorIsoContours : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorIsoContours(void);
    ~gnomonVtkDecoratorIsoContours(void);

    QString name(void) const override {return "Iso Contours";}
    void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) override;

    friend struct gnomonVtkDecoratorIsoContoursPrivate;

protected:
    struct gnomonVtkDecoratorIsoContoursPrivate *d = nullptr;
};
