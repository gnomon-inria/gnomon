#include "gnomonInspectorViewWidget.h"

#include "gnomonActor.h"
#include "gnomonActorMesh.h"
#include "gnomonActorVolume.h"

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewWidgetPrivate
{
public:
    QHash<gnomonActor *, QWidget *> widgets;

public:
    QWidget *dummy;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget() : d(new gnomonInspectorViewWidgetPrivate){
    d->dummy = new QWidget(this);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget()
{
    delete d->dummy;
    delete d;
}

void gnomonInspectorViewWidget::setActor(gnomonActor *actor, bool enabled)
{
    if(actor == nullptr) {
        return;
    }

    if(d->widgets.keys().contains(actor)) {
        this->setCurrentWidget(d->widgets.value(actor));
        d->widgets.value(actor)->setEnabled(enabled);
        return;
    }

    QWidget *widget = nullptr;

    if(gnomonActorMesh *mesh_actor = dynamic_cast<gnomonActorMesh *>(actor)) {

    }

    if(gnomonActor *volume_actor = dynamic_cast<gnomonActorVolume *>(actor)) {

    }
}
