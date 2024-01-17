#include "gnomonTimeSeriesTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>

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

void gnomonTimeSeriesTestCase::createFormSerieFromStringTest(void)
{
    gnomonForm::registerForms();
    std::shared_ptr<gnomonCellComplexSeries> f = std::dynamic_pointer_cast<gnomonCellComplexSeries>(gnomonForm::createDynamicForm("gnomonCellComplex"));
    QVERIFY(f);
    QVERIFY(f->formName() == "gnomonCellComplex");
}

void gnomonTimeSeriesTestCase::cleanup(void)
{

}

void gnomonTimeSeriesTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonTimeSeriesTest, gnomonTimeSeriesTestCase);
