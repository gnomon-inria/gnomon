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
#include <QSlider>

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

gnomonInspectorSlicePlanes::gnomonInspectorSlicePlanes(void) : gnomonInspectorImage(), d(new gnomonInspectorSlicePlanesPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout();

    // ///////////////////////////////////////////////////////////////////
    QHBoxLayout *x_plane_pos_layout = new QHBoxLayout();
    QLabel *x_plane_pos_label = new QLabel();
    x_plane_pos_label->setText("X position :");
    x_plane_pos_layout->addWidget(x_plane_pos_label);
    d->x_plane_pos_sd = new QSlider(Qt::Horizontal);
    d->x_plane_pos_sd->setMinimum(0);
    d->x_plane_pos_sd->setMaximum(100);
    x_plane_pos_layout->addWidget(d->x_plane_pos_sd);
    layout->addLayout(x_plane_pos_layout);

    QHBoxLayout *x_plane_opacity_layout = new QHBoxLayout();
    QLabel *x_plane_opacity_label = new QLabel();
    x_plane_opacity_label->setText("Opacity :");
    x_plane_opacity_layout->addWidget(x_plane_opacity_label);
    d->x_plane_opacity_sb = new QDoubleSpinBox();
    d->x_plane_opacity_sb->setMinimum(0.);
    d->x_plane_opacity_sb->setMaximum(1.);
    d->x_plane_opacity_sb->setSingleStep(0.05);
    x_plane_opacity_layout->addWidget(d->x_plane_opacity_sb);
    layout->addLayout(x_plane_opacity_layout);
    // ///////////////////////////////////////////////////////////////
    QHBoxLayout *y_plane_pos_layout = new QHBoxLayout();
    QLabel *y_plane_pos_label = new QLabel();
    y_plane_pos_label->setText("Y position :");
    y_plane_pos_layout->addWidget(y_plane_pos_label);
    d->y_plane_pos_sd = new QSlider(Qt::Horizontal);
    d->y_plane_pos_sd->setMinimum(0);
    d->y_plane_pos_sd->setMaximum(100);
    y_plane_pos_layout->addWidget(d->y_plane_pos_sd);
    layout->addLayout(y_plane_pos_layout);

    QHBoxLayout *y_plane_opacity_layout = new QHBoxLayout();
    QLabel *y_plane_opacity_label = new QLabel();
    y_plane_opacity_label->setText("Opacity :");
    y_plane_opacity_layout->addWidget(y_plane_opacity_label);
    d->y_plane_opacity_sb = new QDoubleSpinBox();
    d->y_plane_opacity_sb->setMinimum(0.);
    d->y_plane_opacity_sb->setMaximum(1.);
    d->y_plane_opacity_sb->setSingleStep(0.05);
    y_plane_opacity_layout->addWidget(d->y_plane_opacity_sb);
    layout->addLayout(y_plane_opacity_layout);
    // ///////////////////////////////////////////////////////////////
    QHBoxLayout *z_plane_pos_layout = new QHBoxLayout();
    QLabel *z_plane_pos_label = new QLabel();
    z_plane_pos_label->setText("Z position :");
    z_plane_pos_layout->addWidget(z_plane_pos_label);
    d->z_plane_pos_sd = new QSlider(Qt::Horizontal);
    d->z_plane_pos_sd->setMinimum(0);
    d->z_plane_pos_sd->setMaximum(100);
    z_plane_pos_layout->addWidget(d->z_plane_pos_sd);
    layout->addLayout(z_plane_pos_layout);

    QHBoxLayout *z_plane_opacity_layout = new QHBoxLayout();
    QLabel *z_plane_opacity_label = new QLabel();
    z_plane_opacity_label->setText("Opacity :");
    z_plane_opacity_layout->addWidget(z_plane_opacity_label);
    d->z_plane_opacity_sb = new QDoubleSpinBox();
    d->z_plane_opacity_sb->setMinimum(0.);
    d->z_plane_opacity_sb->setMaximum(1.);
    d->z_plane_opacity_sb->setSingleStep(0.05);
    z_plane_opacity_layout->addWidget(d->z_plane_opacity_sb);
    layout->addLayout(z_plane_opacity_layout);

    // ///////////////////////////////////////////////////////////////

    this->setLayout(layout);

    connect(d->x_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit xPlaneOpacityChanged();});
    connect(d->y_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit yPlaneOpacityChanged();});
    connect(d->z_plane_opacity_sb, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=] {emit zPlaneOpacityChanged();});
    connect(d->x_plane_pos_sd, &QSlider::valueChanged, [=] {emit xPlanePosChanged();});
    connect(d->y_plane_pos_sd, &QSlider::valueChanged, [=] {emit yPlanePosChanged();});
    connect(d->z_plane_pos_sd, &QSlider::valueChanged, [=] {emit zPlanePosChanged();});

    d->x_plane_opacity_sb->setValue(0.5);
}

double gnomonInspectorSlicePlanes::xPlaneOpacity()
{
    return d->x_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::yPlaneOpacity()
{
    return d->y_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::zPlaneOpacity()
{
    return d->z_plane_opacity_sb->value();
}

double gnomonInspectorSlicePlanes::xPlanePos()
{
    return d->x_plane_pos_sd->value();
}

double gnomonInspectorSlicePlanes::yPlanePos()
{
    return d->y_plane_pos_sd->value();
}

double gnomonInspectorSlicePlanes::zPlanePos()
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
