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

#include "gnomonTimeSeriesTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>

class gnomonTimeSeriesTestCasePrivate
{

};

gnomonTimeSeriesTestCase::gnomonTimeSeriesTestCase(void) : d(new gnomonTimeSeriesTestCasePrivate)
{

}

gnomonTimeSeriesTestCase::~gnomonTimeSeriesTestCase(void)
{
    delete d;
}

void gnomonTimeSeriesTestCase::initTestCase(void)
{

}

void gnomonTimeSeriesTestCase::init(void)
{

}

void gnomonTimeSeriesTestCase::createEmptyFormTest(void)
{
	gnomonTimeSeries<gnomonCellComplex> *f;
    QVERIFY(f = new gnomonTimeSeries<gnomonCellComplex>());
}


void gnomonTimeSeriesTestCase::cleanup(void)
{

}

void gnomonTimeSeriesTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonTimeSeriesTest, gnomonTimeSeriesTestCase);
