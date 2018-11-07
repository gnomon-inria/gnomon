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
    int index = -1;
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

int gnomonDropSite::index(void)
{
    return d->index;
}

void gnomonDropSite::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->accept();
        return;
    }

    event->ignore();
}

void gnomonDropSite::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void gnomonDropSite::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void gnomonDropSite::dropEvent(QDropEvent *event)
{
    QString path = event->mimeData()->text();

    if(!path.startsWith(":"))
        return;

    d->index = path.remove(":").toInt();

    d->label->setPixmap(*dynamic_cast<QLabel *>(event->source())->pixmap());
}

//
// gnomonDropSite.cpp ends here
