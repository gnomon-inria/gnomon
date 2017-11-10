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

#include <QTreeWidget>

class vtkPolyData;
class vtkImageData;
class gnomonCellComplex;
class gnomonCellGraph;

class gnomonView;
class gnomonActor;
class gnomonActorVolume;
class gnomonActorMeshCellGraph;

class gnomonInspectorViewTreePrivate;
class gnomonInspectorImage;
class gnomonInspectorCellGraph;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewTree : public QTreeWidget {
    Q_OBJECT

public:
     gnomonInspectorViewTree(QWidget *parent = 0);
    ~gnomonInspectorViewTree(void);

    void setView(gnomonView *view);

public:
    void insert(vtkPolyData *mesh);

    QTreeWidgetItem *insert(gnomonActorVolume *volume_actor);
    QTreeWidgetItem *addChild(QTreeWidgetItem *parent, gnomonInspectorImage *inspector_image);

    void insert(gnomonCellComplex *complex);

    QTreeWidgetItem *insert(gnomonActorMeshCellGraph *cellgraph_actor);
    QTreeWidgetItem *addChild(QTreeWidgetItem *parent, gnomonInspectorCellGraph *inspector_cellgraph);


signals:
    void checked(gnomonActor *, bool);

    void selected(vtkPolyData *mesh);
    void selected(gnomonActorVolume *volume);
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
