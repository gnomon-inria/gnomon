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

#include "gnomonStringListEditor.h"
#include <cmath>

class gnomonStringListEditorPrivate
{
public:
    QString name;
    QStringList value;


public:
    QStringList values;

public:
    QLabel *label_name;
    QMap<QString, QCheckBox *> checkbox_value;
    QVBoxLayout *check_layout;
    QWidget *checkbox_widget;
};

gnomonStringListEditor::gnomonStringListEditor(QWidget *parent) : QWidget(parent), d(new gnomonStringListEditorPrivate)
{
    // this->setMinimumSize(QSize(800, 145));
    d->value = QStringList{""};
    d->values = QStringList{""};
    d->name = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    d->label_name = new QLabel(d->name);
    // layout->addWidget(d->label_name);

    d->checkbox_widget = new QWidget();
    d->check_layout = new QVBoxLayout(d->checkbox_widget);
    d->check_layout->setContentsMargins(0, 0, 0, 0);

    for (const auto& v : d->values) {
        d->checkbox_value[v] = new QCheckBox(v);
        if (d->value.contains(v)) {
            d->checkbox_value[v]->setCheckState(Qt::Checked);
        } else {
            d->checkbox_value[v]->setCheckState(Qt::Unchecked);
        }
        d->check_layout->addWidget(d->checkbox_value[v]);
        connect(d->checkbox_value[v], &QCheckBox::stateChanged, [=] () {
            emit this->valueChanged(this->value());
        });
    }

    layout->addWidget(d->checkbox_widget);
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

gnomonStringListEditor::~gnomonStringListEditor(void)
{
    delete d;

    d = NULL;
}

const QString& gnomonStringListEditor::name(void) const
{
    return d->name;
}

const QStringList& gnomonStringListEditor::value(void) const
{
    d->value.clear();
    for (const auto& v : d->values) {
        if (d->checkbox_value[v]->checkState() == Qt::Checked) {
            d->value.append(v);
        }
    }
    return d->value;
}

void gnomonStringListEditor::setName(const QString& name)
{
    d->name = name;
    d->label_name->setText(d->name);
}

void gnomonStringListEditor::setValue(const QStringList& value)
{
    for (const auto& v : d->values) {
        if (value.contains(v)) {
            d->checkbox_value[v]->setCheckState(Qt::Checked);
        } else {
            d->checkbox_value[v]->setCheckState(Qt::Unchecked);
        }
    }
    d->value = this->value();
}

void gnomonStringListEditor::setValues(const QStringList& values)
{
    for (const auto& v : d->values) {
        QCheckBox *forDeletion = d->checkbox_value[v];
        d->check_layout->removeWidget(forDeletion);
        forDeletion->disconnect();
        delete forDeletion;
    }
    d->checkbox_value.clear();

    d->values = values;
    for (const auto& v : d->values) {
        d->checkbox_value[v] = new QCheckBox(v);
        if (d->value.contains(v)) {
            d->checkbox_value[v]->setCheckState(Qt::Checked);
        } else {
            d->checkbox_value[v]->setCheckState(Qt::Unchecked);
        }
        d->check_layout->addWidget(d->checkbox_value[v]);
        connect(d->checkbox_value[v], &QCheckBox::stateChanged, [=] () {
            emit this->valueChanged(this->value());
        });
    }
    d->value = this->value();
}


//
// gnomonStringListEditor.cpp ends here
