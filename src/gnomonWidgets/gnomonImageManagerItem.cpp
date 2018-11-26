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

#include "gnomonImageManager_p.h"
#include "gnomonImageManagerItem.h"
#include "gnomonItemButton.h"

#include <gnomonFonts>

gnomonImageManagerItem::gnomonImageManagerItem(const QColor& color, const QPixmap& thumbnail, gnomonImageManagerPrivate *parent, bool multi_images) : QLabel(parent)
{
    this->parent = parent;

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

    if (!multi_images) {
        this->setStyleSheet(QString("border: 1px solid rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    }

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
    if(parent->state == gnomonImageManagerPrivate::Collapsed) {

        QMimeData *mimeData = new QMimeData;
        mimeData->setText(QString(":%1").arg(this->id));

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(*(this->pixmap()));
        drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));

        Qt::DropAction dropAction = drag->exec();

    } else {

        emit clicked();
    }
}

//
// gnomonImageManagerItem.cpp ends here
