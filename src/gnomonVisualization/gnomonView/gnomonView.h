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
#include <dtkVisualization>

#include <gnomonVisualizationExport.h>

class gnomonActor;
class gnomonInspector;
class gnomonViewManager;
class gnomonViewPrivate;

class GNOMONVISUALIZATION_EXPORT gnomonView : public dtkVisualizationView3D
{
    Q_OBJECT

public:
     gnomonView(QWidget *parent = Q_NULLPTR);
    ~gnomonView(void);

signals:
    void focused(void);

public:
    gnomonViewManager *manager(void);

public:
    QWidget *widget(void) override;
    QWidget *inspector(void) override;

    class vtkRenderer *renderer(void);
    class vtkRenderWindowInteractor *interactor(void);

protected:
    void mousePressEvent(QMouseEvent *) override;

private slots:
    void onInserted(gnomonActor *);

private:
    gnomonViewPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonView *)

//
// gnomonView.h ends here
