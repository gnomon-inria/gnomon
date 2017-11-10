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
class gnomonActorMeshCellComplex;
class gnomonActorMeshCellGraph;
class gnomonActorMeshCellImage;
class gnomonActorVolume;

class gnomonCellComplex;
class gnomonCellGraph;
class vtkImageData;
class vtkPolyData;

class gnomonInspectorViewTree;
class gnomonInspectorViewWidget;
class gnomonInspectorMain;

class gnomonInspectorVolume;
class gnomonInspectorCellGraph;

class QWidget;

class GNOMONVISUALIZATION_EXPORT gnomonViewManager : public QObject
{
    Q_OBJECT

public:
    gnomonActor *actor(vtkPolyData *mesh);
    gnomonActor *actor(vtkImageData *volume);
    gnomonActor *actor(gnomonCellComplex *cellcomplex);
    gnomonActor *actor(gnomonCellGraph *cellgraph);
    gnomonActor *actor(gnomonCellImage *cellimage);

public:
    gnomonActor *insert(vtkPolyData *mesh);
    gnomonActor *insert(vtkImageData *volume);
    gnomonActor *insert(gnomonCellComplex *cellcomplex);
    gnomonActor *insert(gnomonCellGraph *cellgraph);
    gnomonActor *insert(gnomonCellImage *cellimage);

public:
    void remove(vtkPolyData *mesh);
    void remove(vtkImageData *volume);
    void remove(gnomonCellComplex *cellcomplex);
    void remove(gnomonCellGraph *cellgraph);
    void remove(gnomonCellImage *cellimage);

    gnomonInspectorViewTree *inspectorTree(void);
    gnomonInspectorViewWidget *inspectorWidget(void);
    gnomonInspectorMain *inspectorMain(void);

public:
    QList<vtkPolyData *> meshes(void);
    QList<vtkImageData *> volumes(void);
    QList<gnomonCellComplex *> cellcomplexes(void);
    QList<gnomonCellGraph *> cellgraphs(void);
    QList<gnomonCellImage *> cellimages(void);

signals:
    void inserted(vtkPolyData *mesh);
    void inserted(vtkImageData *volume);
    void inserted(gnomonCellComplex *cellcomplex);
    void inserted(gnomonCellGraph *cellgraph);
    void inserted(gnomonCellImage *cellimage);

signals:
    void removed(vtkPolyData *mesh);
    void removed(vtkImageData *volume);
    void removed(gnomonCellComplex *cellcomplex);
    void removed(gnomonCellGraph *cellgraph);
    void removed(gnomonCellImage *cellimage);

 signals:
    void selected(QWidget *inspector);

public slots:
    void clear(void);

public slots:
    void update(void);

 protected slots:
    void onMeshSelected(vtkPolyData *mesh);
    void onVolumeSelected(gnomonActorVolume *volume);
    void onCellGraphSelected(gnomonActorMeshCellGraph *volume);

    void onInspectorVolumeSelected(gnomonInspectorVolume *inspector);
    void onInspectorCellGraphSelected(gnomonInspectorCellGraph *inspector);

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
