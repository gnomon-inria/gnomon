#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <vtkAssembly.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

class GNOMONVISUALIZATION_EXPORT gnomonActor : public QObject, public vtkAssembly
{
public:
    virtual ~gnomonActor(void);

public:
    virtual bool isVisible(void);

public:
    virtual void setInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor) = 0;

public slots:
    virtual void hide(void);
    virtual void show(void);

public slots:
    virtual void update(void) = 0;
};

//
// gnomonActor.h ends here
