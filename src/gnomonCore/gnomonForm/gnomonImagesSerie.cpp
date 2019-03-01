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

#include "gnomonForm/gnomonImagesSerie.h"

#include <dtkImagingCore>
#include <vtkImageData.h>

class gnomonImagesSeriePrivate
{
public:
             gnomonImagesSeriePrivate();
             gnomonImagesSeriePrivate(const gnomonImagesSeriePrivate&);
    virtual ~gnomonImagesSeriePrivate();

public:
    QString channel;
    unsigned long time;
    QStringList channels;

public:
    QMap< unsigned long, QMap<QString, dtkImage*>> images;
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

QMap<QString,QString> gnomonImagesSerie::metadata(void) const
{
    dtkImage *image = this->image();
    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(image);
    converter->convert();

    vtkImageData *image_data = static_cast<vtkImageData *>(converter->output());

    delete converter;

    QMap<QString,QString> metadata;

    qDebug()<<Q_FUNC_INFO<<d->channels.size();
    metadata["Number of channels"] = QString::number(d->channels.size());
    if (d->channels.size()>1) {
        int i_channel = 0;
        for (const auto& channel : d->channels) {
            metadata["Channel "+QString::number(i_channel)] = channel;
            i_channel++;
        }
    }
    metadata["Dimensions"] = "("+QString::number(image_data->GetDimensions()[0])+", "+QString::number(image_data->GetDimensions()[1])+","+QString::number(image_data->GetDimensions()[2])+")";
    metadata["Voxel Type"] = QString(QVariant::typeToName(image->storageType()));
    metadata["Voxel Size"] = "("+QString::number(image_data->GetSpacing()[0])+", "+QString::number(image_data->GetSpacing()[1])+","+QString::number(image_data->GetSpacing()[2])+")";

    return metadata;
}


dtkImage* gnomonImagesSerie::image(unsigned long time) const
{
    return this->image(d->channel, time);
}

dtkImage* gnomonImagesSerie::image() const
{
    return this->image(d->channel, d->time);
}

dtkImage* gnomonImagesSerie::image(const QString& channel, unsigned long time) const
{
    if(time == SIZE_MAX)
        time = d->time;

    if(!d->images.contains(time))
    {
        qDebug() << "gnomonImagesSerie has no time" << time;
        return nullptr;
    }

    QString current_channel = channel;
    if(channel.isEmpty())
        current_channel = d->channel;

    const QMap<QString, dtkImage*>& images = d->images[time];
    QMap<QString, dtkImage*>::const_iterator it = images.constFind(current_channel);
    if(it == images.constEnd())
    {
        qDebug() << "gnomonImagesSerie at time" << time << "has no" << current_channel << "channel";
        return nullptr;
    }

    return it.value();
}


void gnomonImagesSerie::setImage(dtkImage* image, unsigned long time)
{
    this->setImage(image, d->channel, time);
}

void gnomonImagesSerie::setImage(dtkImage* image, const QString& channel, unsigned long time)
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

unsigned long gnomonImagesSerie::time(void) const
{
    return d->time;
}

void gnomonImagesSerie::setTime(unsigned long time)
{
    d->time = time;
}

QList<unsigned long> gnomonImagesSerie::times(void) const
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
