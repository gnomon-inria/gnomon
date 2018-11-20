#include "gnomonWallForm.h"

class gnomonWallFormPrivate {
public:
    gnomon::vec3_t position = {0., 0., 0.};
    gnomon::vec3_t normal = {1., 0., 0.};
};

gnomonWallForm::gnomonWallForm(void) : d(new gnomonWallFormPrivate)
{
    this->record();
}

gnomonWallForm::~gnomonWallForm(void)
{
    delete d;
}

void gnomonWallForm::setNormal(gnomon::vec3_t normal)
{
    d->normal = normal;
}

const gnomon::vec3_t gnomonWallForm::normal(void) const
{
    return d->normal;
}

QString gnomonWallForm::name(void) const
{
    return "gnomonWallForm";
}

void gnomonWallForm::setPosition(gnomon::vec3_t position)
{
    d->position = position;
}

const gnomon::vec3_t gnomonWallForm::position(void) const
{
    return d->position;
}
