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

#pragma once

#include <gnomonCoreExport.h>

#include <functional>

#include <dtkCore>

class gnomonAbstractSystemScenario;

class GNOMONCORE_EXPORT gnomonSystem
{
public:
    typedef std::function<void(std::size_t, std::size_t)> scenario_t;

public:
    gnomonSystem(void);
    gnomonSystem(const gnomonSystem&);
    gnomonSystem(gnomonSystem&&);
    gnomonSystem(gnomonAbstractSystemScenario *);
    gnomonSystem(const scenario_t&);
    ~gnomonSystem(void);

    gnomonSystem& operator = (const gnomonSystem&);
    gnomonSystem& operator = (gnomonSystem&&);

public:
    void setScenario(gnomonAbstractSystemScenario *);
    void setScenario(const scenario_t&);

    void run(std::size_t time, std::size_t dt);

private:
    class gnomonSystemPrivate *d;
};

DTK_DECLARE_OBJECT(gnomonSystem  )
DTK_DECLARE_OBJECT(gnomonSystem *)

//
// gnomonSystem.h ends here
