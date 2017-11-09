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
    qWarning() << Q_FUNC_INFO;
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

gnomonInspectorVolume::~gnomonInspectorVolume(void)
{
    delete d->editor;
    delete d;

    d = NULL;
}

gnomonClutEditor *gnomonInspectorVolume::editor(void) const
{
    return d->editor;
}

//
// gnomonInspectorVolume.cpp ends here
