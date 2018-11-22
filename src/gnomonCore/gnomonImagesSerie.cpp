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

#include "gnomonImagesSerie.h"

class gnomonImagesSeriePrivate
{
public:
             gnomonImagesSeriePrivate();
    virtual ~gnomonImagesSeriePrivate();

public:
    size_t time;
    QString channel;
    QStringList channels;

public:
    QVector<QMap<QString, dtkImage*>> images;
};

gnomonImagesSeriePrivate::gnomonImagesSeriePrivate()
{
    time = 0;
    channel = "";
    images.clear();
}

gnomonImagesSeriePrivate::~gnomonImagesSeriePrivate()
{
    images.clear();
}

gnomonImagesSerie::gnomonImagesSerie() : d(new gnomonImagesSeriePrivate)
{
}

gnomonImagesSerie::~gnomonImagesSerie()
{
    delete d;
}

dtkImage* gnomonImagesSerie::image() const
{
    if(d->time >= d->images.size())
    {
        qDebug() << "gnomonImagesSerie has no time" << d->time;
        return nullptr;
    }
    const QMap<QString, dtkImage*>& images = d->images[d->time];
    QMap<QString, dtkImage*>::const_iterator it = images.constFind(d->channel);
    if(it == images.constEnd())
    {
        qDebug() << "gnomonImagesSerie at time" << d->time << "has no" << d->channel << "channel";
        return nullptr;
    }
    return it.value();
}

void gnomonImagesSerie::setImage(dtkImage* image)
{
    if(d->time >= d->images.size())
        d->images.resize(d->time + 1);
    QMap<QString, dtkImage*>& images = d->images[d->time];
    QMap<QString, dtkImage*>::iterator it = images.find(d->channel);
    if(it == images.end())
        images.insert(d->channel, image);
    else
        it.value() = image;
    if(!d->channels.contains(d->channel))
        d->channels.append(d->channel);
}

size_t gnomonImagesSerie::time(void) const
{
    return d->time;
}

void gnomonImagesSerie::setTime(size_t time)
{
    d->time = time;
}

size_t gnomonImagesSerie::times(void) const
{
    return d->images.size();
}

const QString& gnomonImagesSerie::channel(void) const
{
    return d->channel;
}

void gnomonImagesSerie::setChannel(const QString& channel)
{
    d->channel = channel;
}

const QStringList& gnomonImagesSerie::channels(void) const
{
    return d->channels;
}

// gnomonImagesSerie.cpp ends here