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
        DateTime,
        ArbitraryTime
    };

protected:
	unsigned long timeStamp;
	Mode timeMode;

public:
     gnomonTime(unsigned long timeStamp=0, Mode timeMode=ArbitraryTime);
     gnomonTime(const gnomonTime& o);
    ~gnomonTime() {};

public:
	gnomonTime& operator = (const gnomonTime& time);
public:
    unsigned long getTimeStamp() const;
    void setTimeStamp(unsigned long timeStamp);

    Mode getMode() const;
    void setMode(Mode);
};

bool operator == (const gnomonTime& time1, const gnomonTime& time2);
bool operator != (const gnomonTime& time1, const gnomonTime& time2);
bool operator < (const gnomonTime& time1, const gnomonTime& time2);


// ///////////////////////////////////////////////////////////////////
// Ensure QVariant compatibility
// ///////////////////////////////////////////////////////////////////

Q_DECLARE_METATYPE(gnomonTime)


//
// gnomonTime.h ends here
