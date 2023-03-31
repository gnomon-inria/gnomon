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

#include "gnomonVtkViewPool.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonVtkViewPoolPrivate : public QObject
{
    Q_OBJECT

//public slots:
//    void   linking(void);
//    void unlinking(void);

public:
    QList<gnomonVtkView *> views;

    bool linked;

//public:
//    gnomonVtkView *source = nullptr;
//    gnomonVtkView *target = nullptr;
};

//void gnomonVtkViewPoolPrivate::linking(void)
//{
//    if(!this->source) {
//         this->source = dynamic_cast<gnomonVtkView *>(sender());
//         return;
//    }
//
//    if(!this->target) {
//         this->target = dynamic_cast<gnomonVtkView *>(sender());
//    }
//
//    if (this->source && this->target) {
//        this->source->link(this->target);
//        this->target->link(this->source);
//    }
//}
//
//void gnomonVtkViewPoolPrivate::unlinking(void)
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

gnomonVtkViewPool::gnomonVtkViewPool(QObject *parent) : QObject(parent)
{
    d = new gnomonVtkViewPoolPrivate;
}

gnomonVtkViewPool::~gnomonVtkViewPool(void)
{
    foreach(gnomonVtkView *view, d->views) {
        view->setInPool(false);
    }
    d->views.clear();
}

void gnomonVtkViewPool::addView(gnomonVtkView *view)
{
    if(!d->views.contains(view)) {
        d->views << view;
        view->setInPool(true);
//
//        connect(view, SIGNAL(  linking()), d, SLOT(  linking()));
//        connect(view, SIGNAL(unlinking()), d, SLOT(unlinking()));
        connect(view, SIGNAL(  linking()), this, SLOT(  linkAll()));
        connect(view, SIGNAL(unlinking()), this, SLOT(unlinkAll()));
    }
}

void gnomonVtkViewPool::linkAll(void)
{
    foreach(gnomonVtkView *source, d->views) {
        foreach(gnomonVtkView *target, d->views) {
            if (source != target) {
                source->link(target);
            }
        }
    }
    d->linked = true;
}

void gnomonVtkViewPool::unlinkAll(void)
{
    foreach(gnomonVtkView *source, d->views) {
        foreach(gnomonVtkView *target, d->views) {
            if (source != target) {
                source->unlink(target);
            }
        }
    }
    d->linked = false;
}

bool gnomonVtkViewPool::isLinked(void)
{
    return d->linked;
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonVtkViewPool.moc"

//
// gnomonVtkViewPool.cpp ends here
