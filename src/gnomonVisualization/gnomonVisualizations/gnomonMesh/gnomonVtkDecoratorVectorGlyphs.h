#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorVectorGlyphs : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorVectorGlyphs(void);
    ~gnomonVtkDecoratorVectorGlyphs(void);

    QString name(void) const override {return "Vector Glyphs";}
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
    struct gnomonVtkDecoratorVectorGlyphsPrivate *d = nullptr;
};
