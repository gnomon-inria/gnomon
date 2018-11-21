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
    widget->setToolTip(d->doc);
    widget->setMinimum(d->min);
    widget->setMaximum(d->max);
    parent->connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    [=](int value) { d->value = value; } );
    widget->setValue(d->value);
    return widget;
}

int gnomonSpinBoxParameter::value(void) const
{ return d->value; }

void gnomonSpinBoxParameter::setValue(int value)
{ d->value = value; }

class gnomonDoubleSpinBoxParameterPrivate
{
    public:
        gnomonDoubleSpinBoxParameterPrivate(double, double, double, int, const QString&);

    public:
        double value;
        double min;
        double max;
        int decimals;
        QString doc;
};

gnomonDoubleSpinBoxParameterPrivate::gnomonDoubleSpinBoxParameterPrivate(double value, double min, double max, int decimals, const QString& doc)
{
    this->value = value;
    this->min = min;
    this->max = max;
    this->decimals = decimals;
    this->doc = doc;
}

gnomonDoubleSpinBoxParameter::gnomonDoubleSpinBoxParameter(double value, double min, double max, int decimals, const QString& doc) : d(new gnomonDoubleSpinBoxParameterPrivate(value, min, max, decimals, doc))
{
}

QWidget* gnomonDoubleSpinBoxParameter::connect(QWidget *parent)
{
    QDoubleSpinBox *widget = new QDoubleSpinBox(parent);
    widget->setToolTip(d->doc);
    widget->setMinimum(d->min);
    widget->setMaximum(d->max);
    widget->setDecimals(d->decimals);
    widget->setSingleStep(pow(10, -d->decimals));
    parent->connect(widget, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](int value) { d->value = value; } );
    widget->setValue(d->value);
    return widget;
}

double gnomonDoubleSpinBoxParameter::value(void) const
{ return d->value; }

void gnomonDoubleSpinBoxParameter::setValue(double value)
{ d->value = value; }

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
    widget->setToolTip(d->doc);
    parent->connect(widget, &QCheckBox::stateChanged,
                    [=](bool value) { d->value = value; } );
    if(d->value) {
        widget->setCheckState(Qt::Checked);
    } else {
        widget->setCheckState(Qt::Unchecked);
    }
    return widget;
}

bool gnomonCheckBoxParameter::value(void) const
{ return d->value; }

void gnomonCheckBoxParameter::setValue(bool value)
{ d->value = value; }

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
    widget->setToolTip(d->doc);
    parent->connect(widget, &QLineEdit::textChanged,
                    [=](QString value) { d->value = value; } );
    widget->setText(d->value);
    return widget;
}

QString gnomonLineEditParameter::value(void) const
{ return d->value; }

void gnomonLineEditParameter::setValue(const QString& value)
{ d->value = value; }

class gnomonComboBoxParameterPrivate
{
    public:
        gnomonComboBoxParameterPrivate(const QString&, const QStringList&, const QString&);

    public:
        QString value;
        QStringList values;
        QString doc;
};

gnomonComboBoxParameterPrivate::gnomonComboBoxParameterPrivate(const QString& value, const QStringList& values, const QString& doc)
{
    this->value = value;
    this->values = values;
}

gnomonComboBoxParameter::gnomonComboBoxParameter(const QString& value, const QStringList& values, const QString& doc) : d(new gnomonComboBoxParameterPrivate(value, values, doc))
{
}

QWidget* gnomonComboBoxParameter::connect(QWidget *parent)
{
    QComboBox *widget = new QComboBox(parent);
    widget->setToolTip(d->doc);
    for (auto it = d->values.begin(), it_end = d->values.end(); it != it_end; ++it) {
        widget->addItem(*it);
    }
    parent->connect(widget, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
                    [=](QString value) { d->value = value; } );
    widget->setCurrentText(d->value);
    return widget;
}

QString gnomonComboBoxParameter::value(void) const
{ return d->value; }

void gnomonComboBoxParameter::setValue(const QString& value)
{ d->value = value; }