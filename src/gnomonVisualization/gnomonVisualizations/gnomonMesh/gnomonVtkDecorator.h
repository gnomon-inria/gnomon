#pragma once

#include <qtmetamacros.h>

#include <vtkActor.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <dtkCoreParameters.h>

#include "gnomonView/gnomonVtkView.h"

#include <gnomonVisualizationExport.h>


class GNOMONVISUALIZATION_EXPORT gnomonVtkDecorator : public QObject
{
    Q_OBJECT

 public:
    gnomonVtkDecorator(void) = default;
    ~gnomonVtkDecorator(void) = default;
    virtual QString name(void) const = 0;

    virtual void AddActorIfPossible(void) = 0;
    virtual void removeActor(void) = 0;
    virtual void setVisibility(bool) = 0;


    virtual bool isDecorating(void) = 0;
    virtual void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) = 0;
    virtual void set2DClippingPlane(vtkSmartPointer<vtkPlane> plane) =0;
    virtual void setView(gnomonVtkView *) = 0;
    virtual void unsetView(void) = 0;

    virtual vtkSmartPointer<vtkActor> actor() =0;
    virtual dtkCoreParameters parameters() = 0;
};
