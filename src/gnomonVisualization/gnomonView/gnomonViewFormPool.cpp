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

#include "gnomonViewFormPool.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewFormPoolPrivate : public QObject
{
    Q_OBJECT

//public slots:
//    void   linking(void);
//    void unlinking(void);

public:
    QList<gnomonViewForm *> views;

    bool linked;

//public:
//    gnomonViewForm *source = nullptr;
//    gnomonViewForm *target = nullptr;
};

//void gnomonViewFormPoolPrivate::linking(void)
//{
//    if(!this->source) {
//         this->source = dynamic_cast<gnomonViewForm *>(sender());
//         return;
//    }
//
//    if(!this->target) {
//         this->target = dynamic_cast<gnomonViewForm *>(sender());
//    }
//
//    if (this->source && this->target) {
//        this->source->link(this->target);
//        this->target->link(this->source);
//    }
//}
//
//void gnomonViewFormPoolPrivate::unlinking(void)
//{
//    if (this->source)
//        this->source->unlink(this->target);
//
//    if (this->target)
//        this->target->unlink(this->source);
//
//    this->source = nullptr;
//    this->target = nullptr;
//}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewFormPool::gnomonViewFormPool(QObject *parent) : QObject(parent)
{
    d = new gnomonViewFormPoolPrivate;
}

gnomonViewFormPool::~gnomonViewFormPool(void)
{
    d->views.clear();
}

void gnomonViewFormPool::addView(gnomonViewForm *view)
{
    if(!d->views.contains(view)) {
        d->views << view;
//
//        connect(view, SIGNAL(  linking()), d, SLOT(  linking()));
//        connect(view, SIGNAL(unlinking()), d, SLOT(unlinking()));
        connect(view, SIGNAL(  linking()), this, SLOT(  linkAll()));
        connect(view, SIGNAL(unlinking()), this, SLOT(unlinkAll()));
    }
}

void gnomonViewFormPool::linkAll(void)
{
    foreach(gnomonViewForm *source, d->views) {
        foreach(gnomonViewForm *target, d->views) {
            if (source != target) {
                source->link(target);
            }
        }
    }
    d->linked = true;
}

void gnomonViewFormPool::unlinkAll(void)
{
    foreach(gnomonViewForm *source, d->views) {
        foreach(gnomonViewForm *target, d->views) {
            if (source != target) {
                source->unlink(target);
            }
        }
    }
    d->linked = false;
}

bool gnomonViewFormPool::isLinked(void)
{
    return d->linked;
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewFormPool.moc"

//
// gnomonViewFormPool.cpp ends here
