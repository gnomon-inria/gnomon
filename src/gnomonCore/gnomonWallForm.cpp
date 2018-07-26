#include "gnomonWallForm.h"

class gnomonWallFormPrivate {
public:
    double position[3] = {0., 0., 0.};
    double normal[3] = {1., 0., 0.};
};

gnomonWallForm::gnomonWallForm(void) : d(new gnomonWallFormPrivate)
{

}

gnomonWallForm::~gnomonWallForm(void)
{
    delete d;
}

void gnomonWallForm::setNormal(double *normal)
{
    d->normal[0] = normal[0];
    d->normal[1] = normal[1];
    d->normal[1] = normal[2];
}

const double * gnomonWallForm::normal(void) const
{
    return d->normal;
}

void gnomonWallForm::setPosition(double *position)
{
    d->position[0] = position[0];
    d->position[1] = position[1];
    d->position[1] = position[2];
}

const double * gnomonWallForm::position(void) const
{
    return d->position;
}
