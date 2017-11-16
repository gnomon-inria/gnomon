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

#include "gnomonActorImage.h"

#include <QtGui>
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// gnomonInspectorSlicePlanesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorSlicePlanesPrivate
{
public:
    QSlider *x_plane_pos_sd;
    QSlider *y_plane_pos_sd;
    QSlider *z_plane_pos_sd;

    QDoubleSpinBox *x_plane_opacity_sb;
    QDoubleSpinBox *y_plane_opacity_sb;
    QDoubleSpinBox *z_plane_opacity_sb;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorSlicePlanes
// /////////////////////////////////////////////////////////////////

gnomonInspectorSlicePlanes::gnomonInspectorSlicePlanes(QWidget *parent) : gnomonInspectorImage(parent), d(new gnomonInspectorSlicePlanesPrivate)
{
    d->x_plane_pos_sd = new QSlider(Qt::Horizontal, this);
    d->x_plane_pos_sd->setMinimum(0);
    d->x_plane_pos_sd->setMaximum(100);

    d->y_plane_pos_sd = new QSlider(Qt::Horizontal, this);
    d->y_plane_pos_sd->setMinimum(0);
    d->y_plane_pos_sd->setMaximum(100);

    d->z_plane_pos_sd = new QSlider(Qt::Horizontal, this);
    d->z_plane_pos_sd->setMinimum(0);
    d->z_plane_pos_sd->setMaximum(100);

    d->x_plane_opacity_sb = new QDoubleSpinBox(this);
    d->x_plane_opacity_sb->setMinimum(0.);
    d->x_plane_opacity_sb->setMaximum(1.);
    d->x_plane_opacity_sb->setSingleStep(0.05);

    d->y_plane_opacity_sb = new QDoubleSpinBox(this);
    d->y_plane_opacity_sb->setMinimum(0.);
    d->y_plane_opacity_sb->setMaximum(1.);
    d->y_plane_opacity_sb->setSingleStep(0.05);

    d->z_plane_opacity_sb = new QDoubleSpinBox(this);
    d->z_plane_opacity_sb->setMinimum(0.);
    d->z_plane_opacity_sb->setMaximum(1.);
    d->z_plane_opacity_sb->setSingleStep(0.05);

    QLabel *x_plane_pos_label = new QLabel(this);
    x_plane_pos_label->setText("X position :");

    QHBoxLayout *x_plane_pos_layout = new QHBoxLayout;
    x_plane_pos_layout->addWidget(x_plane_pos_label);
    x_plane_pos_layout->addWidget(d->x_plane_pos_sd);

    QLabel *x_plane_opacity_label = new QLabel(this);
    x_plane_opacity_label->setText("Opacity :");

    QHBoxLayout *x_plane_opacity_layout = new QHBoxLayout;
    x_plane_opacity_layout->addWidget(x_plane_opacity_label);
    x_plane_opacity_layout->addWidget(d->x_plane_opacity_sb);

    QLabel *y_plane_pos_label = new QLabel(this);
    y_plane_pos_label->setText("Y position :");

    QHBoxLayout *y_plane_pos_layout = new QHBoxLayout;
    y_plane_pos_layout->addWidget(y_plane_pos_label);
    y_plane_pos_layout->addWidget(d->y_plane_pos_sd);

    QLabel *y_plane_opacity_label = new QLabel(this);
    y_plane_opacity_label->setText("Opacity :");

    QHBoxLayout *y_plane_opacity_layout = new QHBoxLayout;
    y_plane_opacity_layout->addWidget(y_plane_opacity_label);
    y_plane_opacity_layout->addWidget(d->y_plane_opacity_sb);

    QLabel *z_plane_pos_label = new QLabel(this);
    z_plane_pos_label->setText("Z position :");

    QHBoxLayout *z_plane_pos_layout = new QHBoxLayout;
    z_plane_pos_layout->addWidget(z_plane_pos_label);
    z_plane_pos_layout->addWidget(d->z_plane_pos_sd);

    QLabel *z_plane_opacity_label = new QLabel(this);
    z_plane_opacity_label->setText("Opacity :");

    QHBoxLayout *z_plane_opacity_layout = new QHBoxLayout;
    z_plane_opacity_layout->addWidget(z_plane_opacity_label);
    z_plane_opacity_layout->addWidget(d->z_plane_opacity_sb);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(x_plane_pos_layout);
    layout->addLayout(x_plane_opacity_layout);
    layout->addLayout(y_plane_pos_layout);
    layout->addLayout(y_plane_opacity_layout);
    layout->addLayout(z_plane_pos_layout);
    layout->addLayout(z_plane_opacity_layout);

    this->setLayout(layout);

    connect(d->x_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit xPlaneOpacityChanged();});
    connect(d->y_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit yPlaneOpacityChanged();});
    connect(d->z_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit zPlaneOpacityChanged();});
    connect(d->x_plane_pos_sd, &QSlider::valueChanged, [=] {emit xPlanePosChanged();});
    connect(d->y_plane_pos_sd, &QSlider::valueChanged, [=] {emit yPlanePosChanged();});
    connect(d->z_plane_pos_sd, &QSlider::valueChanged, [=] {emit zPlanePosChanged();});

    d->x_plane_opacity_sb->setValue(0.5);
    d->y_plane_opacity_sb->setValue(0.5);
    d->z_plane_opacity_sb->setValue(0.5);
}

void gnomonInspectorSlicePlanes::setActor(gnomonActorImage *actor)
{
    connect(this, &gnomonInspectorSlicePlanes::xPlanePosChanged,
            [=] () { actor->setXPlanePos(this->xPlanePos());
            });
    connect(this, &gnomonInspectorSlicePlanes::yPlanePosChanged,
            [=] () { actor->setYPlanePos(this->yPlanePos());
            });
    connect(this, &gnomonInspectorSlicePlanes::zPlanePosChanged,
            [=] () { actor->setZPlanePos(this->zPlanePos());
            });
    connect(this, &gnomonInspectorSlicePlanes::xPlaneOpacityChanged,
            [=] () { actor->setXPlaneOpacity(this->xPlaneOpacity());
            });
    connect(this, &gnomonInspectorSlicePlanes::yPlaneOpacityChanged,
            [=] () { actor->setYPlaneOpacity(this->yPlaneOpacity());
            });
    connect(this, &gnomonInspectorSlicePlanes::zPlaneOpacityChanged,
            [=] () { actor->setZPlaneOpacity(this->zPlaneOpacity());
            });
}

double gnomonInspectorSlicePlanes::xPlaneOpacity(void) const
{
    return d->x_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::yPlaneOpacity(void) const
{
    return d->y_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::zPlaneOpacity(void) const
{
    return d->z_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::xPlanePos(void) const
{
    return d->x_plane_pos_sd->value();
}

double gnomonInspectorSlicePlanes::yPlanePos(void) const
{
    return d->y_plane_pos_sd->value();
}

double gnomonInspectorSlicePlanes::zPlanePos(void) const
{
    return d->z_plane_pos_sd->value();
}

void gnomonInspectorSlicePlanes::setXPlaneOpacity(double opacity)
{
    d->x_plane_opacity_sb->setValue(opacity);
}

void gnomonInspectorSlicePlanes::setYPlaneOpacity(double opacity)
{
    d->y_plane_opacity_sb->setValue(opacity);
}

void gnomonInspectorSlicePlanes::setZPlaneOpacity(double opacity)
{
    d->z_plane_opacity_sb->setValue(opacity);
}

void gnomonInspectorSlicePlanes::setXPlanePos(double position)
{
    //TODO
}

void gnomonInspectorSlicePlanes::setYPlanePos(double position)
{
    //TODO
}

void gnomonInspectorSlicePlanes::setZPlanePos(double position)
{
    //TODO
}

gnomonInspectorSlicePlanes::~gnomonInspectorSlicePlanes(void)
{
    delete d;

    d = NULL;
}

//
// gnomonInspectorSlicePlanes.cpp ends here
