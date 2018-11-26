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
#include "gnomonItemButton.h"

#include <gnomonFonts>
#include <gnomonCore/gnomonAbstractImagesSerieWriter>
#include <gnomonCore/gnomonImagesSerie>

#include <dtkScript>
#include <dtkImagingCore>

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>


// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItem
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
    gnomonImageManagerItem(const QColor&, const QPixmap& thumbnail, QWidget *parent = nullptr, bool multi_images = false);
    ~gnomonImageManagerItem(void);

signals:
    void destroy(void);
    void save(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

public:
    int id;

public:
    gnomonItemButton *button_destroy;
    gnomonItemButton *button_save;
    QPixmap thumbnail;
    QPixmap transparent_thumbnail;
};

gnomonImageManagerItem::gnomonImageManagerItem(const QColor& color, const QPixmap& thumbnail, QWidget *parent, bool multi_images) : QLabel(parent)
{
    this->button_destroy = new gnomonItemButton(color, fa::times, this);
    this->button_destroy->move(79, 5);
    this->button_destroy->setVisible(false);

    this->button_save = new gnomonItemButton(color, fa::save, this);
    this->button_save->move(5, 5);
    this->button_save->setVisible(false);

    int size  = 100;
    int space =   3;
    QPixmap pix;

    if (multi_images) {
        this->setBaseSize(size, size);
        pix = thumbnail.scaled(size, size, Qt::KeepAspectRatio);

        QPixmap pix2(100,100);
        pix2.fill(QColor("#282C34"));

        QImage image = pix.toImage();;

        QPainter paint(&pix2);
        paint.setPen(color);
        paint.drawRect(2*space,   0, size-2*space-1, size-2*space-1);
        paint.drawRect(space, space, size-2*space-1, size-2*space-1);
        paint.drawRect(0,   2*space, size-2*space-1, size-2*space-1);
        QRectF target(1,   2*space+1, size-2*space-2, size-2*space-2);
        QRectF source(0,           0, size-2*space-3, size-2*space-3);
        paint.drawImage(target, image, source);
        paint.end();

        this->setPixmap(pix2);
        this->thumbnail = pix2;

        this->transparent_thumbnail = pix2;
        this->transparent_thumbnail.fill();

    } else {
        pix = thumbnail.scaled(size, size, Qt::KeepAspectRatio);
        this->setPixmap(pix);

        this->thumbnail = *this->pixmap();

        this->transparent_thumbnail = *this->pixmap();
        this->transparent_thumbnail.fill();
    }

    QPainter painter;
    painter.begin(&transparent_thumbnail);
    painter.setOpacity(0.5);
    painter.drawPixmap(0, 0, *this->pixmap());
    painter.end();

    if (!multi_images)
        this->setStyleSheet(QString("border: 1px solid rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    connect(this->button_destroy, SIGNAL(clicked()), this, SIGNAL(destroy()));
    connect(this->button_save, SIGNAL(clicked()), this, SIGNAL(save()));

    this->setMouseTracking(true);
}

gnomonImageManagerItem::~gnomonImageManagerItem(void)
{

}

void gnomonImageManagerItem::enterEvent(QEvent *)
{
    this->button_destroy->setVisible(true);
    this->button_save->setVisible(true);
    this->setPixmap(this->transparent_thumbnail);
}

void gnomonImageManagerItem::leaveEvent(QEvent *)
{
    this->button_destroy->setVisible(false);
    this->button_save->setVisible(false);
    this->setPixmap(this->thumbnail);
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
    QSize sizeHint(void) const;

public:
    gnomonImageManagerItem *create(gnomonImagesSeriePtr, const QColor&);

public:
    QHash<gnomonImageManagerItem *, gnomonImagesSeriePtr> images_series;

public:
    gnomonAbstractImagesSerieWriter *writer;

public:
    static int item_counter;

public:
    bool inside = false;

public:
    QWidget *contents;
};

int gnomonImageManagerPrivate::item_counter = 0;

gnomonImageManagerPrivate::gnomonImageManagerPrivate(QWidget *parent) : QScrollArea(parent)
{
    this->contents = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this->contents);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    this->setFrameShape(QFrame::NoFrame);
    this->setWidget(this->contents);
    this->setWidgetResizable(true);

    QString plugin_save = "gnomonImagesSerieWriter";
    QString command = "import " + plugin_save;

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    this->writer =  gnomonCore::imagesSerieWriter::pluginFactory().create(plugin_save);

    if(!this->writer) {
        qWarning() << "cannot create plugin " << plugin_save << " you won't be able to save images!!";
    }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonImageManagerPrivate::~gnomonImageManagerPrivate(void)
{

}

QSize gnomonImageManagerPrivate::sizeHint(void) const
{
    return QSize(200, 130);
}

gnomonImageManagerItem *gnomonImageManagerPrivate::create(gnomonImagesSeriePtr images_serie, const QColor& color)
{
    if(!images_serie)
        return nullptr;

    dtkImage *image = images_serie->image();
    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(image);
    converter->convert();

    vtkImageData *o = static_cast<vtkImageData *>(converter->output());

    delete converter;

    double range[2];
    o->GetPointData()->GetScalars()->GetRange(range);
    double min = range[0];
    double max = range[1];

    int w = o->GetDimensions()[1];
    int h = o->GetDimensions()[0];
    int d = o->GetDimensions()[2];

    QImage i(h, w, QImage::Format_RGB32);

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

    bool multiple_images =  (images_serie->times() > 1);

    gnomonImageManagerItem *item = new gnomonImageManagerItem(color, QPixmap::fromImage(i), this, multiple_images);

    connect(item, &gnomonImageManagerItem::destroy, [=] () {
        this->contents->layout()->removeWidget(item);
        this->images_series.remove(item);
        delete item;
    });

    connect(item, &gnomonImageManagerItem::save, [=] () {
       if(this->writer) {
           QSettings settings("inria", "gnomon");
           settings.beginGroup("General");
           QString path = settings.value("last_saved_file", QDir::homePath()).toString();
           settings.endGroup();

           QString export_file_path = QFileDialog::getSaveFileName(this, tr("Save image"), path, tr("Images (*.inr.gz *.inr *.tif)"));

           if(!export_file_path.isEmpty()) {
               settings.beginGroup("general");
               settings.setValue("last_saved_file", export_file_path);
               settings.endGroup();

               this->writer->setImagesSerie(this->images_series[item].data());
               this->writer->setPath(export_file_path);
               this->writer->run();
           }
       }
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

void gnomonImageManager::addImage(gnomonImagesSeriePtr images_serie, const QColor& color)
{
    gnomonImageManagerItem *item = d->create(images_serie, color);
    item->id = d->item_counter++;

    d->images_series.insert(item, images_serie);
    d->contents->layout()->addWidget(item);
}

gnomonImagesSeriePtr gnomonImageManager::get(int index)
{
    for (auto it = d->images_series.begin(); it != d->images_series.end(); ++it) {
        if (index == it.key()->id) {
            return *it;
        }
    }
    return gnomonImagesSeriePtr();
}

QPixmap gnomonImageManager::thumbnail(int index)
{
    for (auto it = d->images_series.begin(); it != d->images_series.end(); ++it) {
        if (index == it.key()->id) {
            return *(it.key()->pixmap());
        }
    }
    return QPixmap();
}

gnomonImageManager::gnomonImageManager(QWidget *parent) : QFrame(parent)
{
    d = new gnomonImageManagerPrivate;

    QHBoxLayout *t_layout = new QHBoxLayout;
    t_layout->setContentsMargins(0, 0, 0, 0);
    t_layout->setSpacing(0);
    t_layout->addWidget(d);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(t_layout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->setMouseTracking(true);
}

gnomonImageManager::~gnomonImageManager(void)
{
    delete d;
}

QSize gnomonImageManager::sizeHint(void) const
{
    return QSize(200, 140);
}

void gnomonImageManager::enterEvent(QEvent *)
{
    d->inside = true;

    this->update();
}

void gnomonImageManager::leaveEvent(QEvent *)
{
    d->inside = false;

    this->update();
}

void gnomonImageManager::mousePressEvent(QMouseEvent *event)
{
    QRect handle = QRect(this->size().width() / 2 - 100, this->size().height() - 10, 200, 10);

    if (handle.contains(event->pos())) {

        if(this->size().height() < 150)
            emit expand();
        else
            emit shrink();
    }
}

void gnomonImageManager::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if(!d->inside)
        return;

    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(event->rect().width() / 2 - 100, event->rect().height() - 10, 200, 6, 3, 3);
}

gnomonImageManager *gnomonImageManager::s_instance = nullptr;

// ///////////////////////////////////////////////////////////////////

#include "gnomonImageManager.moc"

//
// gnomonImageManager.cpp ends here
