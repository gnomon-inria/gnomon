#include "gnomonWorkspaceTemplate.h"

#include <gnomonParameter>
#include <gnomonImagesSerieFilterCommand>

template<>
void gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >::configure(QWidget* parent, const QString& algorithm)
{
    if(this->pane_item_params_layout)
    {
        for(int row = 0, max_row = this->pane_item_params_layout->count(); row < max_row; ++row)
        {
            QLayoutItem *forDeletion = this->pane_item_params_layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }
    }
    else
    {
        this->pane_item_params_layout = new QFormLayout(parent); 
    }
    if(this->command)
        delete this->command;
    this->command = new gnomonImagesSerieFilterCommand(algorithm);
    for(size_t parameter = 0, max_parameter = this->command->getNbParameters(); parameter < max_parameter; ++parameter)
    {
        gnomonParameter *gnomon_parameter = this->command->getParameter(parameter);
        QString key = "A";
        QWidget* widget = gnomon_parameter->connect(parent); 
        this->pane_item_params_layout->addRow(key, widget);
    }
    this->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    // QMap<QString, QVariant> parameters = this->command->parameters();
    // for(QMap<QString, QVariant>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it)
    // { 
    //     QWidget *widget;
    //     QString key = it.key();
    //     int type = it.value().type();
    //     if (type == QMetaType::Int ||
    //         // type == QMetaType::Uint ||
    //         type == QMetaType::Long ||
    //         type == QMetaType::ULong ||
    //         type == QMetaType::LongLong ||
    //         type == QMetaType::ULongLong) {
    //         widget = new QSpinBox(parent);
    //         static_cast< QSpinBox* >(widget)->setValue(it.value().value<int>());
    //         parent->connect(static_cast< QSpinBox* >(widget), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
    //                 [=](int value){ this->command->setParameter(key, value); });
    //     } else if (type == QMetaType::Float ||
    //                type == QMetaType::Double) {
    //         widget = new QDoubleSpinBox(parent);
    //         static_cast< QDoubleSpinBox* >(widget)->setValue(it.value().value<double>());
    //         parent->connect(static_cast< QDoubleSpinBox* >(widget),  static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
    //                 [=](double value){ this->command->setParameter(key, value); });
    //     } else if (type == QMetaType::QString) {
    //         widget = new QLineEdit(parent);
    //         static_cast< QLineEdit* >(widget)->setText(it.value().value<QString>());
    //         parent->connect(static_cast< QLineEdit* >(widget), &QLineEdit::textChanged,
    //                 [=](QString value){ this->command->setParameter(key, value); });
    //     } else if (type == QMetaType::Bool) {
    //         widget = new QCheckBox(parent);
    //         if(it.value().value<bool>())
    //             static_cast< QCheckBox* >(widget)->setCheckState(Qt::Checked);
    //         else
    //             static_cast< QCheckBox* >(widget)->setCheckState(Qt::Unchecked);
    //         parent->connect(static_cast< QCheckBox* >(widget), &QCheckBox::stateChanged,
    //                 [=](int value){ this->command->setParameter(key, value > Qt::Unchecked); });
    //     }
    //     this->pane_item_params_layout->addRow(it.key(), widget);
    // }  
    // this->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
}