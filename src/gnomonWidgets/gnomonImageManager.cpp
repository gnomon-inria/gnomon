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
#include "gnomonImageManager_p.h"
#include "gnomonImageManagerItem.h"
#include "gnomonImageManagerFocus.h"
#include "gnomonItemButton.h"

#include <gnomonCore/gnomonAbstractImagesSerieWriter.h>

#include <dtkScript>
#include <dtkImagingCore>

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerPrivate
// ///////////////////////////////////////////////////////////////////

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

    gnomonImageManagerItem *item = new gnomonImageManagerItem(color, QPixmap::fromImage(i), this);

    connect(item, &gnomonImageManagerItem::destroy, [=] () {
        this->contents->layout()->removeWidget(item);
        this->images.remove(item);
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

               this->writer->setImage(this->images[item].data());
               this->writer->setPath(export_file_path);
               this->writer->run();
           }
       }
    });

    connect(item, &gnomonImageManagerItem::clicked, [=] () {
        q->present(item);
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
    d->q = this;

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

void gnomonImageManager::present(gnomonImageManagerItem *item)
{
    if(!d->focus_item)
        d->focus_item = new gnomonImageManagerFocus(this);

    QSequentialAnimationGroup *animation = new QSequentialAnimationGroup(this);

    if (d->focus_item->presented) {

        QVariantAnimation *p_animation = new QVariantAnimation(this);
        p_animation->setDuration(250);
        p_animation->setStartValue(d->focus_item->destnt);
        p_animation->setEndValue(d->focus_item->source);
        p_animation->setEasingCurve(QEasingCurve::OutQuad);

        QVariantAnimation *s_animation = new QVariantAnimation(this);
        s_animation->setDuration(250);
        s_animation->setStartValue(d->focus_item->d_size);
        s_animation->setEndValue(d->focus_item->s_size);
        s_animation->setEasingCurve(QEasingCurve::OutQuad);

        QParallelAnimationGroup *g_animation = new QParallelAnimationGroup(this);
        g_animation->addAnimation(p_animation);
        g_animation->addAnimation(s_animation);

        connect(p_animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->focus_item->move(value.toPoint());
        });

        connect(s_animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->focus_item->resize(value.toSize());
            d->focus_item->setPixmap(d->focus_item->pixmap()->scaled(value.toSize().width(), value.toSize().height()));
        });

        connect(g_animation, &QAbstractAnimation::finished, [=] () {
            d->focus_item->presented = false;
        });

        animation->addAnimation(g_animation);

    }

    {

        d->focus_item->move(item->pos());
        d->focus_item->resize(item->size());
        d->focus_item->setStyleSheet("border: 2px solid white;");
        d->focus_item->show();

        d->focus_item->source = d->focus_item->pos();
        d->focus_item->destnt = QPoint(this->size().width() / 2 - 3 * d->focus_item->width() / 2, this->size().height() / 2 - 3 * d->focus_item->height() / 2);
        d->focus_item->s_size = d->focus_item->size();
        d->focus_item->d_size = d->focus_item->size() * 3;

        QVariantAnimation *p_animation = new QVariantAnimation(this);
        p_animation->setDuration(500);
        p_animation->setStartValue(d->focus_item->source);
        p_animation->setEndValue(d->focus_item->destnt);
        p_animation->setEasingCurve(QEasingCurve::OutQuad);

        QVariantAnimation *s_animation = new QVariantAnimation(this);
        s_animation->setDuration(500);
        s_animation->setStartValue(d->focus_item->s_size);
        s_animation->setEndValue(d->focus_item->d_size);
        s_animation->setEasingCurve(QEasingCurve::OutQuad);

        QParallelAnimationGroup *g_animation = new QParallelAnimationGroup(this);
        g_animation->addAnimation(p_animation);
        g_animation->addAnimation(s_animation);

        connect(p_animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->focus_item->move(value.toPoint());
        });

        connect(s_animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->focus_item->resize(value.toSize());
            d->focus_item->setPixmap(item->thumbnail.scaled(value.toSize().width(), value.toSize().height()));
        });

        connect(g_animation, &QAbstractAnimation::finished, [=] () {
            d->focus_item->presented = true;
        });

        animation->addAnimation(g_animation);
    }

    animation->start(QAbstractAnimation::DeleteWhenStopped);
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

        if(this->size().height() < 150) {
            emit expand();
            d->state = gnomonImageManagerPrivate::Expanded;
        } else {
            emit shrink();
            d->state = gnomonImageManagerPrivate::Collapsed;
        }
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

//
// gnomonImageManager.cpp ends here
