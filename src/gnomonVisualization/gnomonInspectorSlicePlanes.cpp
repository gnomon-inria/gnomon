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

#include "gnomonInspectorSlicePlanes.h"

#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

// /////////////////////////////////////////////////////////////////
// gnomonInspectorSlicePlanesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorSlicePlanesPrivate
{
public:
    QDoubleSpinBox *opacity_sb;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorSlicePlanes
// /////////////////////////////////////////////////////////////////

gnomonInspectorSlicePlanes::gnomonInspectorSlicePlanes(void) : gnomonInspectorImage(), d(new gnomonInspectorSlicePlanesPrivate)
{
    d->opacity_sb = new QDoubleSpinBox();
    d->opacity_sb->setMinimum(0.);
    d->opacity_sb->setMaximum(1.);
    d->opacity_sb->setSingleStep(0.05);
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *opacity_layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QLabel *opacity_label = new QLabel();
    opacity_label->setText("Opacity");
    opacity_layout->addWidget(d->opacity_sb);
    opacity_layout->addWidget(opacity_label);
    layout->addLayout(opacity_layout);
    this->setLayout(layout);

    connect(d->opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit opacityChanged(d->opacity_sb->value());});

    d->opacity_sb->setValue(0.5);
}

double gnomonInspectorSlicePlanes::opacity()
{
    return d->opacity_sb->value();
}

void gnomonInspectorSlicePlanes::setOpacity(double opacity)
{
    d->opacity_sb->setValue(opacity);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorSlicePlanes::~gnomonInspectorSlicePlanes(void)
{
    delete d;

    d = NULL;
}

//
// gnomonInspectorSlicePlanes.cpp ends here
