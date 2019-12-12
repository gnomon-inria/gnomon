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

#include "gnomonMessageBoard.h"

#include <dtkThemes>

gnomonMessageBoard::gnomonMessageBoard(QWidget *parent) : QWidget(parent)
{
    this->message_label = new QLabel("");

    this->message_label->setAlignment(Qt::AlignCenter);
//    QColor color = dtkThemesEngine::instance()->color("@base1");
//    QString stylesheet = QString("color:rgb(%1,%2,%3); font: bold 32pt;").arg(color.red()).arg(color.green()).arg(color.blue());
//    this->message_label->setStyleSheet(stylesheet);
    this->message_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    this->message_label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(this->message_label,Qt::AlignCenter);

    this->setLayout(layout);
}

gnomonMessageBoard::~gnomonMessageBoard(void)
{

}

void gnomonMessageBoard::setMessage(const QString& text)
{
    this->message_label->setText(text);
}

//
// gnomonMessageBoard.cpp ends here