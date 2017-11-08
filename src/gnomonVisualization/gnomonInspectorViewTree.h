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

#include <gnomonVisualizationExport.h>

#include <QTreeWidget>

class vtkPolyData;
class vtkImageData;
class gnomonView;

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

public slots:
    // void insert(vtkImageData *);

 private:
    gnomonInspectorViewTreePrivate *d;
};

//
// gnomonInspectorViewTree.h ends here
