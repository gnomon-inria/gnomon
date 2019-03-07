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

#include "gnomonImageManagerData.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

QWidget *gnomonImageManagerDataCreate(const QVariant& v)
{
    QLabel *label = new QLabel;

    if(v.type() == QVariant::Int)
        label->setText(QString::number(v.toInt()));

    if(v.type() == QVariant::Double)
        label->setText(QString::number(v.toDouble()));

    if(v.type() == QVariant::String)
        label->setText(v.toString());

    if(v.type() == QVariant::Color) {

        QPixmap pixmap(16, 16);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::white);
        painter.setBrush(v.value<QColor>());
        painter.drawRoundedRect(0, 0, 16, 16, 8, 8);

        label->setPixmap(pixmap);
    }

    return label;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerDataPrivate : public QScrollArea
{
public:
     gnomonImageManagerDataPrivate(gnomonImageManagerData *data);
    ~gnomonImageManagerDataPrivate(void);

public:
    QWidget *contents;
};

gnomonImageManagerDataPrivate::gnomonImageManagerDataPrivate(gnomonImageManagerData *data) : QScrollArea(0)
{
    QFormLayout *layout = new QFormLayout;

    for(QString key : data->data.keys())
        layout->addRow(key, gnomonImageManagerDataCreate(data->data.value(key)));

    this->contents = new QWidget(this);
    this->contents->setLayout(layout);

    this->setWidget(this->contents);
    this->setWidgetResizable(true);
}

gnomonImageManagerDataPrivate::~gnomonImageManagerDataPrivate(void)
{

}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonImageManagerData::gnomonImageManagerData(QObject *parent) : QObject(parent)
{

}

gnomonImageManagerData::~gnomonImageManagerData(void)
{

}

QScrollArea *gnomonImageManagerData::compute(void)
{
    return new gnomonImageManagerDataPrivate(this);
}

//
// gnomonImageManagerData.cpp ends here
