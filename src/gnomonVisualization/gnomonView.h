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
    QWidget *widget(void);
    QWidget *inspector(void);

public slots:
    void onInserted(vtkImageData *image);

private:
    gnomonViewPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonView *)

//
// gnomonView.h ends here
