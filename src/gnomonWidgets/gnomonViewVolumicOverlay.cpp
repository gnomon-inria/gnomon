#include "gnomonViewVolumicOverlay.h"

gnomonViewVolumicOverlayIcon::gnomonViewVolumicOverlayIcon(QWidget *parent) : QLabel(parent) {}

void gnomonViewVolumicOverlayIcon::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

//////////

gnomonViewVolumicOverlayText::gnomonViewVolumicOverlayText(QString text, QWidget *parent) : QLabel(text, parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void gnomonViewVolumicOverlayText::mousePressEvent(QMouseEvent *mouseEvent)
{
    emit clicked();
}

int gnomonViewVolumicOverlayText::textWidth(void) const
{
    QFontMetrics fm(QApplication::font());
    return fm.width(this->text());
}

//////////

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(fa::icon icon, QString text, QWidget *parent) : QWidget(parent)
{
    this->pixmap = false;
    this->icon = icon;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    QColor color = Qt::gray;
    this->font->setDefaultOption("color", color);

    this->label_text  = new gnomonViewVolumicOverlayText(text, this);
    if(text.isEmpty()) this->label_text->setVisible(false);
    this->label_text->resize(this->label_text->textWidth(), this->label_text->height());
    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(this->font->icon(icon).pixmap(24, 24));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setLayout(layout);
    this->setStyleSheet("background: none;");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);
    connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);
}

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(const QString& path_on, const QString& path_off, QString text, QWidget *parent) : QWidget(parent)
{
    this->pixmap = true;

    this->path_on  = path_on;
    this->path_off = path_off;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    QColor color = Qt::gray;
    this->font->setDefaultOption("color", color);

    this->label_text  = new gnomonViewVolumicOverlayText(text, this);
    if(text.isEmpty()) this->label_text->setVisible(false);

    this->label_icon = new gnomonViewVolumicOverlayIcon(this);
    this->label_icon->setPixmap(QPixmap(path_off));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->label_text);
    layout->addWidget(this->label_icon);

    this->setLayout(layout);
    this->setStyleSheet("background: none;");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    connect(this->label_icon, &gnomonViewVolumicOverlayIcon::clicked, this, &gnomonViewVolumicOverlay::iconClicked);
    connect(this->label_text, &gnomonViewVolumicOverlayText::clicked, this, &gnomonViewVolumicOverlay::textClicked);
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

int gnomonViewVolumicOverlay::textWidth(void) const
{
    return this->label_text->textWidth();
}
