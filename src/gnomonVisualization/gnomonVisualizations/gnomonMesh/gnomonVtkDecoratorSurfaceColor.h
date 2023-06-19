#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorSurfaceColor : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorSurfaceColor(void);
    ~gnomonVtkDecoratorSurfaceColor(void);

    QString name(void) const override {return "Surface Color";}
    void AddActorIfPossible(void) override;
    void removeActor(void) override;
    void setVisibility(bool) override;

    bool isDecorating(void) override;
    void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) override;
    void set2DClippingPlane(vtkSmartPointer<vtkPlane> plane) override;

    void setView(gnomonVtkView *) override;
    void unsetView(void) override;
    vtkSmartPointer<vtkActor> actor() override; //only used for imageRendering
    dtkCoreParameters parameters() override;

protected:
    struct gnomonVtkDecoratorSurfaceColorPrivate *d = nullptr;
};
