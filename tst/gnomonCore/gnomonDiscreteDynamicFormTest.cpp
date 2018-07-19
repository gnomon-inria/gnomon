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

#include "gnomonDiscreteDynamicFormTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>

class gnomonDiscreteDynamicFormTestCasePrivate
{

};

gnomonDiscreteDynamicFormTestCase::gnomonDiscreteDynamicFormTestCase(void) : d(new gnomonDiscreteDynamicFormTestCasePrivate)
{

}

gnomonDiscreteDynamicFormTestCase::~gnomonDiscreteDynamicFormTestCase(void)
{
    delete d;
}

void gnomonDiscreteDynamicFormTestCase::initTestCase(void)
{

}

void gnomonDiscreteDynamicFormTestCase::init(void)
{

}

void gnomonDiscreteDynamicFormTestCase::createEmptyFormTest(void)
{
	gnomonDiscreteDynamicForm* f;
    QVERIFY(f = new gnomonDiscreteDynamicForm());
}


void gnomonDiscreteDynamicFormTestCase::cleanup(void)
{

}

void gnomonDiscreteDynamicFormTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonDiscreteDynamicFormTest, gnomonDiscreteDynamicFormTestCase);
