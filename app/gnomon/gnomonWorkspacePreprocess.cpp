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
// public:
//     QDoubleSpinBox *box_pc_min;
//     QDoubleSpinBox *box_pc_max;

// public:
//     QFormLayout *pane_item_params_layout;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate()
{
}

gnomonWorkspacePreprocessPrivate::~gnomonWorkspacePreprocessPrivate()
{
    if(image_filter_command)
        delete image_filter_command;  
}

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->image_filter_command = new gnomonImagesSerieFilterCommand("gnomonImagesSerieFilter");   

    // d->box_pc_min = new QDoubleSpinBox(this);
    // d->box_pc_min->setMinimum(1);
    // d->box_pc_min->setMaximum(100);
    // d->box_pc_min->setValue(2);

    // d->box_pc_max = new QDoubleSpinBox(this);
    // d->box_pc_max->setMinimum(1);
    // d->box_pc_max->setMaximum(100);
    // d->box_pc_max->setValue(99);

    QComboBox *combo_box = new QComboBox(this);
    QStringList combo_box_keys = gnomonCore::imagesSerieFilter::pluginFactory().keys();
    for (QStringList::iterator it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it)
    { combo_box->addItem(*it); }
    combo_box->addItem("a");

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
    // d->image_filter_command->setParameter("pc_min", d->box_pc_min->value());
    // d->image_filter_command->setParameter("pc_max", d->box_pc_max->value());

    d->image_filter_command->redo();
    dtkImage *img = d->image_filter_command->next();

    if (!img) {
        qDebug() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    d->target->setImage(dtkImagePtr(new dtkImage(*img)));
}

void gnomonWorkspacePreprocess::configure(const QString& text)
{
    // qDebug() << "delete";
    // if(d->image_filter_command)
    //     delete d->image_filter_command;
    qDebug() << "configure";
    for(int row = 0, max_row = d->pane_item_params_layout->count(); row < max_row; ++row)
    {
        QLayoutItem *forDeletion = d->pane_item_params_layout->takeAt(0);
        delete forDeletion->widget();
        delete forDeletion;
    }
    if(text == "gnomonImagesSerieFilter")
    {
        d->image_filter_command = new gnomonImagesSerieFilterCommand("gnomonImagesSerieFilter"); 
        QMap<QString, QVariant> parameters = d->image_filter_command->parameters();
        foreach(QString key, parameters.keys())
        { 
            QWidget *widget;
            int type = parameters[key].type();
            if (type == QMetaType::Int ||
                // type == QMetaType::Uint ||
                type == QMetaType::Long ||
                type == QMetaType::ULong ||
                type == QMetaType::LongLong ||
                type == QMetaType::ULongLong) {
            } else if (type == QMetaType::Float ||
                       type == QMetaType::Double) {
                widget = new QDoubleSpinBox(this);
                static_cast< QDoubleSpinBox* >(widget)->setValue(parameters[key].value<double>());
            } else if (type == QMetaType::QString) {
            } else if (type == QMetaType::Bool) {
            }
            d->pane_item_params_layout->addRow(key, widget);
        }  
        d->pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

//
// gnomonWorkspacePreprocess.cpp ends here
