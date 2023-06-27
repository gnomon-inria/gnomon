#pragma once

#include "gnomonVtkDecorator.h"

#include <gnomonVisualizationExport.h>

class GNOMONVISUALIZATION_EXPORT gnomonVtkDecoratorVectorGlyphs : public gnomonVtkDecorator
{
 public:
    gnomonVtkDecoratorVectorGlyphs(void);
    ~gnomonVtkDecoratorVectorGlyphs(void);

    QString name(void) const override {return "Vector Glyphs";}
    void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) override;

    friend struct gnomonVtkDecoratorVectorGlyphsPrivate;
protected:
    struct gnomonVtkDecoratorVectorGlyphsPrivate *d = nullptr;
};
