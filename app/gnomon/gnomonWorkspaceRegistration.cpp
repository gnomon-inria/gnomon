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
#include "gnomonWorkspaceRegistration.h"

class gnomonWorkspaceRegistrationPrivate
{
public:
    gnomonViewVolumic *registration_view_1;
    gnomonViewVolumic *registration_view_2;
    gnomonViewVolumic *registration_view_3;
    gnomonViewVolumic *registration_view_4;
    gnomonViewVolumic *registration_view_t;

public:
    QComboBox *box;
};

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspaceRegistrationPrivate;

    d->registration_view_1 = new gnomonViewVolumic(this);
    d->registration_view_2 = new gnomonViewVolumic(this);
    d->registration_view_3 = new gnomonViewVolumic(this);
    d->registration_view_4 = new gnomonViewVolumic(this);
    d->registration_view_t = new gnomonViewVolumic(this);

    QGridLayout *registration_layout = new QGridLayout;
    registration_layout->setContentsMargins(0, 0, 0, 0);
    registration_layout->setSpacing(1);
    registration_layout->addWidget(d->registration_view_1, 0, 0);
    registration_layout->addWidget(d->registration_view_2, 0, 1);
    registration_layout->addWidget(d->registration_view_3, 1, 0);
    registration_layout->addWidget(d->registration_view_4, 1, 1);
    registration_layout->addWidget(d->registration_view_t, 0, 2, -1, -1);

    d->box = new QComboBox(this);
    d->box->addItem("Rigid");

    QFormLayout *pane_item_params_layout = new QFormLayout;
    pane_item_params_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pane_item_params_layout->addRow("Method", d->box);

    gnomonOverlayPaneItem *pane_item_inputs = new gnomonOverlayPaneItem;
    pane_item_inputs->setTitle("Parameters");
    pane_item_inputs->addLayout(pane_item_params_layout);
    pane_item_inputs->toggle();

    QPushButton *apply = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *pane_item_apply = new gnomonOverlayPaneItem;
    pane_item_apply->setTitle("Registration");
    pane_item_apply->addWidget(apply);
    pane_item_apply->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(pane_item_inputs);
    pane->addWidget(pane_item_apply);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(registration_layout);
    layout->addWidget(pane);

    connect(apply, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
}

//
// gnomonWorkspaceRegistration.cpp ends here
