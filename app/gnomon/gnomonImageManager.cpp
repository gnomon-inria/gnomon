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

#include "gnomonImageManager.h"

#include <dtkImagingCore>

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItem
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItem(const QPixmap& thumbnail, QWidget *parent = nullptr);
    ~gnomonImageManagerItem(void);

protected:
    void mousePressEvent(QMouseEvent *);

public:
    int id;
};

gnomonImageManagerItem::gnomonImageManagerItem(const QPixmap& thumbnail, QWidget *parent) : QLabel(parent)
{
    this->setPixmap(thumbnail.scaled(100, 100));
}

gnomonImageManagerItem::~gnomonImageManagerItem(void)
{

}

void gnomonImageManagerItem::mousePressEvent(QMouseEvent *)
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString(":%1").arg(this->id));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*(this->pixmap()));
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

    Qt::DropAction dropAction = drag->exec();
}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerPrivate : public QScrollArea
{
public:
     gnomonImageManagerPrivate(QWidget *parent = nullptr);
    ~gnomonImageManagerPrivate(void);

public:
    gnomonImageManagerItem *create(gnomonImageManager::Image);

public:
    QHash<gnomonImageManagerItem *, gnomonImageManager::Image> images;

public:
    QWidget *contents;
};

gnomonImageManagerPrivate::gnomonImageManagerPrivate(QWidget *parent) : QScrollArea(parent)
{
    this->contents = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this->contents);
    layout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    this->setFrameShape(QFrame::NoFrame);
    this->setWidget(this->contents);
    this->setWidgetResizable(true);
}

gnomonImageManagerPrivate::~gnomonImageManagerPrivate(void)
{

}

gnomonImageManagerItem *gnomonImageManagerPrivate::create(gnomonImageManager::Image image)
{
    if(!image)
        return nullptr;

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(image);
    converter->convert();

    vtkImageData *o = static_cast<vtkImageData *>(converter->output());

    delete converter;

    int w = o->GetDimensions()[0];
    int h = o->GetDimensions()[1];
    int d = o->GetDimensions()[2];

    QImage i(w, h, QImage::Format_RGB32);

    QRgb *b = reinterpret_cast<QRgb *>(i.bits());

    int z = d/2;
    for(int c = 0; c < w; ++c) {
        for(int r = 0; r < h; ++r) {
            unsigned char *p = reinterpret_cast<unsigned char *>(o->GetScalarPointer(r, w-c-1, z));
            *(b) = QColor(p[0], p[0], p[0]).rgb();
            ++b;
        }
    }

    return new gnomonImageManagerItem(QPixmap::fromImage(i), this);
}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManager
// ///////////////////////////////////////////////////////////////////

gnomonImageManager *gnomonImageManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonImageManager;

    return s_instance;
}

QSize gnomonImageManager::sizeHint(void) const
{
    return QSize(200, 140);
}

void gnomonImageManager::addImage(gnomonImageManager::Image image)
{
    gnomonImageManagerItem *item = d->create(image);

    d->images.insert(item, image);
    d->contents->layout()->addWidget(item);

    item->id = d->images.values().indexOf(image);
}

gnomonImageManager::Image gnomonImageManager::get(int index)
{
    return d->images.values().at(index);
}

QPixmap gnomonImageManager::thumbnail(int index)
{
    return *(d->images.keys().at(index)->pixmap());
}

gnomonImageManager::gnomonImageManager(QWidget *parent) : QFrame(parent)
{
    d = new gnomonImageManagerPrivate;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonImageManager::~gnomonImageManager(void)
{
    delete d;
}

gnomonImageManager *gnomonImageManager::s_instance = nullptr;

// ///////////////////////////////////////////////////////////////////

#include "gnomonImageManager.moc"

//
// gnomonImageManager.cpp ends here
