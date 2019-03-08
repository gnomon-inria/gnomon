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

#include <gnomonFonts>

gnomonFormManagerItem::gnomonFormManagerItem(const QColor& color, const QPixmap& thumbnail, gnomonFormManagerPrivate *parent) : QLabel(parent)
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

    // pix = thumbnail;
    // this->setPixmap(pix);
    this->image = thumbnail;

    this->thumbnail = thumbnail.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->transparent_thumbnail = thumbnail.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->transparent_thumbnail.fill();
    
    QPainter painter;
    painter.begin(&transparent_thumbnail);
    painter.setOpacity(0.5);
    // painter.drawPixmap(0, 0, *this->pixmap());
    painter.drawPixmap(0, 0, this->thumbnail);
    painter.end();

    this->setPixmap(this->thumbnail);

    this->setStyleSheet(QString("border: 1px solid rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

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

    } else {

        emit clicked();
    }
}

//
// gnomonFormManagerItem.cpp ends here
