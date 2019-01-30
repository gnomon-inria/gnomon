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

#include "gnomonAbstractVisualization.h"
#include "gnomonFormManager.h"
#include "gnomonFormManager_p.h"
#include "gnomonFormManagerData.h"
#include "gnomonFormManagerFocus.h"
#include "gnomonFormManagerItem.h"
#include "gnomonItemButton.h"
#include "gnomonToolBar.h"

#include <gnomonFonts>

#include <gnomonCore/gnomonAbstractForm>
#include <gnomonCore/gnomonImagesSerie>
#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCellImage>

#include <gnomonCore/gnomonImagesSerieWriterCommand>
#include <gnomonCore/gnomonMeshWriterCommand>
#include <gnomonCore/gnomonCellImageWriterCommand>

#include <dtkScript>

// ///////////////////////////////////////////////////////////////////
// gnomonFormManagerPrivate
// ///////////////////////////////////////////////////////////////////

int gnomonFormManagerPrivate::item_counter = 0;

gnomonFormManagerPrivate::gnomonFormManagerPrivate(QWidget *parent) : QScrollArea(parent)
{
    this->contents = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this->contents);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    this->setFrameShape(QFrame::NoFrame);
    this->setWidget(this->contents);
    this->setWidgetResizable(true);

    // QString plugin_save = "gnomonAbstractFormWriter";
    // QString command = "import " + plugin_save;

    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    // this->writer =  gnomonCore::imagesSerieWriter::pluginFactory().create(plugin_save);

    // if(!this->writer) {
    //     qWarning() << "cannot create plugin " << plugin_save << " you won't be able to save images!!";
    // }

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonFormManagerPrivate::~gnomonFormManagerPrivate(void)
{

}

QSize gnomonFormManagerPrivate::sizeHint(void) const
{
    return QSize(200, 130);
}

gnomonFormManagerItem *gnomonFormManagerPrivate::create(gnomonAbstractForm * form, const QColor& color, const QImage& image)
{
    gnomonFormManagerItem *item = new gnomonFormManagerItem(color, QPixmap::fromImage(image), this);

    connect(item, &gnomonFormManagerItem::destroy, [=] () {
        this->contents->layout()->removeWidget(item);
        this->forms.remove(item);
        this->formWriterCommand.remove(item);
        delete item;
    });

    connect(item, &gnomonFormManagerItem::save, [=] () {
        QSettings settings("inria", "gnomon");
        settings.beginGroup("General");
        QString path = settings.value("last_saved_file", QDir::homePath()).toString();
        settings.endGroup();

        gnomonAbstractForm *form = this->forms[item];

        QString export_file_path;
        if (gnomonImagesSerie *images_serie = dynamic_cast<gnomonImagesSerie *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save image"), path, tr("Images (*.inr.gz *.inr *.tif)"));
            static_cast<gnomonImagesSerieWriterCommand *>(this->formWriterCommand[item])->setImagesSerie(images_serie);
            static_cast<gnomonImagesSerieWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonMesh *mesh = dynamic_cast<gnomonMesh *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save mesh"), path, tr("Meshes (*.ply)"));
            static_cast<gnomonMeshWriterCommand *>(this->formWriterCommand[item])->setMesh(mesh);
            static_cast<gnomonMeshWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonCellImage *cellimage = dynamic_cast<gnomonCellImage *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save cell image"), path, tr("Images (*.tif)"));
            static_cast<gnomonCellImageWriterCommand *>(this->formWriterCommand[item])->setCellImage(cellimage);
            static_cast<gnomonCellImageWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        }

        if(!export_file_path.isEmpty()) {
           settings.beginGroup("general");
           settings.setValue("last_saved_file", export_file_path);
           settings.endGroup();
           this->formWriterCommand[item]->redo();
        }
    });

    connect(item, &gnomonFormManagerItem::clicked, [=] () {
        q->present(item);
    });

    gnomonFormManagerData *data = new gnomonFormManagerData(this);
    data->reference = item;
    data->data["Name"] = form->name();
    this->formData.insert(item, data);


    return item;
}

// ///////////////////////////////////////////////////////////////////
// gnomonFormManager
// ///////////////////////////////////////////////////////////////////

gnomonFormManager *gnomonFormManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonFormManager;

    return s_instance;
}

void gnomonFormManager::addForm(gnomonAbstractForm * form, const QColor& color, gnomonAbstractVisualization* visualization)
{

    QImage image = visualization->imageRendering();
    gnomonFormManagerItem *item = d->create(form, color, image);
    item->id = d->item_counter++;

    d->forms.insert(item, form);
    d->formVisualizations.insert(item, visualization);


    QString writerPlugin;
    if (gnomonImagesSerie *images_serie = dynamic_cast<gnomonImagesSerie *>(form)) {
        d->formWriterCommand[item] = new gnomonImagesSerieWriterCommand("gnomonImagesSerieWriter");
        static_cast<gnomonImagesSerieWriterCommand *>(d->formWriterCommand[item])->setImagesSerie(images_serie);
    } else if (gnomonMesh *mesh = dynamic_cast<gnomonMesh *>(form)) {
        d->formWriterCommand[item] = new gnomonMeshWriterCommand("gnomonMeshWriterPropertyTopomesh");
        static_cast<gnomonMeshWriterCommand *>(d->formWriterCommand[item])->setMesh(mesh);
    } else if (gnomonCellImage *cellimage = dynamic_cast<gnomonCellImage *>(form)) {
        d->formWriterCommand[item] = new gnomonCellImageWriterCommand("gnomonCellImageWriterPropertySpatialImage");
        static_cast<gnomonCellImageWriterCommand *>(d->formWriterCommand[item])->setCellImage(cellimage);
    }

    d->contents->layout()->addWidget(item);
}

gnomonAbstractForm * gnomonFormManager::get(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it) {
        if (index == it.key()->id) {
            return *it;
        }
    }
    return nullptr;
}

gnomonAbstractVisualization * gnomonFormManager::getVisualization(int index)
{
    for (auto it = d->formVisualizations.begin(); it != d->formVisualizations.end(); ++it) {
        if (index == it.key()->id) {
            return *it;
        }
    }
    return nullptr;
}

QPixmap gnomonFormManager::thumbnail(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it) {
        if (index == it.key()->id) {
            return *(it.key()->pixmap());
        }
    }
    return QPixmap();
}

gnomonFormManager::gnomonFormManager(QWidget *parent) : QFrame(parent)
{
    d = new gnomonFormManagerPrivate;
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

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

QSize gnomonFormManager::sizeHint(void) const
{
    return QSize(200, 140);
}

void gnomonFormManager::present(gnomonFormManagerItem *item)
{
    if(!d->focus_item)
        d->focus_item = new gnomonFormManagerFocus(this);

    if (d->focus_area)
        delete d->focus_area;

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

    QRect focus_item_dest_rect;

    {

        d->focus_item->move(item->pos());
        d->focus_item->resize(item->size());
        d->focus_item->setStyleSheet("border: 2px solid white;");
        d->focus_item->show();

        d->focus_item->source = d->focus_item->pos();
        d->focus_item->destnt = QPoint(this->size().width() / 2 - 3 * d->focus_item->width() / 2, this->size().height() / 2 - 3 * d->focus_item->height() / 2);
        d->focus_item->s_size = d->focus_item->size();
        d->focus_item->d_size = d->focus_item->size() * 3;

        focus_item_dest_rect = QRect(d->focus_item->destnt, d->focus_item->size() * 3);

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
            // d->focus_item->setPixmap(item->thumbnail.scaled(value.toSize().width(), value.toSize().height()));
            d->focus_item->setPixmap(item->image.scaled(value.toSize().width(), value.toSize().height()));
        });

        connect(g_animation, &QAbstractAnimation::finished, [=] () {
            d->focus_area = d->formData[item]->compute();
            d->focus_area->setParent(this);
            d->focus_area->move(focus_item_dest_rect.topRight() + QPoint(20, 0));
            d->focus_area->resize(d->focus_item->size());
            d->focus_area->show();
            d->focus_item->presented = true;
        });

        animation->addAnimation(g_animation);
    }

    animation->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animation, &QAbstractAnimation::finished, [=] {
        d->focus_area = new QScrollArea(0);
        d->focus_area->setParent(this);
        d->focus_area->move(focus_item_dest_rect.topRight() + QPoint(20, 0));
        d->focus_area->resize(d->focus_item->size());
        d->focus_area->show();
    });
}

void gnomonFormManager::enterEvent(QEvent *)
{
    d->inside = true;

    this->update();
}

void gnomonFormManager::leaveEvent(QEvent *)
{
    d->inside = false;

    this->update();
}

void gnomonFormManager::mousePressEvent(QMouseEvent *event)
{
    QRect handle = QRect(this->size().width() / 2 - 100, this->size().height() - 10, 200, 10);

    if (handle.contains(event->pos())) {

        if(this->size().height() < 150) {
            emit expand();
            d->state = gnomonFormManagerPrivate::Expanded;
        } else {
            emit shrink();
            d->state = gnomonFormManagerPrivate::Collapsed;
        }
    }
}

void gnomonFormManager::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if(!d->inside)
        return;

    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(event->rect().width() / 2 - 100, event->rect().height() - 10, 200, 6, 3, 3);
}

gnomonFormManager *gnomonFormManager::s_instance = nullptr;

//
// gnomonFormManager.cpp ends here
