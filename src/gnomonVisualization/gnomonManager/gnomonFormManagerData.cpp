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
    QVBoxLayout *layout = new QVBoxLayout;

    QFormLayout *metadata_layout = new QFormLayout;

    for(QString key : data->data.keys())
        metadata_layout->addRow(key, gnomonFormManagerDataCreate(data->data.value(key)));

    this->contents = new QWidget(this);
    this->contents->setLayout(layout);

    QString title(data->name);
    title += " (";
    title += data->data_name;
    title += ")";

    QLabel *title_label = new QLabel(title);
    title_label->setAlignment(Qt::AlignCenter);
    title_label->setStyleSheet("font-size: 20px; height: 50px; font-family: Helvetica");

    layout->addWidget(title_label);
    layout->addLayout(metadata_layout);

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
