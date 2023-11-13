#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore/dtkCorePlugin>
#include "gnomonCore/gnomonCorePlugin.h"

class vtkImageData;
class gnomonCellGraph;

class GNOMONCORE_EXPORT gnomonAbstractCellGraphFromImage : public QRunnable
{

    //Inputs
public:
    virtual void setUseMargins(bool use_margins) = 0;

    virtual void setImage(vtkImageData *image) = 0;

    virtual void setMinContactArea(float min_val) = 0;

    virtual void setBackgroundLabel(long label) = 0;

    virtual void setPredefinedProperties(const QStringList& names) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonCellGraph> computedGraph(void) const = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellGraphFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellGraphFromImage, GNOMONCORE_EXPORT, cellGraphFromImage, gnomonCore)


//
// gnomonAbstractCellGraphFromImage.h ends here
