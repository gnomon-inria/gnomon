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

class gnomonInspectorViewTreePrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewTree : public QTreeWidget {
    Q_OBJECT

public:
     gnomonInspectorViewTree(QWidget *parent = 0);
    ~gnomonInspectorViewTree(void);

    void setView(gnomonView *view);

public:
    void insert(vtkPolyData *mesh);
    void insert(vtkImageData *volume);
    void insert(gnomonCellComplex *complex);
    void insert(gnomonCellGraph *graph);

signals:
    void checked(gnomonActor *, bool);

    void selected(vtkPolyData *mesh);
    void selected(vtkImageData *volume);
    void selected(gnomonCellComplex *complex);
    void selected(gnomonCellGraph *graph);
=======
>>>>>>> develop

private slots:
   void onItemClicked(QTreeWidgetItem *item, int column);
   void onItemSelected(void);

private:
   gnomonInspectorViewTreePrivate *d;
};

//
// gnomonInspectorViewTree.h ends here
