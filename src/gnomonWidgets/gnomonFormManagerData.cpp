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

QWidget *gnomonFormManagerDataCreate(const QString& v)
{
    QLabel *label = new QLabel;
    label->setText(v);
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
