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
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;

public:
    QDoubleSpinBox *box_pc_min;
    QDoubleSpinBox *box_pc_max;
};

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->box_pc_min = new QDoubleSpinBox(this);
    d->box_pc_min->setMinimum(1);
    d->box_pc_min->setMaximum(100);
    d->box_pc_min->setValue(2);

    d->box_pc_max = new QDoubleSpinBox(this);
    d->box_pc_max->setMinimum(1);
    d->box_pc_max->setMaximum(100);
    d->box_pc_max->setValue(99);

    QFormLayout *pane_item_params_layout = new QFormLayout;
    pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_params_layout->addRow("pc_min", d->box_pc_min);
    pane_item_params_layout->addRow("pc_max", d->box_pc_max);

    gnomonOverlayPaneItem *pane_item_parameters = new gnomonOverlayPaneItem;
    pane_item_parameters->setTitle("Parameters");
    pane_item_parameters->addLayout(pane_item_params_layout);
    pane_item_parameters->toggle();

    QPushButton *button = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *item = new gnomonOverlayPaneItem(this);
    item->setTitle("Normalization");
    item->addWidget(button);
    item->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_parameters);
    pane->addWidget(item);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(button, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::apply(void)
{
    gnomonImagesSerieFilterCommand *image_filter_command = new gnomonImagesSerieFilterCommand("gnomonImagesSerieFilter");

    Q_ASSERT(image_filter_command);

    image_filter_command->setImage(d->source->image().data());
    image_filter_command->setParameter("pc_min", d->box_pc_min->value());
    image_filter_command->setParameter("pc_max", d->box_pc_max->value());

    image_filter_command->redo();
    dtkImage *img = image_filter_command->next();

    if (!img) {
        qDebug() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    d->target->setImage(dtkImagePtr(new dtkImage(*img)));
}

//
// gnomonWorkspacePreprocess.cpp ends here
