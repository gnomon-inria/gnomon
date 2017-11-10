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

signals:
    void checked(gnomonActor *, bool);

    void selected(vtkPolyData *mesh);
    void selected(vtkImageData *volume);

private slots:
   void onItemClicked(QTreeWidgetItem *item, int column);
   void onItemSelected(void);

private:
   gnomonInspectorViewTreePrivate *d;
};

//
// gnomonInspectorViewTree.h ends here
