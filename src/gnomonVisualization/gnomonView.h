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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <dtkCore>
#include <dtkWidgets>

#include <gnomonVisualizationExport.h>

class vtkImageData;
class vtkPolyData;

class gnomonCellComplex;
class gnomonCellGraph;

class gnomonViewManager;
class gnomonViewPrivate;

class gnomonInspector;

class GNOMONVISUALIZATION_EXPORT gnomonView : public dtkViewWidget
{
    Q_OBJECT

public:
     gnomonView(QWidget *parent = Q_NULLPTR);
    ~gnomonView(void);

public:
    void addCellComplex(gnomonCellComplex&);
    void addCellGraph(gnomonCellGraph&);

public:
    gnomonViewManager *manager(void);

public:
    QWidget *widget(void) override;
    QWidget *inspector(void) override;

public slots:
    void onInserted(vtkImageData *image);
    void onInserted(vtkPolyData *mesh);
    void onInserted(gnomonCellComplex *cellcomplex);
    void onInserted(gnomonCellGraph *cellgraph);

    void onInspectorSelected(QWidget *inspector);

private:
    gnomonViewPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonView *)

//
// gnomonView.h ends here
