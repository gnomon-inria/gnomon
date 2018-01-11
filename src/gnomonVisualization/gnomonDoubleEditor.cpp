// Version: $Id: b3634632a3e3eac8bcc4a3b15a61a881bb42aa64 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonDoubleEditor.h"

#include <cmath>

class gnomonDoubleEditorPrivate
{
public:
    QString name;
    double value;

public:
    double min;
    double max;

public:
    QLabel *label_name;
    QDoubleSpinBox *spinbox_value;
};

gnomonDoubleEditor::gnomonDoubleEditor(QWidget *parent) : QWidget(parent), d(new gnomonDoubleEditorPrivate)
{
    d->min = 0;
    d->max = 1;

    d->value = 1;
    d->name = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->label_name = new QLabel(d->name);
    layout->addWidget(d->label_name);

    d->spinbox_value = new QDoubleSpinBox();
    d->spinbox_value->setRange(d->min,d->max);
    d->spinbox_value->setValue(d->value);

    int dec = floor(-log10((d->max-d->min)/100.));
    double step = pow(10.,-dec);
    d->spinbox_value->setDecimals(dec);
    d->spinbox_value->setSingleStep(step);
    layout->addWidget(d->spinbox_value);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(d->spinbox_value, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &gnomonDoubleEditor::valueChanged);
}

gnomonDoubleEditor::~gnomonDoubleEditor(void)
{
    delete d;

    d = NULL;
}

const QString& gnomonDoubleEditor::name(void) const
{
    return d->name;
}

double gnomonDoubleEditor::value(void) const
{
    d->value = d->spinbox_value->value();
    return d->value;
}

void gnomonDoubleEditor::setName(const QString& name)
{
    d->name = name;
    d->label_name->setText(d->name);
}

void gnomonDoubleEditor::setRange(double min, double max)
{
    d->min = min;
    d->max = max;

    d->spinbox_value->setRange(d->min,d->max);

    int dec = floor(-log10((d->max-d->min)/100.));
    double step = pow(10.,-dec);
    d->spinbox_value->setDecimals(dec);
    d->spinbox_value->setSingleStep(step);

    d->value = d->spinbox_value->value();
}

void gnomonDoubleEditor::setValue(double value)
{
    d->value = value;
    d->spinbox_value->setValue(d->value);
}



//
// gnomonDoubleEditor.cpp ends here
