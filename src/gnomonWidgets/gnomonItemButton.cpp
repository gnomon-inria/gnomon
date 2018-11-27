#include "gnomonItemButton.h"

#include <gnomonFonts>

gnomonItemButton::gnomonItemButton(const QColor& color, int icon, QWidget *parent) : QLabel(parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", color);

    this->setPixmap(this->font->icon(icon).pixmap(16, 16));

    this->setStyleSheet("background: none; border: none;");
}

gnomonItemButton::~gnomonItemButton(void)
{

}

void gnomonItemButton::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
