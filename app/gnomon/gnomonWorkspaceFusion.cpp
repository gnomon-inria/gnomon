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

#include "gnomonDropSite.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"
#include "gnomonWorkspaceFusion.h"

class gnomonWorkspaceFusionPrivate
{
public:
    gnomonViewVolumic *fusion_view_1;
    gnomonViewVolumic *fusion_view_2;
    gnomonViewVolumic *fusion_view_3;
    gnomonViewVolumic *fusion_view_4;
    gnomonViewVolumic *fusion_view_t;

public:
    QSpinBox *iterations_box;
};

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceFusionPrivate;

    d->fusion_view_1 = new gnomonViewVolumic(this);
    d->fusion_view_2 = new gnomonViewVolumic(this);
    d->fusion_view_3 = new gnomonViewVolumic(this);
    d->fusion_view_4 = new gnomonViewVolumic(this);
    d->fusion_view_t = new gnomonViewVolumic(this);

    QGridLayout *fusion_layout = new QGridLayout;
    fusion_layout->setContentsMargins(0, 0, 0, 0);
    fusion_layout->setSpacing(1);
    fusion_layout->addWidget(d->fusion_view_1, 0, 0);
    fusion_layout->addWidget(d->fusion_view_2, 0, 1);
    fusion_layout->addWidget(d->fusion_view_3, 1, 0);
    fusion_layout->addWidget(d->fusion_view_4, 1, 1);
    fusion_layout->addWidget(d->fusion_view_t, 0, 2, -1, -1);

    d->iterations_box = new QSpinBox(this);
    d->iterations_box->setMinimum(1);
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

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(fusion_layout);
    layout->addWidget(pane);

    connect(apply, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

void gnomonWorkspaceFusion::apply(void)
{
    qDebug() << Q_FUNC_INFO;
}

//
// gnomonWorkspaceFusion.cpp ends here
