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

#include "gnomonInspectorClipPlanes.h"
#include "gnomonClutEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorClipPlanesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorClipPlanesPrivate
{
public:
    gnomonClutEditor *editor;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorClipPlanes
// /////////////////////////////////////////////////////////////////

gnomonInspectorClipPlanes::gnomonInspectorClipPlanes(void) : gnomonInspectorImage(), d(new gnomonInspectorClipPlanesPrivate)
{
    d->editor = new gnomonClutEditor();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->editor);
    this->setLayout(layout);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorClipPlanes::~gnomonInspectorClipPlanes(void)
{
    delete d->editor;
    delete d;

    d = NULL;
}

gnomonClutEditor *gnomonInspectorClipPlanes::editor(void) const
{
    return d->editor;
}

//
// gnomonInspectorClipPlanes.cpp ends here
