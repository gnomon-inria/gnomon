#include "gnomonDataManagerTest.h"

#include <gnomonTest>

class gnomonDataManagerTestCasePrivate
{

};

gnomonDataManagerTestCase::gnomonDataManagerTestCase(void) : d(new gnomonDataManagerTestCasePrivate)
{

}

gnomonDataManagerTestCase::~gnomonDataManagerTestCase(void)
{
    delete d;
}

void gnomonDataManagerTestCase::initTestCase(void)
{

}

void gnomonDataManagerTestCase::init(void)
{

}


void gnomonDataManagerTestCase::cleanup(void)
{

}

void gnomonDataManagerTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonDataManagerTest, gnomonDataManagerTestCase);
