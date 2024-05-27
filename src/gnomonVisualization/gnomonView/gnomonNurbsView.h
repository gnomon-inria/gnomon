#pragma once
#include <QtCore>

#include <gnomonVisualizationExport>
#include <vtkSmartPointer.h>


class vtkCamera;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkActor;

class GNOMONVISUALIZATION_EXPORT gnomonNurbsView : public QObject
{
    Q_OBJECT
public:
    gnomonNurbsView(QObject *parent = nullptr);
    virtual ~gnomonNurbsView(void)=default;

public:
    void associate(vtkRenderWindow *);
    void addActor(vtkSmartPointer<vtkActor> actor);
    vtkRenderWindow* renderWindow(void) const;

private:
    vtkSmartPointer<vtkRenderWindow> m_window;
    vtkSmartPointer<vtkRenderer> m_renderer;
};