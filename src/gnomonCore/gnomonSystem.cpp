#include "gnomonSystem.h"

#include "gnomonAbstractSystemScenario.h"

#include <QtCore>

class gnomonSystemPrivate
{
public:
    gnomonSystem::scenario_step_t scenario_step = [](std::size_t, std::size_t){ qWarning() << Q_FUNC_INFO << "please set a scenario";};
    gnomonSystem::scenario_reset_t scenario_reset = [](){ qWarning() << Q_FUNC_INFO << "please implement the reset method of the scenario";};
};

gnomonSystem::gnomonSystem(void) : d(new gnomonSystemPrivate)
{
}

gnomonSystem::gnomonSystem(const gnomonSystem& other) : d(new gnomonSystemPrivate)
{
    d->scenario_step = other.d->scenario_step;
    d->scenario_reset = other.d->scenario_reset;
}

gnomonSystem::gnomonSystem(gnomonSystem&& other) : d(other.d)
{
    other.d = nullptr;
}

gnomonSystem::gnomonSystem(gnomonAbstractSystemScenario * system_scenario): d(new gnomonSystemPrivate)
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    d->scenario_step = std::bind(&gnomonAbstractSystemScenario::step, system_scenario, _1, _2);
    d->scenario_reset = std::bind(&gnomonAbstractSystemScenario::reset, system_scenario);

}

gnomonSystem::gnomonSystem(const scenario_step_t& scenario_step, const scenario_reset_t& scenario_reset): d(new gnomonSystemPrivate)
{
    d->scenario_step = scenario_step;
    d->scenario_reset = scenario_reset;
}

gnomonSystem::~gnomonSystem(void)
{
    if(d)
        delete d;
}

gnomonSystem& gnomonSystem::operator = (const gnomonSystem& o)
{
    d->scenario_step = o.d->scenario_step;
    d->scenario_reset = o.d->scenario_reset;

    return *this;
}

gnomonSystem& gnomonSystem::operator = (gnomonSystem&& o)
{
    if(d)
        delete d;
    d = o.d;
    o.d = nullptr;
    return *this;
}

void gnomonSystem::setScenario(gnomonAbstractSystemScenario *system_scenario)
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    d->scenario_step = std::bind(&gnomonAbstractSystemScenario::step, system_scenario, _1, _2);

    d->scenario_reset = std::bind(&gnomonAbstractSystemScenario::reset, system_scenario);
}

void gnomonSystem::setScenarioStep(const scenario_step_t& scenario_step)
{
    d->scenario_step = scenario_step;
}

void gnomonSystem::setScenarioReset(const scenario_reset_t& scenario_reset)
{
    d->scenario_reset = scenario_reset;
}

void gnomonSystem::step(std::size_t time, std::size_t dt)
{
    d->scenario_step(time, dt);
}

void gnomonSystem::reset(void)
{
    d->scenario_reset();
}
