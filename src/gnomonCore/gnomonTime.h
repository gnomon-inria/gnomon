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

#include <QtCore>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonTime
{
public:
    enum Mode {
        Unknown,
        DateTime,
        ArbitraryTime
    };

public:
     gnomonTime(unsigned long timeStamp=0, Mode timeMode=ArbitraryTime);
     gnomonTime(const gnomonTime& time);
    ~gnomonTime() {};

protected:
	unsigned long timeStamp;
	Mode timeMode;

public:
    unsigned long getTimeStamp() const;
    void setTimeStamp(unsigned long timeStamp);

    Mode getMode() const;
    void setMode(Mode);
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonTime *)
DTK_DECLARE_PLUGIN        (gnomonTime, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonTime, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonTime, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonTime, GNOMONCORE_EXPORT, time);
}

//
// gnomonTime.h ends here
