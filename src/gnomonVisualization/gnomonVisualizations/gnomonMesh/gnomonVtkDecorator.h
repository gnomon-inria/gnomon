#pragma once

#include <qtmetamacros.h>

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
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
    virtual void setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) = 0;

    //actors
    virtual vtkSmartPointer<vtkActor> actor();
    virtual void AddActorIfPossible(void);
    virtual void removeActor(void);
    virtual void setVisibility(bool);

    virtual void set2DClippingPlane(vtkSmartPointer<vtkPlane> plane);
    virtual void setView(gnomonVtkView *);
    virtual void unsetView(void);
    virtual bool isDecorating(void);
    virtual dtkCoreParameters parameters();

 protected:
    bool m_is_decorating = false;
    gnomonVtkView *m_view = nullptr;
    vtkSmartPointer<vtkUnstructuredGrid> m_grid = nullptr;
    dtkCoreParameters m_parameters;

    vtkSmartPointer<vtkActor> m_actor3d = nullptr;
    vtkSmartPointer<vtkDataSetMapper> m_mapper3d = nullptr;
    vtkSmartPointer<vtkActor> m_actor2d = nullptr;
    vtkSmartPointer<vtkDataSetMapper> m_mapper2d = nullptr;
    vtkSmartPointer<vtkPlane> m_clippingPlane = nullptr;
};
