#include "gnomonViewVolumicOverlay.h"

gnomonViewVolumicOverlayIcon::gnomonViewVolumicOverlayIcon(QWidget *parent) : QLabel(parent) {}

void gnomonViewVolumicOverlayIcon::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

//////////

gnomonViewVolumicOverlayText::gnomonViewVolumicOverlayText(QString text, QWidget *parent) : QLabel(text, parent) {}

void gnomonViewVolumicOverlayText::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

//////////

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(fa::icon icon, QString text, QWidget *parent) : QWidget(parent)
{
    this->default_color = Qt::gray;

    this->icon = icon;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", this->default_color);

    this->label_text  = new gnomonViewVolumicOverlayText(text, this);
    if(text.isEmpty()) this->label_text->setVisible(false);
    this->label_text->setContentsMargins(0,0,0,0);

    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(this->font->icon(icon).pixmap(24, 24));
    this->label_icon->setContentsMargins(0,0,0,0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->label_icon);
    layout->addWidget(this->label_text);

    this->setLayout(layout);
    this->setContentsMargins(0,0,0,0);
    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);
    connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);
}

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(const QString& path, QString text, QWidget *parent) : QWidget(parent)
{
    this->default_color = Qt::gray;

    this->icon = icon;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", this->default_color);

    this->label_text  = new gnomonViewVolumicOverlayText(text, this);
    if(text.isEmpty()) this->label_text->setVisible(false);
    this->label_text->setContentsMargins(0,0,0,0);

    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(QPixmap(path));
    this->label_icon->setContentsMargins(0,0,0,0);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setLayout(layout);
    this->setContentsMargins(0,0,0,0);
    this->setStyleSheet("background: none;");

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);
    connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);
}

gnomonViewVolumicOverlay::~gnomonViewVolumicOverlay(void)
{

}

void gnomonViewVolumicOverlay::changePath(const QString& path)
{
    this->label_icon->setPixmap(QPixmap(path));
}

void gnomonViewVolumicOverlay::toggle(bool toggled)
{
    this->toggled = toggled;
    if(this->toggled) {;
        QColor color = Qt::white;
        this->font->setDefaultOption("color", color);
        this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
    } else {
        QColor color = Qt::gray;
        this->font->setDefaultOption("color", color);
        this->label_icon->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
    }
}

void gnomonViewVolumicOverlay::activate(bool activated)
{
    this->activated = activated;
    if(this->activated) {
        this->label_text->setStyleSheet("background: white;  border-radius : 3px");
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
    return this->label_text->text();
}
