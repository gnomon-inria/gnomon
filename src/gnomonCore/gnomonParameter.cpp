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

class gnomonCheckBoxParameterPrivate
{
    public:
        gnomonCheckBoxParameterPrivate(bool, const QString&);

    public:
        bool value;
        QString doc;
};

gnomonCheckBoxParameterPrivate::gnomonCheckBoxParameterPrivate(bool value, const QString& doc)
{
    this->value = value;
}

gnomonCheckBoxParameter::gnomonCheckBoxParameter(bool value, const QString& doc) : d(new gnomonCheckBoxParameterPrivate(value, doc))
{
}

QWidget* gnomonCheckBoxParameter::connect(QWidget *parent)
{
    QCheckBox *widget = new QCheckBox(parent);
    parent->connect(widget, &QCheckBox::stateChanged,
                    [=](bool value) { d->value = value; } );
    if(d->value) {
        widget->setCheckState(Qt::Checked);
    } else {
        widget->setCheckState(Qt::Unchecked);
    }
    return widget;
}

bool gnomonCheckBoxParameter::value() const
{ return d->value; }

class gnomonLineEditParameterPrivate
{
    public:
        gnomonLineEditParameterPrivate(const QString&, const QString&);

    public:
        QString value;
        QString doc;
};

gnomonLineEditParameterPrivate::gnomonLineEditParameterPrivate(const QString& value, const QString& doc)
{
    this->value = value;
}

gnomonLineEditParameter::gnomonLineEditParameter(const QString& value, const QString& doc) : d(new gnomonLineEditParameterPrivate(value, doc))
{
}

QWidget* gnomonLineEditParameter::connect(QWidget *parent)
{
    QLineEdit *widget = new QLineEdit(parent);
    parent->connect(widget, &QLineEdit::textChanged,
                    [=](QString value) { d->value = value; } );
    widget->->setText(d->value);
    return widget;
}

QString gnomonLineEditParameter::value() const
{ return d->value; }