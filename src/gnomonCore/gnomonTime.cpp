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


#include "gnomonCore.h"

#include <dtkCore>
#include "gnomonTime.h"


gnomonTime::gnomonTime(unsigned long timeStamp, Mode timeMode)
{
    this->timeStamp = timeStamp;
    this->timeMode = timeMode;
}

gnomonTime::gnomonTime(const gnomonTime& o)
{
    this->timeStamp = o.timeStamp;
    this->timeMode = o.timeMode;
}

gnomonTime& gnomonTime::operator = (const gnomonTime& time)
{
    this->timeStamp = time.timeStamp;
    this->timeMode = time.timeMode;
    return *this;
}

unsigned long gnomonTime::getTimeStamp() const
{
	return this->timeStamp;
}

void gnomonTime::setTimeStamp(unsigned long timeStamp)
{
	this->timeStamp = timeStamp;
}

gnomonTime::Mode gnomonTime::getMode() const
{
	return this->timeMode;
}

void gnomonTime::setMode(Mode timeMode=ArbitraryTime)
{
    this->timeMode = timeMode;
}


bool operator == (const gnomonTime& time1, const gnomonTime& time2)
{
	return time1.getTimeStamp()==time2.getTimeStamp();
}

bool operator != (const gnomonTime& time1, const gnomonTime& time2)
{
	return time1.getTimeStamp()!=time2.getTimeStamp();
}

bool operator < (const gnomonTime& time1, const gnomonTime& time2)
{
    return time1.getTimeStamp()<time2.getTimeStamp();
}

gnomonTime operator + (const gnomonTime& time, unsigned long delta_time)
{
    return gnomonTime(time.getTimeStamp() + delta_time, time.getMode());
}

//
// gnomonTime.cpp ends here
