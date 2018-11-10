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

#include <gnomonFonts>

#include <dtkImagingCore>

#include <vtkDataArray.h>
#include <vtkPointData.h>

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItemButton
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItemButton : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItemButton(const QColor& color, QWidget *parent = nullptr);
    ~gnomonImageManagerItemButton(void);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    gnomonFontAwesome *font;
};


gnomonImageManagerItemButton::gnomonImageManagerItemButton(const QColor& color, QWidget *parent) : QLabel(parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", color);

    this->setPixmap(this->font->icon(fa::times).pixmap(16, 16));

    this->setStyleSheet("background: none; border: none;");
}

gnomonImageManagerItemButton::~gnomonImageManagerItemButton(void)
{

}


void gnomonImageManagerItemButton::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItem
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItem(const QColor&, const QPixmap& thumbnail, QWidget *parent = nullptr);
    ~gnomonImageManagerItem(void);

signals:
    void destroy(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

public:
    int id;

public:
    gnomonImageManagerItemButton *button;
};

gnomonImageManagerItem::gnomonImageManagerItem(const QColor& color, const QPixmap& thumbnail, QWidget *parent) : QLabel(parent)
{
    this->button = new gnomonImageManagerItemButton(color, this);
    this->button->move(79, 5);
    this->button->setVisible(false);

    this->setPixmap(thumbnail.scaled(100, 100));
    this->setStyleSheet(QString("border: 1px solid rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    connect(this->button, SIGNAL(clicked()), this, SIGNAL(destroy()));

    this->setMouseTracking(true);
}

gnomonImageManagerItem::~gnomonImageManagerItem(void)
{

}

void gnomonImageManagerItem::enterEvent(QEvent *)
{
    this->button->setVisible(true);
}

void gnomonImageManagerItem::leaveEvent(QEvent *)
{
    this->button->setVisible(false);
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
    gnomonImageManagerItem *create(dtkImagePtr, const QColor&);

public:
    QHash<gnomonImageManagerItem *, dtkImagePtr> images;

public:
    static int item_counter;

public:
    QWidget *contents;
};

int gnomonImageManagerPrivate::item_counter = 0;

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

gnomonImageManagerItem *gnomonImageManagerPrivate::create(dtkImagePtr image, const QColor& color)
{
    if(!image)
        return nullptr;

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(image.data());
    converter->convert();

    vtkImageData *o = static_cast<vtkImageData *>(converter->output());

    delete converter;

    double range[2];
    o->GetPointData()->GetScalars()->GetRange(range);
    double min = range[0];
    double max = range[1];

    int w = o->GetDimensions()[0];
    int h = o->GetDimensions()[1];
    int d = o->GetDimensions()[2];

    QImage i(w, h, QImage::Format_RGB32);

    QRgb *b = reinterpret_cast<QRgb *>(i.bits());

    int z = d/2;
    for(int c = 0; c < w; ++c) {
        for(int r = 0; r < h; ++r) {
            double v;

            switch(image->storageType()) {
             case QMetaType::UChar:
                 v = reinterpret_cast<unsigned char *>(o->GetScalarPointer(r, w-c-1, z))[0];
                 break;

            case QMetaType::UShort:
                v = reinterpret_cast<unsigned short *>(o->GetScalarPointer(r, w-c-1, z))[0];
                break;

            default:
                v = 0;
                qWarning() << Q_FUNC_INFO << "not implemented for " << image->storageType();
                break;
            }

            v -= min;
            v *= (255./(max-min));

            *(b) = QColor(v, v, v).rgb();
            ++b;
        }
    }

    gnomonImageManagerItem *item = new gnomonImageManagerItem(color, QPixmap::fromImage(i), this);

    connect(item, &gnomonImageManagerItem::destroy, [=] () {
        this->contents->layout()->removeWidget(item);
        this->images.remove(item);
        delete item;
    });

    return item;
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

void gnomonImageManager::addImage(dtkImagePtr image, const QColor& color)
{
    gnomonImageManagerItem *item = d->create(image, color);
    item->id = d->item_counter++;

    d->images.insert(item, image);
    d->contents->layout()->addWidget(item);
}

dtkImagePtr gnomonImageManager::get(int index)
{
    for (auto it = d->images.begin(); it != d->images.end(); ++it) {
        if (index == it.key()->id) {
            return *it;
        }
    }
    return dtkImagePtr();
}

QPixmap gnomonImageManager::thumbnail(int index)
{
    for (auto it = d->images.begin(); it != d->images.end(); ++it) {
        if (index == it.key()->id) {
            return *(it.key()->pixmap());
        }
    }
    return QPixmap();
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
