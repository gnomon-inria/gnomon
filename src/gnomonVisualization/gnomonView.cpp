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

#include "gnomonActor.h"
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"
#include "gnomonActorMeshCellImage.h"
#include "gnomonActorVolume.h"
#include "gnomonInspectorViewTree.h"
#include "gnomonInspector.h"
#include "gnomonView.h"
#include "gnomonViewManager.h"

#include <gnomonStyle>

#include <dtkWidgets>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkImageData.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLWidget.h>

// ///////////////////////////////////////////////////////////////////
// gnomonViewWidget
// ///////////////////////////////////////////////////////////////////

class gnomonViewWidget : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    gnomonViewWidget(QWidget *parent = Q_NULLPTR) : QVTKOpenGLWidget(parent)
    {

    }

    ~gnomonViewWidget(void)
    {

    }

signals:
    void focused(void);

private:
    void mousePressEvent(QMouseEvent *event)
    {
        QVTKOpenGLWidget::mousePressEvent(event);

        emit focused();
    }
};

// ///////////////////////////////////////////////////////////////////
// gnomonViewPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    vtkRenderer *renderer;
    gnomonViewWidget *widget;

public:
    gnomonViewManager *manager;
};

// ///////////////////////////////////////////////////////////////////
// gnomonView
// ///////////////////////////////////////////////////////////////////

gnomonView::gnomonView(QWidget *parent) : dtkViewWidget(parent)
{
    QColor background_color = QColor(GNOMON_STYLE_BACKGROUNDCOLOR);

    d = new gnomonViewPrivate;

    d->manager = new gnomonViewManager();

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    d->window = vtkGenericOpenGLRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new gnomonViewWidget(this);
    d->widget->SetRenderWindow(d->window);
    d->widget->setEnableHiDPI(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);

    connect(d->widget, SIGNAL(focused()), this, SIGNAL(focused()));

    connect(d->manager, SIGNAL(inserted(gnomonActor *)), this, SLOT(onInserted(gnomonActor *)));
}

gnomonView::~gnomonView(void)
{
    d->renderer->Delete();
    d->window->Delete();

    delete d->manager;
    delete d;
}

gnomonViewManager *gnomonView::manager(void)
{
    return d->manager;
}

QWidget *gnomonView::widget(void)
{
    return d->widget;
}

QWidget *gnomonView::inspector(void)
{
    return d->manager->inspector();
}

void gnomonView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO;

    gnomonView::mousePressEvent(event);

    emit focused();
}

void gnomonView::onInserted(gnomonActor *actor)
{
    actor->setInteractor(d->widget->GetInteractor());
    actor->update();
    d->renderer->AddActor(actor);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonView.moc"

//
// gnomonView.cpp ends here
