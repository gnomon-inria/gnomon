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

#include "gnomonVisualizations/gnomonAbstractVisualization.h"
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

#include "gnomonFormManager.h"
#include "gnomonFormManager_p.h"
#include "gnomonFormManagerData.h"
#include "gnomonFormManagerFocus.h"
#include "gnomonFormManagerItem.h"
#include "gnomonItemButton.h"
#include "gnomonToolBar.h"
#include "gnomonView/gnomonViewForm.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeWriterCommand>

#include <dtkFonts>
#include <dtkThemes>
#include <dtkScript>

#include <vtkCamera.h>

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

    // QString plugin_save = "gnomonplugins.core.io.gnomonAbstractFormWriter";
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

gnomonFormManagerItem *gnomonFormManagerPrivate::create(gnomonAbstractDynamicForm * form, const QColor& color, const QImage& image)
{
    gnomonFormManagerItem *item = new gnomonFormManagerItem(color, QPixmap::fromImage(image), form->times().size(), this);

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

        gnomonAbstractDynamicForm *form = this->forms[item];

        QString export_file_path;
        if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save image"), path, tr("Images (*.tif)"));
            static_cast<gnomonImageWriterCommand *>(this->formWriterCommand[item])->setImage(image);
            static_cast<gnomonImageWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save mesh"), path, tr("Meshes (*.ply)"));
            static_cast<gnomonMeshWriterCommand *>(this->formWriterCommand[item])->setMesh(mesh);
            static_cast<gnomonMeshWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonCellImageSeries *cellimage = dynamic_cast<gnomonCellImageSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save cell image"), path, tr("Images (*.tif)"));
            static_cast<gnomonCellImageWriterCommand *>(this->formWriterCommand[item])->setCellImage(cellimage);
            static_cast<gnomonCellImageWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonCellComplexSeries *cellcomplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save cell complex"), path, tr("Meshes (*.ply)"));
            static_cast<gnomonCellComplexWriterCommand *>(this->formWriterCommand[item])->setCellComplex(cellcomplex);
            static_cast<gnomonCellComplexWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save data frame"), path, tr("Comma separated value (*.csv)"));
            gnomonDataFrame * df = dynamic_cast<gnomonDataFrame *>(dataFrame->current());
            static_cast<gnomonDataFrameWriterCommand *>(this->formWriterCommand[item])->setDataFrame(df);
            static_cast<gnomonDataFrameWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
        } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
            export_file_path = QFileDialog::getSaveFileName(this, tr("Save tree"), path, tr("Comma separated value (*.xml)"));
            static_cast<gnomonTreeWriterCommand *>(this->formWriterCommand[item])->setPath(export_file_path);
            static_cast<gnomonTreeWriterCommand *>(this->formWriterCommand[item])->setInput(tree);
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
    data->data = form->current()->metadata();
    this->formData.insert(item, data);

    return item;
}

gnomonFormManagerItem *gnomonFormManagerPrivate::create(const QString& contents, const QColor& color, const QImage& image)
{
    gnomonFormManagerItem *item = new gnomonFormManagerItem(color, QPixmap::fromImage(image), 1, this);

    connect(item, &gnomonFormManagerItem::destroy, [=] () {
        this->contents->layout()->removeWidget(item);
        this->forms.remove(item);
        this->formWriterCommand.remove(item);
        delete item;
    });

    connect(item, &gnomonFormManagerItem::clicked, [=] () {
        q->present(item);
    });

    gnomonFormManagerData *data = new gnomonFormManagerData(this);
    data->contents = contents;
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

    this->setMouseTracking(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

QSize gnomonFormManager::sizeHint(void) const
{
    return QSize(200, 140);
}

void gnomonFormManager::addForm(gnomonAbstractDynamicForm *form, const QColor& color, gnomonAbstractVisualization *visualization, vtkCamera *cam)
{
    QImage image = visualization->imageRendering();

    gnomonFormManagerItem *item = d->create(form, color, image);
    item->id = d->item_counter++;

    d->forms.insert(item, form->clone());
    d->formVisualizations.insert(item, visualization);
    d->formCameras.insert(item, cam);

    QString writerPlugin;

    if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
        d->formWriterCommand[item] = new gnomonImageWriterCommand("gnomonImageWriter");
        static_cast<gnomonImageWriterCommand *>(d->formWriterCommand[item])->setImage(image);
    } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        d->formWriterCommand[item] = new gnomonMeshWriterCommand("gnomonMeshWriterPropertyTopomesh");
        static_cast<gnomonMeshWriterCommand *>(d->formWriterCommand[item])->setMesh(mesh);
    } else if (gnomonCellImageSeries *cellimage = dynamic_cast<gnomonCellImageSeries *>(form)) {
        d->formWriterCommand[item] = new gnomonCellImageWriterCommand("gnomonCellImageWriterPropertySpatialImage");
        static_cast<gnomonCellImageWriterCommand *>(d->formWriterCommand[item])->setCellImage(cellimage);
    } else if (gnomonCellComplexSeries *cellcomplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        d->formWriterCommand[item] = new gnomonCellComplexWriterCommand("gnomonCellComplexWriterPropertyTopomesh");
        static_cast<gnomonCellComplexWriterCommand *>(d->formWriterCommand[item])->setCellComplex(cellcomplex);
    }

    d->contents->layout()->addWidget(item);
}

void gnomonFormManager::addForm(gnomonAbstractDynamicForm * form, const QColor& color, gnomonAbstractMatplotlibVisualization* visualization)
{
    QImage image = visualization->imageRendering();

    gnomonFormManagerItem *item = d->create(form, color, image);
    item->id = d->item_counter++;

    d->forms.insert(item, form);
    d->formMatplotlibVisualizations.insert(item, visualization);

    QString writerPlugin;

    if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        gnomonDataFrame * df = dynamic_cast<gnomonDataFrame *>(dataFrame->current());
        d->formWriterCommand[item] = new gnomonDataFrameWriterCommand("gnomonDataFrameWriterPandas");
        static_cast<gnomonDataFrameWriterCommand *>(d->formWriterCommand[item])->setDataFrame(df);
    } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        d->formWriterCommand[item] = new gnomonTreeWriterCommand("gnomonTreeWriterTreex");
        static_cast<gnomonTreeWriterCommand *>(d->formWriterCommand[item])->setInput(tree);
    }

    d->contents->layout()->addWidget(item);
}

void gnomonFormManager::addForm(gnomonAbstractDynamicForm * form, const QColor& color, const QImage& image)
{
    gnomonFormManagerItem *item = d->create(form, color, image);
    item->id = d->item_counter++;

    d->forms.insert(item, form);
    d->formMatplotlibVisualizations.insert(item, nullptr);

    QString writerPlugin;

    if (gnomonDataFrame *dataFrame = dynamic_cast<gnomonDataFrame *>(form)) {
        d->formWriterCommand[item] = new gnomonDataFrameWriterCommand("gnomonDataFrameWriterPandas");
        static_cast<gnomonDataFrameWriterCommand *>(d->formWriterCommand[item])->setDataFrame(dataFrame);
    }

    d->contents->layout()->addWidget(item);
}

gnomonAbstractDynamicForm *gnomonFormManager::get(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it)
        if (index == it.key()->id)
            return *it;

    return nullptr;
}

gnomonAbstractVisualization *gnomonFormManager::getVisualization(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it) {
        if (index == it.key()->id) {
            if (d->formVisualizations.contains(it.key()))
                return d->formVisualizations.value(it.key());
            else
                return nullptr;
        }
    }

    return nullptr;
}

vtkCamera *gnomonFormManager::getCamera(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it) {
        if (index == it.key()->id) {
            if (d->formCameras.contains(it.key()))
                return d->formCameras.value(it.key());
            else
                return nullptr;
        }
    }

    return nullptr;
}

QPixmap gnomonFormManager::thumbnail(int index)
{
    for (auto it = d->forms.begin(); it != d->forms.end(); ++it)
        if (index == it.key()->id)
            return *(it.key()->pixmap());

    return QPixmap();
}

void gnomonFormManager::present(gnomonFormManagerItem *item)
{
    if(d->animation)
    {
        d->deferred_focus = item;
        d->animation->setDirection(QAbstractAnimation::Backward);
        d->animation->start();
        d->dismissView();
        return;
    }

    if(!d->focus_item)
        d->focus_item = new gnomonFormManagerFocus(this);

    // if (d->focus_area)
    //     delete d->focus_area;

    d->animation = new QSequentialAnimationGroup(this);

    QRect focus_item_dest_rect;
    {
        d->focus_item->move(item->pos());
        d->focus_item->resize(item->size());
        d->focus_item->setStyleSheet("border: 2px solid white;");
        d->focus_item->show();

        d->focus_item->source = d->focus_item->pos();
//        d->focus_item->destnt = QPoint(this->width() / 4 - this->width() / 8, this->size().height() / 4 + 50);
        d->focus_item->destnt = QPoint(this->width()/2 - this->size().height()/2, this->size().height()/4);
        d->focus_item->s_size = d->focus_item->size();
//        d->focus_item->d_size = QSize(this->width()/2, this->height() * 1/2);
        d->focus_item->d_size = QSize(2*this->height()/4, 2*this->height()/4);

        focus_item_dest_rect = QRect(d->focus_item->destnt, d->focus_item->size() * 6);

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

        connect(s_animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value)
        {
            d->focus_item->resize(value.toSize());
            // d->focus_item->setPixmap(item->thumbnail.scaled(value.toSize().width(), value.toSize().height()));
            d->focus_item->setPixmap(item->image.scaled(value.toSize().width(), value.toSize().height()));

            this->repaint();
        });

        connect(s_animation, &QAbstractAnimation::finished, [=] (void) -> void
        {
            // if (d->view != nullptr) {

            //     d->view->hide();

            //     delete d->view;
            // }

            this->repaint();
        });

        connect(g_animation, &QAbstractAnimation::finished, [=] () {

            qDebug() << Q_FUNC_INFO << "In here";

            if(g_animation->direction() == QAbstractAnimation::Forward) {

                QScrollArea *data = d->formData[item]->compute();

                d->focus_item->presented = true;

                d->focus_area = data;
                d->focus_area->setParent(this);
                d->focus_area->setEnabled(d->focus_item->presented ? true : false);
                d->focus_area->move(focus_item_dest_rect.topRight() + QPoint(20, 0));
                d->focus_area->resize(QSize(2*this->height()/4, 2*this->height()/4));
                d->focus_area->setVisible(true);
                d->focus_area->setStyleSheet(QString("QScrollArea { border: 1px solid %1; border-radius: 10; }").arg(dtkThemesEngine::instance()->value("@hl")));

                gnomonAbstractDynamicForm *form = gnomonFormManager::instance()->get(item->id);

                d->view = new gnomonViewForm(this);
                d->view->setInputView(true);
                d->view->setEnableMenus(false);
                d->view->resize(QSize(2*this->height()/4, 2*this->height()/4));
                d->view->move(d->focus_item->pos());
                d->view->hide();

                connect(d->view, &gnomonViewForm::formAdded, [=] () {
                    d->view->show();
                });

                d->view->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(item->id));

            } else {
                d->deleteAnimation();
                d->dismissView();

                if (d->deferred_focus)
                {
                    this->present(d->deferred_focus);
                    d->animation->setDirection(QAbstractAnimation::Forward);
                    d->animation->start();
                    d->deferred_focus = nullptr;
                }
            }
        });

        d->animation->addAnimation(g_animation);
    }

    d->animation->start();
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
    qDebug() << Q_FUNC_INFO << event->pos();

    QRect handle = QRect(this->size().width() / 2 - 100, this->size().height() - 10, 200, 10);

    if (handle.contains(event->pos())) {

        if(this->size().height() < 150) {
            emit expand();
            d->state = gnomonFormManagerPrivate::Expanded;
        } else {
            emit shrink();
            d->state = gnomonFormManagerPrivate::Collapsed;
        }

    } else {

        qDebug() << Q_FUNC_INFO << 0;

        if(gnomonFormManagerItem *item = dynamic_cast<gnomonFormManagerItem *>(childAt(event->pos()))) {

            qDebug() << Q_FUNC_INFO << 10;

            d->animation->setDirection(d->focus_item->presented ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);

            d->animation->start();

            event->ignore();

            return QFrame::mousePressEvent(event);
        }

        qDebug() << Q_FUNC_INFO << 1;

        if (event->pos().y() > d->sizeHint().height()) {
            if (d->animation) {
                d->animation->setDirection(QAbstractAnimation::Backward);
                d->animation->start();
            }

            d->dismissView();
        }
    }

    QFrame::mousePressEvent(event);
}

void gnomonFormManager::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(event->rect(), dtkThemesEngine::instance()->color("@bg"));
    painter.setPen(dtkThemesEngine::instance()->color("@bgalt"));
    painter.drawLine(event->rect().bottomLeft(), event->rect().bottomRight());

    if(!d->inside)
        return;

    painter.setBrush(dtkThemesEngine::instance()->color("@fgalt"));
    painter.drawRoundedRect(event->rect().width() / 2 - 100, event->rect().height() - 10, 200, 6, 3, 3);
}

void gnomonFormManagerPrivate::deleteAnimation(void)
{
    delete focus_item;
    focus_item = nullptr;

    delete animation;
    animation = nullptr;
}

void gnomonFormManagerPrivate::dismissView(void)
{
    if (view)
        view->hide();

    if (focus_area)
        focus_area->hide();
}

gnomonFormManager *gnomonFormManager::s_instance = nullptr;

//
// gnomonFormManager.cpp ends here
