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

class gnomonActor;

class gnomonActorMeshCellComplex;
class gnomonActorMeshCellGraph;
class gnomonActorMeshCellImage;
class gnomonActorVolume;

class gnomonView;

class gnomonInspectorViewTreePrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewTree : public QTreeWidget
{
    Q_OBJECT

public:
     gnomonInspectorViewTree(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorViewTree(void);

public:
    QTreeWidgetItem *insert(gnomonActorMeshCellComplex *);
    QTreeWidgetItem *insert(gnomonActorMeshCellGraph *);
    QTreeWidgetItem *insert(gnomonActorMeshCellImage *);
    QTreeWidgetItem *insert(gnomonActorVolume *);

signals:
    void checked(gnomonActor *, bool);

    void selected(gnomonActorVolume *);
    void selected(gnomonActorMeshCellComplex *);
    void selected(gnomonActorMeshCellGraph *);
    void selected(gnomonActorMeshCellImage *);

private slots:
   void onItemClicked(QTreeWidgetItem *item, int column);
   void onItemSelected(void);

private:
   gnomonInspectorViewTreePrivate *d;
};

//
// gnomonInspectorViewTree.h ends here
