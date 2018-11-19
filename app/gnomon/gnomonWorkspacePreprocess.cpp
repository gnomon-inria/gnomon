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

#include "gnomonWorkspacePreprocess.h"
#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include <gnomonImagesSerieFilterCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>

#include <QtWidgets>

class gnomonWorkspacePreprocessPrivate
{
public:
    gnomonWorkspacePreprocessPrivate();
    ~gnomonWorkspacePreprocessPrivate();

public:
    gnomonImagesSerieFilterCommand *image_filter_command;

public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;

public:
    QFormLayout *pane_item_params_layout;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate()
{
    this->image_filter_command = nullptr;
}

gnomonWorkspacePreprocessPrivate::~gnomonWorkspacePreprocessPrivate()
{
    if(this->image_filter_command)
        delete this->image_filter_command;
}

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonImagesSerieFilter", &stat);

    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QComboBox *combo_box = new QComboBox(this);
    QStringList combo_box_keys = gnomonCore::imagesSerieFilter::pluginFactory().keys();
    for (QStringList::iterator it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it)
    { combo_box->addItem(*it); }

    connect(combo_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(configure(QString)));

    gnomonOverlayPaneItem *pane_item_algorithm = new gnomonOverlayPaneItem(this);
    pane_item_algorithm->setTitle("Algorithm");
    pane_item_algorithm->addWidget(combo_box);
    pane_item_algorithm->toggle();

    d->pane_item_params_layout = new QFormLayout;

    gnomonOverlayPaneItem *pane_item_parameters = new gnomonOverlayPaneItem;
    pane_item_parameters->setTitle("Parameters");
    pane_item_parameters->addLayout(d->pane_item_params_layout);
    pane_item_parameters->toggle();

    QPushButton *button = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(this);
    pane_item_button->setTitle("Preprocess");
    pane_item_button->addWidget(button);
    pane_item_button->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_algorithm);
    pane->addWidget(pane_item_parameters);
    pane->addWidget(pane_item_button);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(button, SIGNAL(clicked()), this, SLOT(apply()));

    configure(combo_box->currentText());
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::apply(void)
{
    Q_ASSERT(d->image_filter_command);

    d->image_filter_command->setImage(d->source->image().data());

    d->image_filter_command->redo();
    dtkImage *img = d->image_filter_command->next();

    if (!img) {
        qDebug() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    d->target->setImage(dtkImagePtr(new dtkImage(*img)));
}

void gnomonWorkspacePreprocess::configure(const QString& algorithm)
{
    for(int row = 0, max_row = d->pane_item_params_layout->count(); row < max_row; ++row)
    {
        QLayoutItem *forDeletion = d->pane_item_params_layout->takeAt(0);
        forDeletion->widget()->disconnect();
        delete forDeletion->widget();
        delete forDeletion;
    }
    if(d->image_filter_command)
        delete d->image_filter_command;
    d->image_filter_command = new gnomonImagesSerieFilterCommand(algorithm);
    QMap<QString, QVariant> parameters = d->image_filter_command->parameters();
    for(QMap<QString, QVariant>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it)
    {
        QWidget *widget;
        QString key = it.key();
        int type = it.value().type();
        if (type == QMetaType::Int ||
            // type == QMetaType::Uint ||
            type == QMetaType::Long ||
            type == QMetaType::ULong ||
            type == QMetaType::LongLong ||
            type == QMetaType::ULongLong) {
            widget = new QSpinBox(this);
            static_cast< QSpinBox* >(widget)->setValue(it.value().value<int>());
            connect(static_cast< QSpinBox* >(widget), static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                    [=](int value){ d->image_filter_command->setParameter(key, value); });
        } else if (type == QMetaType::Float ||
                   type == QMetaType::Double) {
            widget = new QDoubleSpinBox(this);
            static_cast< QDoubleSpinBox* >(widget)->setValue(it.value().value<double>());
            connect(static_cast< QDoubleSpinBox* >(widget), static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [=](double value){ d->image_filter_command->setParameter(key, value); });
        } else if (type == QMetaType::QString) {
            widget = new QLineEdit(this);
            static_cast< QLineEdit* >(widget)->setText(it.value().value<QString>());
            connect(static_cast< QLineEdit* >(widget), &QLineEdit::textChanged,
                    [=](QString value){ d->image_filter_command->setParameter(key, value); });
        } else if (type == QMetaType::Bool) {
            widget = new QCheckBox(this);
            if(it.value().value<bool>())
                static_cast< QCheckBox* >(widget)->setCheckState(Qt::Checked);
            else
                static_cast< QCheckBox* >(widget)->setCheckState(Qt::Unchecked);
            connect(static_cast< QCheckBox* >(widget), &QCheckBox::stateChanged,
                    [=](int value){ d->image_filter_command->setParameter(key, value > Qt::Unchecked); });
        }
        d->pane_item_params_layout->addRow(it.key(), widget);
    }
    d->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
}

//
// gnomonWorkspacePreprocess.cpp ends here
