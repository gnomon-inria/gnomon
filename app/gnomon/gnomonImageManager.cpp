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

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItem
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItem(const QPixmap& thumbnail, QWidget *parent = nullptr);
    ~gnomonImageManagerItem(void);
};

gnomonImageManagerItem::gnomonImageManagerItem(const QPixmap& thumbnail, QWidget *parent) : QLabel(parent)
{
    this->setPixmap(thumbnail.scaled(100, 100));
}

gnomonImageManagerItem::~gnomonImageManagerItem(void)
{

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

    int w = image->GetDimensions()[0];
    int h = image->GetDimensions()[1];
    int d = image->GetDimensions()[2];

    QImage i(w, h, QImage::Format_RGB32);

    QRgb *b = reinterpret_cast<QRgb *>(i.bits()) + w * (h - 1);

    unsigned char *p = reinterpret_cast<unsigned char *>(image->GetScalarPointer());

    for(int r = 0; r < h; r++) {
        for(int c = 0; c < w; c++) {
            *(b) = QColor(p[0], p[1], p[2]).rgb();
            p += image->GetNumberOfScalarComponents();
        }

        b -= w * 2;
    }

    return new gnomonImageManagerItem(QPixmap::fromImage(i), this);
}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManager
// ///////////////////////////////////////////////////////////////////

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

QSize gnomonImageManager::sizeHint(void) const
{
    return QSize(200, 140);
}

void gnomonImageManager::addImage(gnomonImageManager::Image image)
{
    gnomonImageManagerItem *item = d->create(image);

    d->images.insert(item, image);
    d->contents->layout()->addWidget(item);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonImageManager.moc"

//
// gnomonImageManager.cpp ends here
