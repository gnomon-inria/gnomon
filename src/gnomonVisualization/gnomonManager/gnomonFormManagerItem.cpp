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

#include "gnomonFormManager_p.h"
#include "gnomonFormManagerItem.h"
#include "gnomonItemButton.h"

#include <dtkFonts>

gnomonFormManagerItem::gnomonFormManagerItem(const QColor& color, const QPixmap& thumbnail, int n_times, gnomonFormManagerPrivate *parent) : QLabel(parent)
{
    this->parent = parent;

    int size  = 100;
    int pos   = size - 20;

    this->button_destroy = new gnomonItemButton(color, fa::times, this);
    this->button_destroy->move(pos, pos);
    this->button_destroy->setVisible(false);

    this->button_save = new gnomonItemButton(color, fa::save, this);
    this->button_save->move(5, pos);
    this->button_save->setVisible(false);

    int space =   3;

    this->image = thumbnail;

    if (n_times>1) {
        this->setBaseSize(size, size);

        QPixmap pix(100,100);
        pix.fill(Qt::transparent);

        QPainter paint(&pix);
        paint.setPen(color);
        paint.drawRect(2*space,       0, size-2*space-1, size-2*space-1);
        paint.fillRect(2*space+1,     1, size-2*space-2, size-2*space-2, Qt::black);
        paint.drawRect(space,     space, size-2*space-1, size-2*space-1);
        paint.fillRect(space+1, space+1, size-2*space-2, size-2*space-2, Qt::black);
        paint.drawRect(0,       2*space, size-2*space-1, size-2*space-1);
        QRectF target(1,      2*space+1, size-2*space-2, size-2*space-2);
        QRectF source(0,              0, size-2*space-3, size-2*space-3);
        paint.drawImage(target, thumbnail.scaled(size-2*space-3, size-2*space-3, Qt::KeepAspectRatio).toImage(), source);
        paint.end();

//        this->setPixmap(pix);
        this->thumbnail = pix;

        this->transparent_thumbnail = pix;
        this->transparent_thumbnail.fill();
    } else {

        this->thumbnail = thumbnail.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        this->transparent_thumbnail = thumbnail.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->transparent_thumbnail.fill();
    }

    QPainter painter;
    painter.begin(&transparent_thumbnail);
    painter.setOpacity(0.5);
    // painter.drawPixmap(0, 0, *this->pixmap());
    painter.drawPixmap(0, 0, this->thumbnail);
    painter.end();

    this->setPixmap(this->thumbnail);

    if (n_times==1) {
        this->setStyleSheet(QString("border: 1px solid rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    }

    connect(this->button_destroy, SIGNAL(clicked()), this, SIGNAL(destroy()));
    connect(this->button_save, SIGNAL(clicked()), this, SIGNAL(save()));

    this->setMouseTracking(true);
}

gnomonFormManagerItem::~gnomonFormManagerItem(void)
{

}

void gnomonFormManagerItem::enterEvent(QEvent *)
{
    this->button_destroy->setVisible(true);
    this->button_save->setVisible(true);
    this->setPixmap(this->transparent_thumbnail);
}

void gnomonFormManagerItem::leaveEvent(QEvent *)
{
    this->button_destroy->setVisible(false);
    this->button_save->setVisible(false);
    this->setPixmap(this->thumbnail);
}

void gnomonFormManagerItem::mousePressEvent(QMouseEvent *)
{
    if(parent->state == gnomonFormManagerPrivate::Collapsed) {

        QMimeData *mimeData = new QMimeData;
        mimeData->setText(QString(":%1").arg(this->id));

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(*(this->pixmap()));
        drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

        Qt::DropAction dropAction = drag->exec();

        Q_UNUSED(dropAction);

    } else {

        emit clicked();
    }
}

//
// gnomonFormManagerItem.cpp ends here
