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

#include <dtkImagingCore>

class gnomonImagesSeriePrivate
{
public:
             gnomonImagesSeriePrivate();
             gnomonImagesSeriePrivate(const gnomonImagesSeriePrivate&);
    virtual ~gnomonImagesSeriePrivate();

public:
    QString channel;
    size_t time;
    QStringList channels;

public:
    QMap< size_t, QMap<QString, dtkImage*>> images;
};

gnomonImagesSeriePrivate::gnomonImagesSeriePrivate()
{
    this->time = 0;
    this->channel = "";
    this->images.clear();
}

gnomonImagesSeriePrivate::gnomonImagesSeriePrivate(const gnomonImagesSeriePrivate& d)
{
    this->time = d.time;
    this->channel = d.channel;

    auto i = d.images.constBegin();
    while (i != d.images.constEnd()) {
        this->images[i.key()] = QMap<QString, dtkImage*>();
        for (QMap<QString, dtkImage*>::const_iterator it = d.images[i.key()].cbegin(),
                 it_end = d.images[i.key()].cend(); it != it_end; ++it)
        {
            this->images[i.key()].insert(it.key(), new dtkImage(*it.value()));
            this->channels << it.key();
        }
        ++i;
    }

    this->channels.removeDuplicates();
}

gnomonImagesSeriePrivate::~gnomonImagesSeriePrivate()
{
    for (auto& image : this->images)
    {
        qDeleteAll(image.begin(), image.end());
        image.clear();
    }
    images.clear();
}

gnomonImagesSerie::gnomonImagesSerie() : d(new gnomonImagesSeriePrivate)
{
}

gnomonImagesSerie::gnomonImagesSerie(const gnomonImagesSerie& images_serie) : d(new gnomonImagesSeriePrivate(*images_serie.d))
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


void gnomonImagesSerie::setImage(dtkImage* image, size_t time)
{
    this->setImage(image, d->channel, time);
}

void gnomonImagesSerie::setImage(dtkImage* image, const QString& channel, size_t time)
{
    QString current_channel = channel;
    if(channel.isEmpty())
        current_channel = d->channel;

    if(time == SIZE_MAX)
        time = d->time;

    if(!d->images.contains(time)) {
        d->images[time] = QMap<QString, dtkImage*>();
    }


    QMap<QString, dtkImage*>& images = d->images[time];
    QMap<QString, dtkImage*>::iterator it = images.find(current_channel);
    if (it == images.end()) {
        images.insert(current_channel, image);
    }
    else {
        delete it.value();
        it.value() = image;
    }
    if(!d->channels.contains(current_channel))
        d->channels.append(current_channel);
}

size_t gnomonImagesSerie::time(void) const
{
    return d->time;
}

void gnomonImagesSerie::setTime(size_t time)
{
    d->time = time;
}

QList<size_t> gnomonImagesSerie::times(void) const
{
    return d->images.keys();
}

QString gnomonImagesSerie::channel(void) const
{
    return d->channel;
}

void gnomonImagesSerie::setChannel(const QString& channel)
{
    d->channel = channel;
}

QStringList gnomonImagesSerie::channels(void) const
{
    return d->channels;
}

gnomonImagesSerie* gnomonImagesSerie::copy() const
{ return new gnomonImagesSerie(*this); }

// gnomonImagesSerie.cpp ends here
