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

#include "gnomonInspectorViewWidget.h"

#include "gnomonActor.h"
#include "gnomonActorMesh.h"
#include "gnomonActorVolume.h"

#include "gnomonInspectorVolume.h"
#include "gnomonInspectorMesh.h"
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"

#include "gnomonClutEditor.h"

#include <QStackedWidget>

#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkImageAccumulate.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewWidgetPrivate
{
public:
    QHash<gnomonActor *, QWidget *> widgets;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorViewWidgetPrivate)
{
    this->setWidgetResizable(true);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget()
{
    delete d;
}

void gnomonInspectorViewWidget::setActor(gnomonActor *actor, bool enabled)
{
    if(actor == nullptr) {
        return;
    }

    if(d->widgets.keys().contains(actor)) {
        this->setWidget(d->widgets.value(actor));
        d->widgets.value(actor)->setEnabled(enabled);
        return;
    }

    QWidget *widget = nullptr;

    if (gnomonActorMesh *mesh_actor = dynamic_cast<gnomonActorMesh *>(actor)) {
        QWidget *mesh_inspector = new gnomonInspectorMesh();
        d->widgets.insert(mesh_actor, mesh_inspector);
        this->setWidget(mesh_inspector);
        mesh_inspector->setEnabled(enabled);
        return;
    }

    if (gnomonActorMeshCellComplex *complex_actor = dynamic_cast<gnomonActorMeshCellComplex *>(actor)) {
        QWidget *complex_inspector = new gnomonInspectorMesh();
        d->widgets.insert(complex_actor, complex_inspector);
        d->stacked_widget->addWidget(complex_inspector);
        d->stacked_widget->setCurrentWidget(complex_inspector);
        complex_inspector->setEnabled(enabled);
        return;
    }

    if (gnomonActorMeshCellGraph *graph_actor = dynamic_cast<gnomonActorMeshCellGraph *>(actor)) {
        QWidget *graph_inspector = new gnomonInspectorMesh();
        d->widgets.insert(graph_actor, graph_inspector);
        d->stacked_widget->addWidget(graph_inspector);
        d->stacked_widget->setCurrentWidget(graph_inspector);
        graph_inspector->setEnabled(enabled);
        return;
    }

    if (gnomonActorVolume *volume_actor = dynamic_cast<gnomonActorVolume *>(actor)) {

        gnomonInspectorVolume *volume_inspector = new gnomonInspectorVolume();

        volume_inspector->editor()->setHistogram(volume_actor->histogram());
        volume_inspector->editor()->setRange(volume_actor->rangeMin(), volume_actor->rangeMax());
        connect(volume_inspector->editor(), &gnomonClutEditor::updated, [=] () {
                volume_actor->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
                volume_actor->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(volume_inspector->editor()->opacityTransferFunction()));
        });

        d->widgets.insert(volume_actor, volume_inspector);

        this->setWidget(volume_inspector);
        volume_inspector->setEnabled(enabled);
        return;
    }
}

//
// gnomonInspectorViewWidget.cpp ends here
