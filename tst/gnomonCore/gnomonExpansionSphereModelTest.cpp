#include "gnomonExpansionSphereModelTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
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
}

void gnomonExpansionSphereModelTestCase::init(void)
{
}

void gnomonExpansionSphereModelTestCase::sphereExpansionTest(void)
{
    gnomonSphereForm *sphere_t = new gnomonSphereForm;
    sphere_t->setRadius(1.);

    gnomonAbstractDynamicForm *dynamic_form = new gnomonDiscreteDynamicForm;

    gnomonTime time(1, gnomonTime::Mode::ArbitraryTime);

    dynamic_form->insert(sphere_t, time);

    gnomonExpansionSphereModel model;
    model.setExpansionRate(1.2);
    model.setForm(dynamic_form);
    unsigned int delta_time = 10;
    model.run(time, delta_time);

    QVERIFY(dynamic_form->availableTimes().size() == 2);
    QVERIFY(static_cast<gnomonSphereForm *>(dynamic_form->atTime(time + delta_time))->radius() == 13.);
}

void gnomonExpansionSphereModelTestCase::cleanup(void)
{

}

void gnomonExpansionSphereModelTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonExpansionSphereModelTest, gnomonExpansionSphereModelTestCase);
