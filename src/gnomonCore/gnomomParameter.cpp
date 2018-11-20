class guiSpinBoxParameterPrivate
{
    public:
        guiSpinBoxParameterPrivate(int, int, const QString&);

    public:
        int min;
        int max;
        QString doc;
};

guiSpinBoxParameterPrivate::guiSpinBoxParameterPrivate(int min, int max, const QString& doc)
{
    this->min = min;
    this->max = max;
    this->doc = doc;
}

guiSpinBoxParameter::guiSpinBoxParameter(int min, int max, const QString& doc) : d(new guiSpinBoxParameter(min, max, doc))
{
}

QWidget* guiSpinBoxParameter::connect(QWidget *parent, void (func*)(const QString&, const QVariant&))
{
    QSpinBox *widget = new QSpinBox(parent);
    widget->setMinimum(d->min);
    widget->setMaximum(d->min);
    parent->connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                [=](int value){ func(key, value); });
}
