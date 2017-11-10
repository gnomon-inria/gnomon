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

#include "gnomonInspectorImageProperties.h"
#include "gnomonClutEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorImagePropertiesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorImagePropertiesPrivate
{
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorImageProperties
// /////////////////////////////////////////////////////////////////

gnomonInspectorImageProperties::gnomonInspectorImageProperties(void) : gnomonInspectorImage(), d(new gnomonInspectorImagePropertiesPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // layout->addWidget(d->editor);
    this->setLayout(layout);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorImageProperties::~gnomonInspectorImageProperties(void)
{
    delete d;

    d = NULL;
}

//
// gnomonInspectorImageProperties.cpp ends here
