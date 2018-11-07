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

    gnomonOverlayPaneItem *pane_item_inputs = new gnomonOverlayPaneItem;
    pane_item_inputs->setTitle("Inputs");

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
