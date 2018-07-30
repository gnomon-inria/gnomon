#include "gnomonSystem.h"

#include "gnomonAbstractSystemScenario.h"

#include <QtCore>

class gnomonSystemPrivate
{
public:
    gnomonSystem::scenario_t scenario = [](std::size_t, std::size_t){ qWarning() << Q_FUNC_INFO << "please set a scenario";};
};

gnomonSystem::gnomonSystem(void) : d(new gnomonSystemPrivate)
{
}

gnomonSystem::gnomonSystem(const gnomonSystem& other) : d(new gnomonSystemPrivate)
{
    d->scenario = other.d->scenario;
}

gnomonSystem::gnomonSystem(gnomonSystem&& other) : d(other.d)
{
    other.d = nullptr;
}

gnomonSystem::gnomonSystem(gnomonAbstractSystemScenario * system_scenario): d(new gnomonSystemPrivate)
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    d->scenario = std::bind(&gnomonAbstractSystemScenario::step, system_scenario, _1, _2);
}

gnomonSystem::gnomonSystem(const scenario_t& scenario): d(new gnomonSystemPrivate)
{
    d->scenario = scenario;
}

gnomonSystem::~gnomonSystem(void)
{
    if(d)
        delete d;
}

gnomonSystem& gnomonSystem::operator = (const gnomonSystem& o)
{
    d->scenario = o.d->scenario;
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
    d->scenario = std::bind(&gnomonAbstractSystemScenario::step, system_scenario, _1, _2);
}

void gnomonSystem::setScenario(const scenario_t& scenario)
{
    d->scenario = scenario;
}

void gnomonSystem::step(std::size_t time, std::size_t dt)
{
    d->scenario(time, dt);
}
