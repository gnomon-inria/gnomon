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

#include "gnomonWidgetsParameter.h"

#include "gnomonColor/gnomonCoreParameterColor.h"

#include "gnomonEditor/gnomonColorMapEditor.h"
#include "gnomonEditor/gnomonDoubleRangeEditor.h"
#include "gnomonEditor/gnomonStringListEditor.h"
#include "gnomonEditor/gnomonLookupTableEditor.h"

QWidget *gnomonWidgetsParameter::widget(gnomonCoreParameter *parameter, QWidget *parent)
{
    if (gnomonCoreParameterInt *p = dynamic_cast<gnomonCoreParameterInt *>(parameter)) {
        return gnomonWidgetsParameterInt::widget(p, parent);
    }
    if (gnomonCoreParameterDouble *p = dynamic_cast<gnomonCoreParameterDouble *>(parameter)) {
        return gnomonWidgetsParameterDouble::widget(p, parent);
    }
    if (gnomonCoreParameterIntRange *p = dynamic_cast<gnomonCoreParameterIntRange *>(parameter)) {
        return gnomonWidgetsParameterIntRange::widget(p, parent);
    }
    if (gnomonCoreParameterDoubleRange *p = dynamic_cast<gnomonCoreParameterDoubleRange *>(parameter)) {
        return gnomonWidgetsParameterDoubleRange::widget(p, parent);
    }
    if (gnomonCoreParameterBool *p = dynamic_cast<gnomonCoreParameterBool *>(parameter)) {
        return gnomonWidgetsParameterBool::widget(p, parent);
    }
    if (gnomonCoreParameterText *p = dynamic_cast<gnomonCoreParameterText *>(parameter)) {
        return gnomonWidgetsParameterText::widget(p, parent);
    }
    if (gnomonCoreParameterString *p = dynamic_cast<gnomonCoreParameterString *>(parameter)) {
        return gnomonWidgetsParameterString::widget(p, parent);
    }
    if (gnomonCoreParameterStringList *p = dynamic_cast<gnomonCoreParameterStringList *>(parameter)) {
        return gnomonWidgetsParameterStringList::widget(p, parent);
    }
    if (gnomonCoreParameterFile *p = dynamic_cast<gnomonCoreParameterFile *>(parameter)) {
        return gnomonWidgetsParameterFile::widget(p, parent);
    }
    if (gnomonCoreParameterColorMap *p = dynamic_cast<gnomonCoreParameterColorMap *>(parameter)) {
        return gnomonWidgetsParameterColorMap::widget(p, parent);
    }
    if (gnomonCoreParameterLookupTable *p = dynamic_cast<gnomonCoreParameterLookupTable *>(parameter)) {
        return gnomonWidgetsParameterLookupTable::widget(p, parent);
    }
    return nullptr;
}

QString gnomonWidgetsParameterInt::style = QStringLiteral("spinbox");

QWidget *gnomonWidgetsParameterInt::widget(gnomonCoreParameterInt *parameter, QWidget *parent)
{
    if (style == QStringLiteral("spinbox")) {
        QSpinBox *widget = new QSpinBox(parent);
        widget->setToolTip(parameter->doc());
        widget->setMinimum(parameter->min());
        widget->setMaximum(parameter->max());
        widget->setValue(parameter->value());

        QObject::connect(widget, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) {
            parameter->setValue(value);
        });

        return widget;

    } else if (style == QStringLiteral("slider")) {
        QSlider *widget = new QSlider(parent);
        widget->setToolTip(parameter->doc());
        widget->setMinimum(parameter->min());
        widget->setMaximum(parameter->max());
        widget->setValue(parameter->value());

        QObject::connect(widget, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),[=](int value) {
            parameter->setValue(value);
        });

        return widget;

    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterDouble::style = QStringLiteral("spinbox");

QWidget *gnomonWidgetsParameterDouble::widget(gnomonCoreParameterDouble *parameter, QWidget *parent)
{
    if (style == QStringLiteral("spinbox")) {
        QDoubleSpinBox *widget = new QDoubleSpinBox(parent);
        widget->setToolTip(parameter->doc());
        widget->setMinimum(parameter->min());
        widget->setMaximum(parameter->max());
        widget->setValue(parameter->value());
        widget->setDecimals(parameter->accuracy());
        widget->setSingleStep(pow(10.,-parameter->accuracy()));

        QObject::connect(widget, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value) {
            parameter->setValue(value);
        });

        return widget;

    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterIntRange::style = QStringLiteral("range_editor");

QWidget *gnomonWidgetsParameterIntRange::widget(gnomonCoreParameterIntRange *parameter, QWidget *parent)
{
    if (style == QStringLiteral("range_editor")) {
        gnomonDoubleRangeEditor *widget = new gnomonDoubleRangeEditor(parent);
        widget->setToolTip(parameter->doc());
        widget->setRange(parameter->min(),parameter->max());
        widget->setValueMin(parameter->value()[0]);
        widget->setValueMax(parameter->value()[1]);

        QObject::connect(widget, &gnomonDoubleRangeEditor::valueMinChanged, [=](int value) {
            parameter->setValue(value, widget->valueMax());
        });

        QObject::connect(widget, &gnomonDoubleRangeEditor::valueMaxChanged,[=](int value) {
            parameter->setValue(widget->valueMin(), value);
        });

        return widget;

    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterDoubleRange::style = QStringLiteral("range_editor");

QWidget *gnomonWidgetsParameterDoubleRange::widget(gnomonCoreParameterDoubleRange *parameter, QWidget *parent)
{
    if (style == QStringLiteral("range_editor")) {
        gnomonDoubleRangeEditor *widget = new gnomonDoubleRangeEditor(parent);
        widget->setToolTip(parameter->doc());
        widget->setRange(parameter->min(),parameter->max());
        widget->setValueMin(parameter->value()[0]);
        widget->setValueMax(parameter->value()[1]);

        QObject::connect(widget, &gnomonDoubleRangeEditor::valueMinChanged, [=](int value) {
            parameter->setValue(value, widget->valueMax());
        });

        QObject::connect(widget, &gnomonDoubleRangeEditor::valueMaxChanged, [=](int value) {
            parameter->setValue(widget->valueMin(), value);
        });

        return widget;

    } else {
        return nullptr;
    }
}


QString gnomonWidgetsParameterBool::style = QStringLiteral("checkbox");

QWidget *gnomonWidgetsParameterBool::widget(gnomonCoreParameterBool *parameter, QWidget *parent)
{
    if (style == QStringLiteral("checkbox")) {
        QCheckBox *widget = new QCheckBox(parent);
        widget->setToolTip(parameter->doc());
        widget->setCheckState(parameter->value() ? Qt::Checked : Qt::Unchecked);

        QObject::connect(widget, &QCheckBox::stateChanged, [=](bool value) {
            parameter->setValue(value);
        });

        return widget;

    } else {
        return nullptr;
    }
}


QString gnomonWidgetsParameterText::style = QStringLiteral("textedit");

QWidget *gnomonWidgetsParameterText::widget(gnomonCoreParameterText *parameter, QWidget *parent)
{
    if (style == QStringLiteral("textedit")) {
        QTextEdit *widget = new QTextEdit(parent);
        widget->setToolTip(parameter->doc());
        widget->setText(parameter->value());

        QObject::connect(widget, &QTextEdit::textChanged, [=](void) {
            parameter->setValue(widget->toPlainText());
        });

        return widget;

    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterString::style = QStringLiteral("combobox");

QWidget *gnomonWidgetsParameterString::widget(gnomonCoreParameterString *parameter, QWidget *parent)
{
    if (style == QStringLiteral("combobox")) {

        QComboBox *widget = new QComboBox(parent);
        widget->setToolTip(parameter->doc());

        for (const auto value : parameter->values()) {
            widget->addItem(value);
        }

        widget->setCurrentText(parameter->value());

        QObject::connect(widget, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int id) {

            parameter->setCurrentIndex(id);
        });

        return widget;

    } else if (style == QStringLiteral("lineedit")) {
        QLineEdit *widget = new QLineEdit(parent);
        widget->setToolTip(parameter->doc());
        widget->setText(parameter->value());

        QObject::connect(widget, &QLineEdit::textChanged, [=](QString value) {
            parameter->setValue(value);
        });

        return widget;

    } else {

        return nullptr;
    }
}


QString gnomonWidgetsParameterStringList::style = QStringLiteral("checkboxes");

QWidget *gnomonWidgetsParameterStringList::widget(gnomonCoreParameterStringList *parameter, QWidget *parent)
{
    if (style == QStringLiteral("checkboxes")) {
        gnomonStringListEditor *widget = new gnomonStringListEditor(parent);
        widget->setToolTip(parameter->doc());
        widget->setValues(parameter->values());
        widget->setValue(parameter->value());

        QObject::connect(widget, &gnomonStringListEditor::valueChanged, [=](const QStringList& val) {
            parameter->setValue(val);
        });

        return widget;
    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterFile::style = QStringLiteral("filedialog");

QWidget *gnomonWidgetsParameterFile::widget(gnomonCoreParameterFile *parameter, QWidget *parent)
{
    if (style == QStringLiteral("filedialog")) {
        QWidget *widget = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        QLabel *file_label = new QLabel(parameter->value());
        layout->addWidget(file_label);

        QPushButton *browse_button = new QPushButton("...");
        browse_button->setFixedWidth(40);
        layout->addWidget(browse_button);

        widget->setLayout(layout);
        widget->setToolTip(parameter->doc());

        QObject::connect(browse_button, &QPushButton::clicked, [=](void) {
            QString path;
            if (!parameter->formats().contains("/")) {
                QString format_string = "File (";
                int i_format = 0;
                for (const auto& format : parameter->formats()) {
                    if (i_format>0) {
                        format_string += " ";
                    }
                    format_string += "*."+format;
                    i_format++;
                }
                format_string += ")";
                qDebug()<<Q_FUNC_INFO<<format_string;
                path = QFileDialog::getOpenFileName(widget, "File path", path, format_string);
                file_label->setText(QFileInfo(path).fileName());
            } else {
                path = QFileDialog::getExistingDirectory(widget, "Select directory", path);
                file_label->setText(QFileInfo(path).fileName());
            }
            parameter->setValue(path);
        });

        return widget;

    } else {
        return nullptr;
    }
}


QString gnomonWidgetsParameterColorMap::style = QStringLiteral("colormap_editor");

QWidget *gnomonWidgetsParameterColorMap::widget(gnomonCoreParameterColorMap *parameter, QWidget *parent)
{
    if (style == QStringLiteral("colormap_editor")) {
        gnomonColorMapEditor *widget = new gnomonColorMapEditor(parent);
        widget->setToolTip(parameter->doc());

        widget->setValue(parameter->name());

        QObject::connect(widget, &gnomonColorMapEditor::valueChanged, [=](const QMap<double, QColor>& val) {
            parameter->setValue(val);
            parameter->setName(widget->name());
        });

        return widget;

    } else {
        return nullptr;
    }
}

QString gnomonWidgetsParameterLookupTable::style = QStringLiteral("lut_editor");

QWidget *gnomonWidgetsParameterLookupTable::widget(gnomonCoreParameterLookupTable *parameter, QWidget *parent)
{
    if (style == QStringLiteral("lut_editor")) {
        gnomonLookupTableEditor *widget = new gnomonLookupTableEditor(parent);
        widget->setToolTip(parameter->doc());

        widget->setValue(parameter->value());

        QObject::connect(widget, &gnomonLookupTableEditor::valueChanged, [=](gnomonLookupTable val) {
            parameter->setValue(val);
        });

        return widget;

    } else {
        return nullptr;
    }
}

//
// gnomonWidgetsParameter.cpp ends here
