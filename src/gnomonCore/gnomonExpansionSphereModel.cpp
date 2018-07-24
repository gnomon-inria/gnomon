#include "gnomonExpansionSphereModel.h"

#include "gnomonAbstractDynamicForm.h"
#include "gnomonAbstractForm.h"
#include "gnomonSphereForm.h"

class gnomonExpansionSphereModelPrivate {
public:
    double expansion_rate = 1.;
};

gnomonExpansionSphereModel::gnomonExpansionSphereModel(void) : d(new gnomonExpansionSphereModelPrivate)
{

}

gnomonExpansionSphereModel::~gnomonExpansionSphereModel(void)
{
    delete d;
}

void gnomonExpansionSphereModel::run(const gnomonTime& time, unsigned long delta_time)
{
    Q_ASSERT(gnomonAbstractModel::dynamic_form);

    gnomonAbstractForm *form_t = dynamic_form->atTime(time);

    Q_ASSERT(form_t);

    gnomonSphereForm *sphere_form_t = static_cast<gnomonSphereForm *>(form_t);

    if(!sphere_form_t) {
        qWarning() << Q_FUNC_INFO << "Failed trying to cast gnomonAbstractForm to gnomonSphereForm at time t";
    }

    gnomonAbstractForm *form_t_dt = gnomonAbstractModel::dynamic_form->atTime(time + delta_time);

    Q_ASSERT(form_t_dt);

    gnomonSphereForm *sphere_form_t_dt = static_cast<gnomonSphereForm *>(form_t_dt);

    if(!sphere_form_t_dt) {
        qWarning() << Q_FUNC_INFO << "Failed trying to cast gnomonAbstractForm to gnomonSphereForm at time t + dt";
    }

    sphere_form_t_dt->setRadius(sphere_form_t->radius() + d->expansion_rate * delta_time);
}

void gnomonExpansionSphereModel::setExpansionRate(double expansion_rate)
{
    d->expansion_rate = expansion_rate;
}

double gnomonExpansionSphereModel::expansionRate(void)
{
    return d->expansion_rate;
}
