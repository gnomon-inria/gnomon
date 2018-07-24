#include "gnomonWallContext.h"

#include "gnomonAbstractDynamicForm.h"
#include "gnomonAbstractForm.h"
#include "gnomonSphereForm.h"

class gnomonWallContextPrivate {
public:
    double coord = 0.;
    char axis = 'x';
    double coord_0 = 0.;
    char axis_0 = 'x';
    double coord_1 = 0.;
    char axis_1 = 'x';
    gnomonTime t_1;
};

gnomonWallContext::gnomonWallContext(void) : d(new gnomonWallContextPrivate)
{

}

gnomonWallContext::~gnomonWallContext(void)
{
    delete d;
}

void gnomonWallContext::run(const gnomonTime& time, unsigned long delta_time)
{
    if((time + delta_time) < d->t_1) {
        d->coord = d->coord_0;
        d->axis = d->axis_0;
    } else {
        d->coord = d->coord_1;
        d->axis = d->axis_1;
    }
}

void gnomonWallContext::setTimeT1(const gnomonTime& t_1)
{
    d->t_1 = t_1;
}

void gnomonWallContext::setWallPosition0(double coord, char axis)
{
    d->coord_0 = coord;
    d->axis_0 = axis;
}

void gnomonWallContext::setWallPosition1(double coord, char axis)
{
    d->coord_1 = coord;
    d->axis_1 = axis;
}

char gnomonWallContext::wallAxis()
{
    return d->axis;
}

char gnomonWallContext::wallCoord()
{
    return d->coord;
}

const gnomonTime& gnomonWallContext::timeT1(void)
{
    return d->t_1;
}
