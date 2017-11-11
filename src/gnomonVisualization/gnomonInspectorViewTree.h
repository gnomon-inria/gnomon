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

#include <gnomonVisualizationExport.h>

#include <QtGui>
#include <QtWidgets>

class vtkPolyData;
class vtkImageData;

class gnomonCellComplex;
class gnomonCellGraph;

class gnomonActor;
class gnomonActorVolume;
class gnomonActorImage;
class gnomonActorMeshCellGraph;

class gnomonInspectorViewTreePrivate;
class gnomonInspectorImage;
class gnomonInspectorVolume;
class gnomonInspectorCellGraph;

class gnomonView;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewTree : public QTreeWidget
{
    Q_OBJECT

public:
     gnomonInspectorViewTree(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorViewTree(void);

    void setView(gnomonView *view);

public:
    void insert(vtkPolyData *mesh);

    QTreeWidgetItem *insert(gnomonActorVolume *volume_actor);
    QTreeWidgetItem *addChild(QTreeWidgetItem *parent, gnomonInspectorVolume *inspector_volume);

    QTreeWidgetItem *insert(gnomonActorImage *image_actor);
    QTreeWidgetItem *addChild(QTreeWidgetItem *parent, gnomonInspectorImage *inspector_image);

    void insert(gnomonCellComplex *complex);

    QTreeWidgetItem *insert(gnomonActorMeshCellGraph *cellgraph_actor);
    QTreeWidgetItem *addChild(QTreeWidgetItem *parent, gnomonInspectorCellGraph *inspector_cellgraph);

signals:
    void checked(gnomonActor *, bool);

    void selected(vtkPolyData *mesh);
    void selected(gnomonActorVolume *volume);
    void selected(gnomonActorImage *image);
    void selected(gnomonInspectorVolume *volume);
    void selected(gnomonInspectorImage *image);
    void selected(gnomonCellComplex *complex);
    void selected(gnomonActorMeshCellGraph *cellgraph);
    void selected(gnomonInspectorCellGraph *cellgraph);

private slots:
   void onItemClicked(QTreeWidgetItem *item, int column);
   void onItemSelected(void);

private:
   gnomonInspectorViewTreePrivate *d;
};

//
// gnomonInspectorViewTree.h ends here
