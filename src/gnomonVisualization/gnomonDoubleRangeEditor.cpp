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

#include "gnomonDoubleRangeEditor.h"
#include <cmath>

class gnomonDoubleRangeEditorPrivate
{
public:
    QString name;
    double valueMin;
    double valueMax;
    QList<double> value;

public:
    double min;
    double max;

public:
    QLabel *label_name;
    QDoubleSpinBox *spinbox_valueMin;
    QDoubleSpinBox *spinbox_valueMax;
    QWidget *spinbox_widget;
};

gnomonDoubleRangeEditor::gnomonDoubleRangeEditor(QWidget *parent) : QWidget(parent), d(new gnomonDoubleRangeEditorPrivate)
{
    // this->setMinimumSize(QSize(800, 145));
    d->min = 0;
    d->max = 1;

    d->valueMin = 0;
    d->valueMax = 1;
    d->value = QList<double>{d->valueMin,d->valueMax};
    d->name = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->label_name = new QLabel(d->name);
    layout->addWidget(d->label_name);

    d->spinbox_valueMin = new QDoubleSpinBox();
    d->spinbox_valueMin->setRange(d->min,d->valueMax);
    d->spinbox_valueMin->setValue(d->valueMin);

    d->spinbox_valueMax = new QDoubleSpinBox();
    d->spinbox_valueMax->setRange(d->valueMin,d->max);
    d->spinbox_valueMax->setValue(d->valueMax);

    int dec = floor(-log10((d->max-d->min)/100.));
    double step = pow(10.,-dec);

    d->spinbox_widget = new QWidget();
    QHBoxLayout *spin_layout = new QHBoxLayout(d->spinbox_widget);

    d->spinbox_valueMin->setDecimals(dec);
    d->spinbox_valueMin->setSingleStep(step);
    spin_layout->addWidget(d->spinbox_valueMin);

    d->spinbox_valueMax->setDecimals(dec);
    d->spinbox_valueMax->setSingleStep(step);
    spin_layout->addWidget(d->spinbox_valueMax);

    layout->addWidget(d->spinbox_widget);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(d->spinbox_valueMin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &gnomonDoubleRangeEditor::valueMinChanged);
    connect(d->spinbox_valueMax, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &gnomonDoubleRangeEditor::valueMaxChanged);
}

gnomonDoubleRangeEditor::~gnomonDoubleRangeEditor(void)
{
    delete d;

    d = NULL;
}

const QString& gnomonDoubleRangeEditor::name(void) const
{
    return d->name;
}

double gnomonDoubleRangeEditor::valueMin(void) const
{
    d->valueMin = d->spinbox_valueMin->value();
    d->spinbox_valueMax->setRange(d->valueMin,d->max);

    return d->valueMin;
}

double gnomonDoubleRangeEditor::valueMax(void) const
{
    d->valueMax = d->spinbox_valueMax->value();
    d->spinbox_valueMin->setRange(d->min,d->valueMax);
    return d->valueMax;
}

const QList<double>& gnomonDoubleRangeEditor::value(void) const
{
    d->valueMin = d->spinbox_valueMin->value();
    d->valueMax = d->spinbox_valueMax->value();

    d->spinbox_valueMin->setRange(d->min,d->valueMax);
    d->spinbox_valueMax->setRange(d->valueMin,d->max);

    d->value = QList<double>{d->valueMin,d->valueMax};
    return d->value;
}

void gnomonDoubleRangeEditor::setName(const QString& name)
{
    d->name = name;
    d->label_name->setText(d->name);
}

void gnomonDoubleRangeEditor::setRange(double min, double max)
{
    d->min = min;
    d->max = max;

    d->spinbox_valueMin->setRange(d->min,d->valueMax);
    d->spinbox_valueMax->setRange(d->valueMin,d->max);

    int dec = floor(-log10((d->max-d->min)/100.));
    double step = pow(10.,-dec);

    d->spinbox_valueMin->setDecimals(dec);
    d->spinbox_valueMin->setSingleStep(step);
    d->valueMin = d->spinbox_valueMin->value();

    d->spinbox_valueMax->setDecimals(dec);
    d->spinbox_valueMax->setSingleStep(step);
    d->valueMax = d->spinbox_valueMax->value();

    d->spinbox_valueMin->setRange(d->min,d->valueMax);
    d->spinbox_valueMax->setRange(d->valueMin,d->max);
}

void gnomonDoubleRangeEditor::setValueMin(double value)
{
    d->valueMin = value;
    d->spinbox_valueMin->setValue(d->valueMin);;
    d->spinbox_valueMax->setRange(d->valueMin,d->max);
}

void gnomonDoubleRangeEditor::setValueMax(double value)
{
    d->valueMax = value;
    d->spinbox_valueMax->setValue(d->valueMax);
    d->spinbox_valueMin->setRange(d->min,d->valueMax);
}



//
// gnomonDoubleRangeEditor.cpp ends here
