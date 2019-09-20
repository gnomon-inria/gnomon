#include "gnomonDataManagerTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>

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
    dtkImage *the_image = new dtkImage();
    dtkCoreObjectManager::instance()->add(QVariant::fromValue(the_image), "image");
}

void gnomonDataManagerTestCase::init(void)
{

}

void gnomonDataManagerTestCase::getExistingImageTest(void)
{
    QVERIFY(gnomonDataManager::instance()->get("image"));
}

void gnomonDataManagerTestCase::getNonExistingImageTest(void)
{
    if(gnomonDataManager::instance()->get("imageNotHere")) {
        QFAIL("I should get a nullptr so not come here");
    }
}

void gnomonDataManagerTestCase::cleanup(void)
{

}

void gnomonDataManagerTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonDataManagerTest, gnomonDataManagerTestCase);
