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

#include "gnomonStringEditor.h"
#include <cmath>

class gnomonStringEditorPrivate
{
public:
    QString name;
    QString value;

public:
    QStringList list;

public:
    QLabel *label_name;
    QComboBox *combobox_value;
};

gnomonStringEditor::gnomonStringEditor(QWidget *parent) : QWidget(parent), d(new gnomonStringEditorPrivate)
{
    // this->setMinimumSize(QSize(800, 145));
    d->list.append("");

    d->value = "";
    d->name = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->label_name = new QLabel(d->name);
    layout->addWidget(d->label_name);

    d->combobox_value = new QComboBox();
    for (const auto& s : d->list)
    {
        d->combobox_value->addItem(s);
    }
    d->combobox_value->setCurrentText(d->value);
    layout->addWidget(d->combobox_value);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(d->combobox_value, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), this, &gnomonStringEditor::valueChanged);
}

gnomonStringEditor::~gnomonStringEditor(void)
{
    delete d;

    d = NULL;
}

const QString& gnomonStringEditor::name(void) const
{
    return d->name;
}

const QString& gnomonStringEditor::value(void) const
{
    d->value = d->combobox_value->currentText();
    return d->value;
}

void gnomonStringEditor::setName(const QString& name)
{
    d->name = name;
    d->label_name->setText(d->name);
}

void gnomonStringEditor::setList(const QStringList& list)
{
    d->list = list;
    d->combobox_value->clear();
    for (const auto& s : d->list)
    {
        d->combobox_value->addItem(s);
    }
    d->combobox_value->setCurrentText(d->value);
}

void gnomonStringEditor::setValue(const QString& value)
{
    d->value = value;
    d->combobox_value->setCurrentText(d->value);
}



//
// gnomonStringEditor.cpp ends here
