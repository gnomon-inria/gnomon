#pragma once
#include <QtCore>
#include <QtGui>

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

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged);

signals:
    void backgroundColorChanged(void);

public:
    void setBackgroundColor(const QColor& color);
    const QColor& backgroundColor(void);

public:
    void associate(vtkRenderWindow *);
    void addActor(vtkSmartPointer<vtkActor> actor);
    vtkRenderWindow* renderWindow(void) const;

private:
    class gnomonNurbsViewPrivate *d;
};