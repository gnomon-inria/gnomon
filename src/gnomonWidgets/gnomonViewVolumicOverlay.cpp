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

#include "gnomonViewVolumicOverlay.h"

gnomonViewVolumicOverlayIcon::gnomonViewVolumicOverlayIcon(QWidget *parent) : QLabel(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize gnomonViewVolumicOverlayIcon::sizeHint(void) const
{
    return QSize(24, 24);
}

void gnomonViewVolumicOverlayIcon::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumicOverlayText::gnomonViewVolumicOverlayText(QString text, QWidget *parent) : QLabel(text, parent)
{
    // this->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // this->setScaledContents(true);
}

void gnomonViewVolumicOverlayText::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

QSize gnomonViewVolumicOverlayText::sizeHint(void) const
{
    QFontMetrics metrics(qApp->font());

    qDebug() << Q_FUNC_INFO << metrics.width(this->text());

    return QSize(metrics.width(this->text()), 24);
}


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(fa::icon icon, QString text, QWidget *parent) : QFrame(parent)
{
    this->pixmap = false;
    this->icon = icon;

    QColor color = Qt::gray;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", color);

    if (!text.isEmpty()) {
        qDebug() << "Creating overlay text for" << text;
        this->label_text  = new gnomonViewVolumicOverlayText(text, this);
    }

    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(this->font->icon(icon).pixmap(24, 24));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    if(!text.isEmpty())
        layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setStyleSheet("background: none; border: 1px solid red;");

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);

    if(this->label_text)
        connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);

    int w = this->label_icon->sizeHint().width();

    qDebug() << Q_FUNC_INFO << this->label_icon->sizeHint().width();

    if(this->label_text) {

        qDebug() << Q_FUNC_INFO << "Adding label text width" << this->label_text->sizeHint().width();

        w += 20;
        w += this->label_text->sizeHint().width();
    } else {
        w += 5;
    }

    qDebug() << Q_FUNC_INFO << w;

    this->setFixedWidth(w);
}

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(const QString& path_on, const QString& path_off, QString text, QWidget *parent) : QFrame(parent)
{
    this->pixmap = true;

    this->path_on  = path_on;
    this->path_off = path_off;

    QColor color = Qt::gray;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", color);

    if(!text.isEmpty())
        this->label_text  = new gnomonViewVolumicOverlayText(text, this);

    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(QPixmap(path_off));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    if(!text.isEmpty())
        layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);

    if(this->label_text)
        connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);

    int w = this->label_icon->sizeHint().width();

    if(this->label_text) {
        w += 20;
        w += this->label_text->sizeHint().width();
    } else {
        w += 5;
    }

    qDebug() << Q_FUNC_INFO << w;

    this->setFixedWidth(w);
}

gnomonViewVolumicOverlay::~gnomonViewVolumicOverlay(void)
{

}

void gnomonViewVolumicOverlay::changePaths(const QString& path_on, const QString& path_off)
{
    this->path_on = path_on;
    this->path_off = path_off;
}

void gnomonViewVolumicOverlay::changeColor(const QColor& color)
{
    this->font->setDefaultOption("color", color);
    this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
}

void gnomonViewVolumicOverlay::changeIcon(fa::icon icon)
{
    this->icon = icon;
    this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
}

void gnomonViewVolumicOverlay::toggle(bool toggled)
{
    this->toggled = toggled;

    if(this->toggled) {;
        QColor color = Qt::white;
        if(this->pixmap) {
            this->label_icon->setPixmap(QPixmap(path_on));
        } else {
            this->font->setDefaultOption("color", color);
            this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
        }
    } else {
        QColor color = Qt::gray;
        if(this->pixmap) {
            this->label_icon->setPixmap(QPixmap(path_off));
        } else  {
            this->font->setDefaultOption("color", color);
            this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
        }
    }
}

void gnomonViewVolumicOverlay::activate(bool activated)
{
    this->activated = activated;

    if(!this->label_text)
        return;

    if(this->activated) {
        this->label_text->setStyleSheet("background: white; border-radius: 3px;");
    } else {
        this->label_text->setStyleSheet("background : none;");
    }
}

bool gnomonViewVolumicOverlay::isActivated(void) const
{
    return this->activated;
}

bool gnomonViewVolumicOverlay::isToggled(void) const
{
    return this->toggled;
}

QString gnomonViewVolumicOverlay::text(void) const
{
    if(this->label_text)
        return this->label_text->text();

    return QString();
}

//
// gnomonViewVolumicOverlay.cpp ends here
