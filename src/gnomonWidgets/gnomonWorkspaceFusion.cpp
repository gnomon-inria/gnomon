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

#include "gnomonWorkspaceFusion.h"

#include "gnomonGridLayout.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"

#include <gnomonImagesFusionCommand>

class gnomonWorkspaceFusionPrivate
{
public:
    gnomonGridLayout *layout;

public:
    gnomonViewVolumic *target;

public:
    gnomonImagesFusionCommand *images_fusion_command = nullptr;

public:
    QSpinBox *iterations_box;
};

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspaceFusionPrivate;

    d->layout = new gnomonGridLayout;

    d->target = new gnomonViewVolumic(this);
    d->target->setMinimumWidth(250);

    d->iterations_box = new QSpinBox(this);
    d->iterations_box->setMinimum(0);
    d->iterations_box->setMaximum(10);
    d->iterations_box->setValue(5);

    QFormLayout *pane_item_params_layout = new QFormLayout;
    pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_params_layout->addRow("Iterations", d->iterations_box);

    gnomonOverlayPaneItem *pane_item_inputs = new gnomonOverlayPaneItem;
    pane_item_inputs->setTitle("Parameters");
    pane_item_inputs->addLayout(pane_item_params_layout);
    pane_item_inputs->toggle();

    QPushButton *apply = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *pane_item_apply = new gnomonOverlayPaneItem;
    pane_item_apply->setTitle("Fusion");
    pane_item_apply->addWidget(apply);
    pane_item_apply->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_inputs);
    pane->addWidget(pane_item_apply);
    pane->toggle();

    QWidget *dummy = new QWidget(this);
    dummy->setLayout(d->layout);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(dummy);
    splitter->addWidget(d->target);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(pane);

    connect(apply, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

void gnomonWorkspaceFusion::apply(void)
{
    d->images_fusion_command = new gnomonImagesFusionCommand("gnomonImagesFusion");
    d->images_fusion_command->setParameter("nb_iterations", d->iterations_box->value());

    if(d->layout->views().isEmpty()) return;

    for(gnomonViewVolumic *view : d->layout->views()) {
        d->images_fusion_command->addImage(view->image().data());
    }
    d->images_fusion_command->redo();
    d->target->setImage(dtkImagePtr(d->images_fusion_command->output()));
}

//
// gnomonWorkspaceFusion.cpp ends here
