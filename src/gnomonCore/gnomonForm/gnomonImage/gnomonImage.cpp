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

#include "gnomonImage.h"

#include <dtkImagingCore>

class gnomonImagePrivate
{
public:
             gnomonImagePrivate();
             gnomonImagePrivate(const gnomonImagePrivate&);
    virtual ~gnomonImagePrivate();

public:
    QMap<QString, dtkImage *> images;
};

gnomonImagePrivate::gnomonImagePrivate()
{
    this->images.clear();
}

gnomonImagePrivate::gnomonImagePrivate(const gnomonImagePrivate& d)
{
    this->images.clear();
    for (QMap<QString, dtkImage *>::const_iterator it = d.images.cbegin(), it_end = d.images.cend(); it != it_end; ++it) {
        this->images[it.key()] = new dtkImage(*(it.value()));
    }
}

gnomonImagePrivate::~gnomonImagePrivate()
{
    qDeleteAll(this->images);
    images.clear();
}

gnomonImage::gnomonImage() : d(new gnomonImagePrivate)
{
}

gnomonImage::gnomonImage(const gnomonImage& dynamic_form) : d(new gnomonImagePrivate(*dynamic_form.d))
{
}

gnomonImage::~gnomonImage()
{
    delete d;
}

dtkImage * gnomonImage::image(QString channel) const
{
    QMap<QString, dtkImage *>::const_iterator it = d->images.constFind(channel);
    if (it == d->images.constEnd()) {
        return nullptr;
    }

    return it.value();
}

void gnomonImage::setImage(dtkImage * image, QString channel)
{
    QMap<QString, dtkImage *>::iterator it = d->images.find(channel);
    if (it == d->images.end()) {
        d->images.insert(channel, image);
    }
    else {
        delete it.value();
        it.value() = image;
    }
}

QStringList gnomonImage::channels(void) const
{
    return d->images.keys();
}

gnomonAbstractForm * gnomonImage::clone() const
{ return new gnomonImage(*this); }

// gnomonImage.cpp ends here
