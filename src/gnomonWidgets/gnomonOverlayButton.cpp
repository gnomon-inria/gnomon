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

#include "gnomonOverlayButton.h"

#include <dtkThemes>

gnomonOverlayButtonIcon::gnomonOverlayButtonIcon(QWidget *parent) : QLabel(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize gnomonOverlayButtonIcon::sizeHint(void) const
{
    return QSize(24, 24);
}

void gnomonOverlayButtonIcon::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonOverlayButtonText::gnomonOverlayButtonText(QString text, QWidget *parent) : QLabel(text, parent)
{
    // this->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // this->setScaledContents(true);
}

void gnomonOverlayButtonText::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

QSize gnomonOverlayButtonText::sizeHint(void) const
{
    QFontMetrics metrics(qApp->font());

    return QSize(metrics.width(this->text()) + 20, 24);
}


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonOverlayButton::gnomonOverlayButton(QChar qchar, QString text, QWidget *parent) : QFrame(parent)
{
    this->pixmap = true;

    QColor color = Qt::gray;

    if (!text.isEmpty()) {
        this->label_text  = new gnomonOverlayButtonText(text, this);
        this->label_text->setObjectName(qchar);
    }

    this->pix_on = QPixmap(24, 24);
    this->pix_on.fill(Qt::transparent);

    QPainter painter_on(&this->pix_on);
    painter_on.setRenderHint(QPainter::Antialiasing, true);
    painter_on.setPen("#ffae03");
    painter_on.setBrush(QColor("#ffae03"));
    painter_on.drawRoundedRect(2, 2, 20, 20, 4, 4);
    painter_on.setPen("#222222");
    QFont font_on = painter_on.font();
    font_on.setWeight(QFont::Black);
    font_on.setPixelSize(16);
    painter_on.setFont(font_on);
    painter_on.drawText(this->pix_on.rect(), Qt::AlignCenter, QString(qchar));

    this->pix_off = QPixmap(24, 24);
    this->pix_off.fill(Qt::transparent);

    QPainter painter_off(&this->pix_off);
    painter_off.setRenderHint(QPainter::Antialiasing, true);
    painter_off.setPen("#b3b3b3");
    painter_off.setBrush(QColor("#b3b3b3"));
    painter_off.drawRoundedRect(2, 2, 20, 20, 4, 4);
    painter_off.setPen("#222222");
    QFont font_off = painter_off.font();
    font_off.setWeight(QFont::Black);
    font_off.setPixelSize(16);
    painter_off.setFont(font_off);
    painter_off.drawText(this->pix_off.rect(), Qt::AlignCenter, QString(qchar));

    this->label_icon = new gnomonOverlayButtonIcon(this);
    this->label_icon->setPixmap(this->pix_off);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(this->label_icon);
    if(!text.isEmpty())
        layout->addWidget(this->label_text);

    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonOverlayButtonIcon::clicked, this, &gnomonOverlayButton::iconClicked);

    if(this->label_text)
        connect(this->label_text, &gnomonOverlayButtonText::clicked, this, &gnomonOverlayButton::textClicked);

    int w = this->label_icon->sizeHint().width();

    if(this->label_text) {
        w += 20;
        w += this->label_text->sizeHint().width();
    } else {
        w += 4;
    }

    this->setFixedWidth(w);
    this->setFixedHeight(30);
}

gnomonOverlayButton::gnomonOverlayButton(fa::icon icon, QString text, QWidget *parent) : QFrame(parent)
{
    this->pixmap = false;
    this->icon = icon;

    QColor color = Qt::gray;

    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", color);

    if (!text.isEmpty())
        this->label_text  = new gnomonOverlayButtonText(text, this);

    this->label_icon = new gnomonOverlayButtonIcon(this);
    this->label_icon->setPixmap(dtkFontAwesome::instance()->icon(icon).pixmap(24, 24));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    if(!text.isEmpty())
        layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonOverlayButtonIcon::clicked, this, &gnomonOverlayButton::iconClicked);

    if(this->label_text)
        connect(this->label_text, &gnomonOverlayButtonText::clicked, this, &gnomonOverlayButton::textClicked);

    int w = this->label_icon->sizeHint().width();

    if(this->label_text) {
        w += 20;
        w += this->label_text->sizeHint().width();
    } else {
        w += 4;
    }

    this->setFixedWidth(w);
    this->setFixedHeight(30);
}

gnomonOverlayButton::gnomonOverlayButton(const QString& path_on, const QString& path_off, QString text, QWidget *parent) : QFrame(parent)
{
    this->pixmap = true;

    this->path_on  = path_on;
    this->path_off = path_off;

    QColor color = Qt::gray;

    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", color);

    if(!text.isEmpty())
        this->label_text  = new gnomonOverlayButtonText(text, this);

    this->label_icon = new gnomonOverlayButtonIcon(this);
    this->label_icon->setPixmap(QPixmap(path_off));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    if(!text.isEmpty())
        layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonOverlayButtonIcon::clicked, this, &gnomonOverlayButton::iconClicked);

    if(this->label_text)
        connect(this->label_text, &gnomonOverlayButtonText::clicked, this, &gnomonOverlayButton::textClicked);

    int w = this->label_icon->sizeHint().width();

    if(this->label_text) {
        w += 20;
        w += this->label_text->sizeHint().width();
    } else {
        w += 4;
    }

    this->setFixedWidth(w);
    this->setFixedHeight(30);
}

gnomonOverlayButton::~gnomonOverlayButton(void)
{

}

void gnomonOverlayButton::changePaths(const QString& path_on, const QString& path_off)
{
    this->path_on = path_on;
    this->path_off = path_off;
}

void gnomonOverlayButton::changeColor(const QColor& color)
{
    dtkFontAwesome::instance()->setDefaultOption("color", color);
    this->label_icon->setPixmap(dtkFontAwesome::instance()->icon(this->icon).pixmap(24, 24));
}

void gnomonOverlayButton::changeIcon(fa::icon icon)
{
    this->icon = icon;
    this->label_icon->setPixmap(dtkFontAwesome::instance()->icon(this->icon).pixmap(24, 24));
}

void gnomonOverlayButton::toggle(bool toggled)
{
    this->toggled = toggled;

    if(this->toggled) {;
        QColor color = dtkThemesEngine::instance()->color("@fg");

        if (this->pixmap) {
            this->label_icon->setPixmap(QPixmap(path_on));
        } else {
            dtkFontAwesome::instance()->setDefaultOption("color", color);
            this->label_icon->setPixmap(dtkFontAwesome::instance()->icon(this->icon).pixmap(24, 24));
        }
    } else {
        QColor color = dtkThemesEngine::instance()->color("@grey");
        if (this->pixmap) {
            this->label_icon->setPixmap(QPixmap(path_off));
        } else  {
            dtkFontAwesome::instance()->setDefaultOption("color", color);
            this->label_icon->setPixmap(dtkFontAwesome::instance()->icon(this->icon).pixmap(24, 24));
        }
    }
}

void gnomonOverlayButton::activate(bool activated)
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

bool gnomonOverlayButton::isActivated(void) const
{
    return this->activated;
}

bool gnomonOverlayButton::isToggled(void) const
{
    return this->toggled;
}

QString gnomonOverlayButton::text(void) const
{
    if(this->label_text)
        return this->label_text->text();

    return QString();
}

//
// gnomonOverlayButton.cpp ends here
