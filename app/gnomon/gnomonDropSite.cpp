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

#include "gnomonDropSite.h"

class gnomonDropSitePrivate
{
public:
    QLabel *label;

public:
    int index;
};

gnomonDropSite::gnomonDropSite(QWidget *parent) : QFrame(parent)
{
    d = new gnomonDropSitePrivate;
    d->label = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(d->label);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

gnomonDropSite::~gnomonDropSite(void)
{
    delete d;
}

QSize gnomonDropSite::sizeHint(void) const
{
    return QSize(120, 120);
}

//
// gnomonDropSite.cpp ends here
