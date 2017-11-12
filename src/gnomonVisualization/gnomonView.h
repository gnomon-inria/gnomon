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
class gnomonCellImage;

class gnomonViewManager;
class gnomonViewPrivate;

class gnomonInspector;

class gnomonActorImage;
class gnomonActorVolume;
class gnomonActorScalarBar;

class GNOMONVISUALIZATION_EXPORT gnomonView : public dtkViewWidget
{
    Q_OBJECT

public:
     gnomonView(QWidget *parent = Q_NULLPTR);
    ~gnomonView(void);

signals:
    void focused(void);

public:
    void addCellComplex(gnomonCellComplex&);
    void addCellGraph(gnomonCellGraph&);

public:
    gnomonViewManager *manager(void);

public:
    QWidget *widget(void) override;
    QWidget *inspector(void) override;

public slots:
    void onInserted(gnomonActorImage *image);
    void onInserted(gnomonActorVolume *volume);
    void onInserted(gnomonActorScalarBar *scalar_bar);
    void onInserted(vtkImageData *image);
    void onInserted(vtkPolyData *mesh);
    void onInserted(gnomonCellComplex *cellcomplex);
    void onInserted(gnomonCellGraph *cellgraph);
    void onInserted(gnomonCellImage *cellimage);

public slots:
    void onInspectorSelected(QWidget *inspector);

protected:
    void mousePressEvent(QMouseEvent *) override;

private:
    gnomonViewPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonView *)

//
// gnomonView.h ends here
