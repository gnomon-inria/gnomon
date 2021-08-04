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

#include <dtkWidgets>

#include "gnomonColor/gnomonCoreParameterColor.h"

#include "gnomonEditor/gnomonColorMapEditor.h"
//#include "gnomonEditor/gnomonDoubleRangeEditor.h"
//#include "gnomonEditor/gnomonStringListEditor.h"
#include "gnomonEditor/gnomonLookupTableEditor.h"

QWidget *gnomonWidgetsParameter::widget(dtkCoreParameter *parameter, QWidget *parent)
{
    if (dtk::d_bool *p = dynamic_cast<dtk::d_bool *>(parameter)) {
        auto *w = new dtkWidgetsParameterBoolCheckBox(parent);
        w->connect(p);
        return w;
    }

    if (dtk::d_int *p = dynamic_cast<dtk::d_int *>(parameter)) {
        auto *w = new dtkWidgetsParameterIntSpinBox(parent);
        w->connect(p);
        return w;
    }
    if (dtk::d_real *p = dynamic_cast<dtk::d_real *>(parameter)) {
        auto *w = new dtkWidgetsParameterDoubleSpinBox(parent);
        w->connect(p);
        return w;
    }
    if (dtk::d_range_int *p = dynamic_cast<dtk::d_range_int *>(parameter)) {
        auto *w = new dtkWidgetsParameterIntRange(parent);
        w->connect(p);
        return w;
    }
    if (dtk::d_range_real *p = dynamic_cast<dtk::d_range_real *>(parameter)) {
        auto *w = new dtkWidgetsParameterDoubleRange(parent);
        w->connect(p);
        return w;
    }
    if (dtk::d_string *p = dynamic_cast<dtk::d_string *>(parameter)) {
        auto *w = new dtkWidgetsParameterStringLineEdit(parent);
        w->connect(p);
        return w;
    }
    if (dtk::d_inliststring *p = dynamic_cast<dtk::d_inliststring *>(parameter)) {
        // old gnomonCoreString -> combobox?
        // old ...list -> checkboxes ?
        auto *w = new dtkWidgetsParameterStringListComboBox(parent); // checkboxes ?
        w->connect(p);
        return w;
    }
    if (dtk::d_inliststringlist *p = dynamic_cast<dtk::d_inliststringlist *>(parameter)) {
        // old gnomonCoreString -> combobox?
        // old ...list -> checkboxes ?
        auto *w = new dtkWidgetsParameterListStringListCheckBox(parent); // checkboxes ?
        w->connect(p);
        return w;
    }

    if (dtk::d_path *p = dynamic_cast<dtk::d_path *>(parameter)) {
        auto *w = new dtkWidgetsParameterFileBrowse(parent);
        w->connect(p);
        return w;
    }
    if (gnomonCoreParameterColorMap *p = dynamic_cast<gnomonCoreParameterColorMap *>(parameter)) {

        gnomonColorMapEditor *widget = new gnomonColorMapEditor(parent);
        widget->setToolTip(p->documentation());
        widget->setValue(p->name());

        QObject::connect(widget, &gnomonColorMapEditor::valueChanged, [=](const QMap<double, QColor>& val) {
            p->setValue(val);
            p->setName(widget->name());
        });

        return widget;
    }
    //if (gnomonCoreParameterLookupTable *p = dynamic_cast<gnomonCoreParameterLookupTable *>(parameter)) {
    //    return gnomonWidgetsParameterLookupTable::widget(p, parent);
    //}
    return nullptr;
}

/*

QString gnomonWidgetsParameterText::style = QStringLiteral("textedit");

QWidget *gnomonWidgetsParameterText::widget(dtkCoreParameterText *parameter, QWidget *parent)
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

QWidget *gnomonWidgetsParameterString::widget(dtk::d_inliststring *parameter, QWidget *parent)
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

QWidget *gnomonWidgetsParameterStringList::widget(dtk::d_inliststring *parameter, QWidget *parent)
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

QWidget *gnomonWidgetsParameterFile::widget(dtk::d_path *parameter, QWidget *parent)
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
*/