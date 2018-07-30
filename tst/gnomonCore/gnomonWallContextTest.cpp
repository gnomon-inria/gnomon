#include "gnomonWallContextTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkLog>

#include <gnomonAbstractDynamicForm.h>
#include <gnomonExpansionSphereModel.h>
#include <gnomonSphereForm.h>
#include <gnomonWallContext.h>

gnomonWallContextTestCase::gnomonWallContextTestCase(void)
{

}

gnomonWallContextTestCase::~gnomonWallContextTestCase(void)
{

}

void gnomonWallContextTestCase::initTestCase(void)
{
    dtkLogger::instance().attachConsole();
    dtkLogger::instance().setLevel("trace");
}

void gnomonWallContextTestCase::init(void)
{
}

void gnomonWallContextTestCase::sphereExpansionTest(void)
{
    gnomonWall context;
    gnomonTime time_t1(4, gnomonTime::Mode::ArbitraryTime);
    context.setTimeT1(time_t1);
    context.setWallPosition0(3., 'x');
    context.setWallPosition1(4., 'y');

    unsigned int delta_time = 1;
    gnomonTime time(1, gnomonTime::Mode::ArbitraryTime);
    context.step(time, delta_time);

    QVERIFY(context.wallCoord() == 3);
    QVERIFY(context.wallAxis() == 'x');

    delta_time = 6;
    context.step(time, delta_time);

    QVERIFY(context.wallCoord() == 4);
    QVERIFY(context.wallAxis() == 'y');
}

void gnomonWallContextTestCase::cleanup(void)
{

}

void gnomonWallContextTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonWallContextTest, gnomonWallContextTestCase);
