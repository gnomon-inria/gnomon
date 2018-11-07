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

#include "gnomonOverlayPane.h"

class gnomonOverlayPanePrivate
{
public:
    int width;

public:
    bool on = false;
    bool deactivate = false;

public:
    QVBoxLayout *layout;

public:
    QPropertyAnimation *animation;
};

gnomonOverlayPane::gnomonOverlayPane(QWidget *parent) : QFrame(parent)
{
    d = new gnomonOverlayPanePrivate;
    d->width = 0;

    d->animation = new QPropertyAnimation(this, "width");
    d->animation->setDuration(500);
    d->animation->setEasingCurve(QEasingCurve::OutQuad);

    d->layout = new QVBoxLayout(this);
    d->layout->setAlignment(Qt::AlignTop);
    d->layout->setContentsMargins(0, 0, 0, 0);

    QWidget *widget = new QWidget(this);
    widget->setLayout(d->layout);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scroll);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

gnomonOverlayPane::~gnomonOverlayPane(void)
{
    delete d;
}

int gnomonOverlayPane::width(void)
{
    return d->width;
}

void gnomonOverlayPane::setWidth(int width)
{
    d->width = width;

    this->setFixedWidth(d->width);

    emit widthChanged();
}

void gnomonOverlayPane::toggle(void)
{
    if (d->deactivate)
        return;

    qlonglong stt = d->on ? 300 :   0;
    qlonglong end = d->on ?   0 : 300;

    if (d->animation->state() == QAbstractAnimation::Running) {
        d->animation->stop();
        stt = d->animation->currentValue().toInt();
    }

    d->animation->setStartValue(stt);
    d->animation->setEndValue(end);
    d->animation->start();

    d->on = !d->on;
}

void gnomonOverlayPane::deactivate(void)
{
    d->deactivate = true;
}

void gnomonOverlayPane::setColor(QColor color)
{
    gnomonOverlayPane::color = color;
}

void gnomonOverlayPane::enterEvent(QEvent *)
{
    emit entered();
}

void gnomonOverlayPane::leaveEvent(QEvent *)
{
    emit left();
}

void gnomonOverlayPane::paintEvent(QPaintEvent *event)
{
    QRect rect = event->rect();
    rect.setWidth(d->width);

    QPainter painter(this);
    painter.fillRect(rect, QColor(color.red(), color.green(), color.blue(), 196));
    painter.setPen(color);
    painter.drawLine(rect.topRight(), rect.bottomRight());
}

void gnomonOverlayPane::addLayout(QLayout *layout)
{
    d->layout->addLayout(layout);
}

void gnomonOverlayPane::addWidget(QWidget *widget)
{
    d->layout->addWidget(widget);
}

void gnomonOverlayPane::clear(void)
{
    QLayoutItem *child;

    while ((child = d->layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

QColor gnomonOverlayPane::color = QColor("#242525");

//
// gnomonOverlayPane.cpp ends here
