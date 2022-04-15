#pragma once

#include <gnomonCoreExport.h>

#include <functional>

#include <dtkCore>

class gnomonAbstractSystemScenario;

class GNOMONCORE_EXPORT gnomonSystem
{
public:
    typedef std::function<void(std::size_t, std::size_t)> scenario_step_t;
    typedef std::function<void()> scenario_reset_t;

public:
    gnomonSystem(void);
    gnomonSystem(const gnomonSystem&);
    gnomonSystem(gnomonSystem&&);
    gnomonSystem(gnomonAbstractSystemScenario *);
    gnomonSystem(const scenario_step_t&, const scenario_reset_t& = nullptr);
    ~gnomonSystem(void);

    gnomonSystem& operator = (const gnomonSystem&);
    gnomonSystem& operator = (gnomonSystem&&);

public:
    void setScenario(gnomonAbstractSystemScenario *);

    void setScenarioStep(const scenario_step_t&);
    void setScenarioReset(const scenario_reset_t&);

    void step(std::size_t time, std::size_t dt);
    void reset(void);

private:
    class gnomonSystemPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonSystem  )
DTK_DECLARE_OBJECT(gnomonSystem *)

//
// gnomonSystem.h ends here
