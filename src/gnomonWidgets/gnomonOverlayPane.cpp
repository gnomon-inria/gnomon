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

#include <gnomonFonts>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonOverlayPaneToggle : public QLabel
{
    Q_OBJECT

public:
     gnomonOverlayPaneToggle(QWidget *parent = nullptr);
    ~gnomonOverlayPaneToggle(void);

signals:
    void toggle(void);

protected:
    void mousePressEvent(QMouseEvent *);

public:
    gnomonFontAwesome *font;
    bool toggled = false;
};

gnomonOverlayPaneToggle::gnomonOverlayPaneToggle(QWidget *parent) : QLabel(parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", QColor("#ffffff"));

    this->setAlignment(Qt::AlignCenter);
    this->setPixmap(this->font->icon(fa::chevronleft).pixmap(16, 16));
    this->setFixedWidth(20);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

gnomonOverlayPaneToggle::~gnomonOverlayPaneToggle(void)
{

}

void gnomonOverlayPaneToggle::mousePressEvent(QMouseEvent *)
{
    this->toggled = !this->toggled;

    this->font->setDefaultOption("color", QColor("#ffffff"));

    emit toggle();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonOverlayPanePrivate
{
public:
    int width;
    int size = 320;

public:
    bool on = false;
    bool deactivate = false;

public:
    gnomonOverlayPaneToggle *toggle;

public:
    QVBoxLayout *layout;

public:
    QPropertyAnimation *animation;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

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

    d->toggle = new gnomonOverlayPaneToggle(this);
    d->on = d->toggle->toggled;

    QWidget *widget = new QWidget(this);
    widget->setLayout(d->layout);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setAlignment(Qt::AlignTop);
    scroll->setWidgetResizable(true);
    scroll->setWidget(widget);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->toggle);
    layout->addWidget(scroll);

    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);

    connect(d->toggle, SIGNAL(toggle()), this, SLOT(toggle()));

    if(qApp->screens().first()->size().width() > 2000) {
        d->size = 620;
    }

    this->toggle();
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

    qDebug()<<Q_FUNC_INFO<<d->toggle->toggled<<d->on;

    d->toggle->setPixmap(d->toggle->font->icon(d->toggle->toggled ? fa::chevronright : fa::chevronleft).pixmap(16, 16));

    qlonglong stt = d->on ? d->size :  20;
    qlonglong end = d->on ?  20     : d->size;

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

void gnomonOverlayPane::clearLayout(void)
{
    QLayoutItem *child;

    while ((child = d->layout->takeAt(0)) != 0) {
    }
}

// ///////////////////////////////////////////////////////////////////

QColor gnomonOverlayPane::color = QColor("#242525");

// ///////////////////////////////////////////////////////////////////

#include "gnomonOverlayPane.moc"

//
// gnomonOverlayPane.cpp ends here
