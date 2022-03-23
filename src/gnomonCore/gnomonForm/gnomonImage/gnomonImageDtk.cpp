#include "gnomonImage.h"

#include <dtkImagingCore>
#include <vtkImageData.h>


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

QMap<QString,QString> gnomonImage::metadata(void) const
{
    QString channel = this->channels()[0];
    dtkImage *image = this->image(channel);
    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(image);
    converter->convert();

    vtkImageData *image_data = static_cast<vtkImageData *>(converter->output());

    delete converter;

    QMap<QString,QString> metadata;

    qDebug()<<Q_FUNC_INFO<<this->channels().size();
    metadata["Number of channels"] = QString::number(this->channels().size());
    if (this->channels().size()>1) {
        int i_channel = 0;
        for (const auto& channel : this->channels()) {
            metadata["Channel "+QString::number(i_channel)] = channel;
            i_channel++;
        }
    }
    metadata["Dimensions"] = "("+QString::number(image_data->GetDimensions()[0])+", "+QString::number(image_data->GetDimensions()[1])+","+QString::number(image_data->GetDimensions()[2])+")";
    metadata["Voxel Type"] = QString(QVariant::typeToName(image->storageType()));
    metadata["Voxel Size"] = "("+QString::number(image_data->GetSpacing()[0])+", "+QString::number(image_data->GetSpacing()[1])+","+QString::number(image_data->GetSpacing()[2])+")";

    return metadata;
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
