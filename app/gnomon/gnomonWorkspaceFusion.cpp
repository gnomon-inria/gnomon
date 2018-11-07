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
    gnomonDropSite *site_1;
    gnomonDropSite *site_2;
    gnomonDropSite *site_3;
    gnomonDropSite *site_4;
};

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceFusionPrivate;

    d->fusion_view_1 = new gnomonViewVolumic(this);
    d->fusion_view_2 = new gnomonViewVolumic(this);
    d->fusion_view_3 = new gnomonViewVolumic(this);
    d->fusion_view_4 = new gnomonViewVolumic(this);
    d->fusion_view_t = new gnomonViewVolumic(this);

    d->site_1 = new gnomonDropSite(this);
    d->site_2 = new gnomonDropSite(this);
    d->site_3 = new gnomonDropSite(this);
    d->site_4 = new gnomonDropSite(this);

    QGridLayout *fusion_layout = new QGridLayout;
    fusion_layout->setContentsMargins(0, 0, 0, 0);
    fusion_layout->setSpacing(1);
    fusion_layout->addWidget(d->fusion_view_1, 0, 0);
    fusion_layout->addWidget(d->fusion_view_2, 0, 1);
    fusion_layout->addWidget(d->fusion_view_3, 1, 0);
    fusion_layout->addWidget(d->fusion_view_4, 1, 1);
    fusion_layout->addWidget(d->fusion_view_t, 0, 2, -1, -1);

    QFormLayout *pane_item_inputs_layout = new QFormLayout;
    pane_item_inputs_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_inputs_layout->addRow("Reference", d->site_1);
    pane_item_inputs_layout->addRow("Moving", d->site_2);
    pane_item_inputs_layout->addRow("Moving", d->site_3);
    pane_item_inputs_layout->addRow("Moving", d->site_4);

    gnomonOverlayPaneItem *pane_item_inputs = new gnomonOverlayPaneItem;
    pane_item_inputs->setTitle("Inputs");
    pane_item_inputs->addLayout(pane_item_inputs_layout);
    pane_item_inputs->toggle();

    QPushButton *apply = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *pane_item_apply = new gnomonOverlayPaneItem;
    pane_item_apply->setTitle("Inputs");
    pane_item_apply->addLayout(pane_item_inputs_layout);
    pane_item_apply->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_inputs);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(fusion_layout);
    layout->addWidget(pane);
}

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

//
// gnomonWorkspaceFusion.cpp ends here
