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

// /////////////////////////////////////////////////////////////////
// gnomonInspectorImagePropertiesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorImagePropertiesPrivate
{
public:

};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorImageProperties
// /////////////////////////////////////////////////////////////////

gnomonInspectorImageProperties::gnomonInspectorImageProperties(QWidget *parent) : gnomonInspectorImage(parent), d(new gnomonInspectorImagePropertiesPrivate)
{
    // QVBoxLayout *layout = new QVBoxLayout;
    // layout->setContentsMargins(0, 0, 0, 0);
    // layout->setSpacing(0);

    // this->setLayout(layout);
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
