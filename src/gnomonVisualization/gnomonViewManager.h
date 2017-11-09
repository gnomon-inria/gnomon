// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include "gnomonVisualizationExport.h"

#include <QtCore>
#include <QWidget>

class gnomonActor;
class gnomonActorMesh;
class gnomonActorVolume;

class vtkImageData;
class vtkPolyData;

class gnomonInspectorViewTree;
class gnomonInspectorViewWidget;
class gnomonInspectorMain;
class QWidget;

class GNOMONVISUALIZATION_EXPORT gnomonViewManager : public QObject
{
    Q_OBJECT

public:
    gnomonActor *actor(vtkPolyData *mesh);
    gnomonActor *actor(vtkImageData *volume);

public:
    gnomonActor *insert(vtkPolyData *mesh);
    gnomonActor *insert(vtkImageData *volume);

public:
    void remove(vtkPolyData *mesh);
    void remove(vtkImageData *volume);

    gnomonInspectorViewTree *inspectorTree(void);
    gnomonInspectorViewWidget *inspectorWidget(void);
    gnomonInspectorMain *inspectorMain(void);

public:
    QList<vtkPolyData *> meshes(void);
    QList<vtkImageData *> volumes(void);

signals:
    void inserted(vtkPolyData *mesh);
    void inserted(vtkImageData *volume);

signals:
    void removed(vtkPolyData *mesh);
    void removed(vtkImageData *volume);

 signals:
    void selected(QWidget *inspector);

public slots:
    void clear(void);

public slots:
    void update(void);

 protected slots:
    void onMeshSelected(vtkPolyData *mesh);
    void onVolumeSelected(vtkImageData *volume);

private:
     gnomonViewManager(void);
    ~gnomonViewManager(void);

private:
    class gnomonViewManagerPrivate *d;

private:
    friend class gnomonView;
};

//
// gnomonViewManager.h ends here
