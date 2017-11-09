#include "gnomonInspectorViewWidget.h"

#include "gnomonActor.h"
#include "gnomonActorMesh.h"
#include "gnomonActorVolume.h"

#include "gnomonInspectorVolume.h"

#include "gnomonClutEditor.h"

#include <QStackedWidget>

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkSmartPointer.h"
#include "vtkImageAccumulate.h"

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewWidgetPrivate
{
public:
    QHash<gnomonActor *, QWidget *> widgets;
    QStackedWidget *stacked_widget;

public:
    QWidget *dummy;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorViewWidgetPrivate)
{
    d->dummy = new QWidget(this);
    d->stacked_widget = new QStackedWidget(this);
    d->stacked_widget->setMinimumSize(800, 145);

    this->setWidgetResizable(true);
    this->setWidget(d->stacked_widget);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget()
{
    delete d->stacked_widget;
    delete d->dummy;
    delete d;
}

void gnomonInspectorViewWidget::setActor(gnomonActor *actor, bool enabled)
{
    if(actor == nullptr) {
        return;
    }

    if(d->widgets.keys().contains(actor)) {
        d->stacked_widget->setCurrentWidget(d->widgets.value(actor));
        d->widgets.value(actor)->setEnabled(enabled);
        return;
    }

    QWidget *widget = nullptr;

    if (gnomonActorMesh *mesh_actor = dynamic_cast<gnomonActorMesh *>(actor)) {
        QWidget *mesh_inspector = new gnomonInspectorVolume();
        d->widgets.insert(mesh_actor, mesh_inspector);
        d->stacked_widget->addWidget(mesh_inspector);
        d->stacked_widget->setCurrentWidget(mesh_inspector);
        mesh_inspector->setEnabled(enabled);
        return;
    }

    if (gnomonActorVolume *volume_actor = dynamic_cast<gnomonActorVolume *>(actor)) {
        gnomonInspectorVolume *volume_inspector = new gnomonInspectorVolume();

        volume_inspector->editor()->setHistogram(volume_actor->histogram());
        volume_inspector->editor()->setRange(volume_actor->rangeMin(), volume_actor->rangeMax());
        connect(volume_inspector->editor(),
                &gnomonClutEditor::updated,
                [=]()
                {
                    volume_actor->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(volume_inspector->editor()->colorTransferFunction()));
                    volume_actor->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(volume_inspector->editor()->opacityTransferFunction()));

                });

        d->widgets.insert(volume_actor, volume_inspector);

        d->stacked_widget->addWidget(volume_inspector);
        d->stacked_widget->setCurrentWidget(volume_inspector);
        volume_inspector->setEnabled(enabled);
        return;
    }
}
