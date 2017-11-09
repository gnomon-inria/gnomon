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
#include "gnomonClutEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolumePrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorVolumePrivate
{
public:
    gnomonClutEditor *editor;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorVolume
// /////////////////////////////////////////////////////////////////

gnomonInspectorVolume::gnomonInspectorVolume(void) : gnomonInspector(), d(new gnomonInspectorVolumePrivate)
{
    d->editor = new gnomonClutEditor(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->editor);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorVolume::~gnomonInspectorVolume(void)
{
    delete d;

    d = NULL;
}

//
// gnomonInspectorVolume.cpp ends here
