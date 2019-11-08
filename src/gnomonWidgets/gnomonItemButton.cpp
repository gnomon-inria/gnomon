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

#include "gnomonItemButton.h"

#include <dtkFonts>

gnomonItemButton::gnomonItemButton(const QColor& color, int icon, QWidget *parent) : QLabel(parent)
{
    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", color);

    this->setPixmap(dtkFontAwesome::instance()->icon(icon).pixmap(16, 16));

    this->setStyleSheet("background: none; border: none;");
}

gnomonItemButton::~gnomonItemButton(void)
{

}

void gnomonItemButton::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

// 
// gnomonItemButton.cpp ends here
