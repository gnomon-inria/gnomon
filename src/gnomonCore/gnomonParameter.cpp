#include "gnomonParameter.h"

class gnomonSpinBoxParameterPrivate
{
    public:
        gnomonSpinBoxParameterPrivate(int, int, int, const QString&);

    public:
        int value;
        int min;
        int max;
        QString doc;
};

gnomonSpinBoxParameterPrivate::gnomonSpinBoxParameterPrivate(int value, int min, int max, const QString& doc)
{
    this->value = value;
    this->min = min;
    this->max = max;
    this->doc = doc;
}

gnomonSpinBoxParameter::gnomonSpinBoxParameter(int value, int min, int max, const QString& doc) : d(new gnomonSpinBoxParameterPrivate(value, min, max, doc))
{
}

QWidget* gnomonSpinBoxParameter::connect(QWidget *parent)
{
    QSpinBox *widget = new QSpinBox(parent);
    widget->setMinimum(d->min);
    widget->setMaximum(d->max);
    parent->connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    [=](int value) { d->value = value; } );
    widget->setValue(d->value);
    return widget;
}

int gnomonSpinBoxParameter::value() const
{ return d->value; }

class gnomonDoubleSpinBoxParameterPrivate
{
    public:
        gnomonDoubleSpinBoxParameterPrivate(double, double, double, const QString&);

    public:
        double value;
        double min;
        double max;
        QString doc;
};

gnomonDoubleSpinBoxParameterPrivate::gnomonDoubleSpinBoxParameterPrivate(double value, double min, double max, const QString& doc)
{
    this->value = value;
    this->min = min;
    this->max = max;
    this->doc = doc;
}

gnomonDoubleSpinBoxParameter::gnomonDoubleSpinBoxParameter(double value, double min, double max, const QString& doc) : d(new gnomonDoubleSpinBoxParameterPrivate(value, min, max, doc))
{
}

QWidget* gnomonDoubleSpinBoxParameter::connect(QWidget *parent)
{
    QDoubleSpinBox *widget = new QDoubleSpinBox(parent);
    widget->setMinimum(d->min);
    widget->setMaximum(d->max);
    parent->connect(widget, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](int value) { d->value = value; } );
    widget->setValue(d->value);
    return widget;
}

double gnomonDoubleSpinBoxParameter::value() const
{ return d->value; }