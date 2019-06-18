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

#include "gnomonInspectorVolume.h"

#include "gnomonActor/gnomonActorLegacy/gnomonActorVolume.h"

#include <gnomonWidgets>

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorVolume;

class gnomonInspectorVolumePrivate
{
public:
    gnomonClutEditor *editor;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolume
// /////////////////////////////////////////////////////////////////

gnomonInspectorVolume::gnomonInspectorVolume(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorVolumePrivate)
{
    d->editor = new gnomonClutEditor(this);

    this->setAlignment(Qt::AlignTop);
    this->setFrameShape(QFrame::NoFrame);
    this->setWidgetResizable(true);
    this->setWidget(d->editor);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorVolume::~gnomonInspectorVolume(void)
{
    delete d;

    d = NULL;
}

gnomonClutEditor *gnomonInspectorVolume::editor(void) const
{
    return d->editor;
}

void gnomonInspectorVolume::setActor(gnomonActorVolume *actor) const
{
    d->editor->setRange(actor->rangeMin(), actor->rangeMax());
    d->editor->setHistogram(actor->histogram());
    d->editor->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(actor->opacityTransferFunction()));
    d->editor->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(actor->colorTransferFunction()));
    connect(d->editor, &gnomonClutEditor::updated, [=] () {
        actor->setColorTransferFunction(static_cast<vtkColorTransferFunction *>(d->editor->colorTransferFunction()));
        actor->setOpacityTransferFunction(static_cast<vtkPiecewiseFunction *>(d->editor->opacityTransferFunction()));
    });
}
//
// gnomonInspectorVolume.cpp ends here
