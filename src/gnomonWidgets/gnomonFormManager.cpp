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

#include "gnomonFormManager.h"
#include "gnomonFormManager_p.h"
#include "gnomonFormManagerItem.h"
#include "gnomonItemButton.h"
#include "gnomonToolBar.h"

#include <gnomonFonts>
#include <gnomonCore/gnomonAbstractForm>

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
        delete item;
    });

    connect(item, &gnomonFormManagerItem::save, [=] () {
        qDebug()<<"Save form";
    });

    connect(item, &gnomonFormManagerItem::clicked, [=] () {
        q->present(item);
    });

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

void gnomonFormManager::addForm(gnomonAbstractForm * form, const QColor& color, const QImage& image)
{
    gnomonFormManagerItem *item = d->create(form, color, image);
    item->id = d->item_counter++;

    d->forms.insert(item, form);
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
    qDebug()<<"Focus";
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
