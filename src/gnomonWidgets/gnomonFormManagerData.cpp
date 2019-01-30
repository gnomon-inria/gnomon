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

#include "gnomonFormManagerData.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

QWidget *gnomonFormManagerDataCreate(const QVariant& v)
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

class gnomonFormManagerDataPrivate : public QScrollArea
{
public:
     gnomonFormManagerDataPrivate(gnomonFormManagerData *data);
    ~gnomonFormManagerDataPrivate(void);

public:
    QWidget *contents;
};

gnomonFormManagerDataPrivate::gnomonFormManagerDataPrivate(gnomonFormManagerData *data) : QScrollArea(0)
{
    QFormLayout *layout = new QFormLayout;

    for(QString key : data->data.keys())
        layout->addRow(key, gnomonFormManagerDataCreate(data->data.value(key)));

    this->contents = new QWidget(this);
    this->contents->setLayout(layout);

    this->setWidget(this->contents);
    this->setWidgetResizable(true);
}

gnomonFormManagerDataPrivate::~gnomonFormManagerDataPrivate(void)
{

}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonFormManagerData::gnomonFormManagerData(QObject *parent) : QObject(parent)
{

}

gnomonFormManagerData::~gnomonFormManagerData(void)
{

}

QScrollArea *gnomonFormManagerData::compute(void)
{
    return new gnomonFormManagerDataPrivate(this);
}

//
// gnomonFormManagerData.cpp ends here
