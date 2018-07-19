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

gnomonTime::gnomonTime(const gnomonTime& time)
{
    this->timeStamp = time.timeStamp;
    this->timeMode = time.timeMode;
}

unsigned long gnomonTime::getTimeStamp()
{
	return this->timeStamp;
}

void gnomonTime::setTimeStamp(unsigned long timeStamp)
{
	this->timeStamp = timeStamp;
}

gnomonTime::Mode gnomonTime::getMode()
{
	return this->timeMode;
}

void gnomonTime::setMode(Mode timeMode=ArbitraryTime)
{
    this->timeMode = timeMode;
}


// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonTime, time, gnomonCore);
}

//
// gnomonCellComplexData.cpp ends here

