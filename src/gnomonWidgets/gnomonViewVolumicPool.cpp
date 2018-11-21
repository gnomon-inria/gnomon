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

#include "gnomonViewVolumicPool.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicPoolPrivate : public QObject
{
    Q_OBJECT

public slots:
    void   linking(void);
    void unlinking(void);

public:
    QList<gnomonViewVolumic *> views;

public:
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;
};

void gnomonViewVolumicPoolPrivate::linking(void)
{
    if(!this->source) {
         this->source = dynamic_cast<gnomonViewVolumic *>(sender());
         return;
    }

    if(!this->target) {
         this->target = dynamic_cast<gnomonViewVolumic *>(sender());
    }

    if (this->source && this->target) {
        this->source->link(this->target);
        this->target->link(this->source);
    }
}

void gnomonViewVolumicPoolPrivate::unlinking(void)
{
    if (this->source == dynamic_cast<gnomonViewVolumic *>(sender())) {
        this->source->unlink();
        this->source = nullptr;
    }

    if (this->target == dynamic_cast<gnomonViewVolumic *>(sender()))
        this->target->unlink();
        this->target = nullptr;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumicPool::gnomonViewVolumicPool(QObject *parent) : QObject(parent)
{
    d = new gnomonViewVolumicPoolPrivate;
}

gnomonViewVolumicPool::~gnomonViewVolumicPool(void)
{
    d->views.clear();
}

void gnomonViewVolumicPool::addView(gnomonViewVolumic *view)
{
    d->views << view;

    connect(view, SIGNAL(  linking()), d, SLOT(  linking()));
    connect(view, SIGNAL(unlinking()), d, SLOT(unlinking()));
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewVolumicPool.moc"

//
// gnomonViewVolumicPool.cpp ends here
