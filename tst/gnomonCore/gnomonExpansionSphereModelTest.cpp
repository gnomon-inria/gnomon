#include "gnomonExpansionSphereModelTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkLog>

#include <gnomonExpansionSphereModel.h>
#include <gnomonSphereForm.h>
#include <gnomonAbstractDynamicForm.h>

class gnomonExpansionSphereModelTestCasePrivate
{

};

gnomonExpansionSphereModelTestCase::gnomonExpansionSphereModelTestCase(void) : d(new gnomonExpansionSphereModelTestCasePrivate)
{

}

gnomonExpansionSphereModelTestCase::~gnomonExpansionSphereModelTestCase(void)
{
    delete d;
}

void gnomonExpansionSphereModelTestCase::initTestCase(void)
{
    dtkLogger::instance().attachConsole();
    dtkLogger::instance().setLevel("trace");
    dtkImaging::setVerboseLoading(true);

    dtkImaging::initialize();
}

void gnomonExpansionSphereModelTestCase::init(void)
{
}

void gnomonExpansionSphereModelTestCase::sphereExpansionTest(void)
{
    gnomonSphereForm *sphere_t = new gnomonSphereForm;
    sphere_t->setRadius(1.);

    gnomonAbstractDynamicForm *dynamic_form = new gnomonDiscreteDynamicForm;

    gnomonTime time(0., gnomonTime::Mode::ArbitraryTime);

    dynamic_form->insert(sphere_t, time);

}

void gnomonExpansionSphereModelTestCase::cleanup(void)
{

}

void gnomonExpansionSphereModelTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonExpansionSphereModelTest, gnomonExpansionSphereModelTestCase);
