#include "gnomonFileSystemFormReaderTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkLog>

#include <gnomonFileSystemFormReader.h>
#include <gnomonTime.h>
#include <gnomonIntensityImage.h>

class gnomonFileSystemFormReaderTestCasePrivate
{
public:
    gnomonFileSystemFormReader *reader = nullptr;
};

gnomonFileSystemFormReaderTestCase::gnomonFileSystemFormReaderTestCase(void) : d(new gnomonFileSystemFormReaderTestCasePrivate)
{

}

gnomonFileSystemFormReaderTestCase::~gnomonFileSystemFormReaderTestCase(void)
{
    delete d;
}

void gnomonFileSystemFormReaderTestCase::initTestCase(void)
{
    dtkLogger::instance().attachConsole();
    dtkLogger::instance().setLevel("trace");
    dtkImaging::setVerboseLoading(true);

    dtkImaging::initialize();
}

void gnomonFileSystemFormReaderTestCase::init(void)
{
    QVariantHash parameters;
    QString configuration_file_path = QFINDTESTDATA("../resources/test.dyform");
    parameters["configuration_file_path"] = configuration_file_path;

    d->reader = new gnomonFileSystemFormReader(parameters);

    QVERIFY(d->reader->timeMode() == gnomonTime::Mode::ArbitraryTime);

    QVariantHash wrong_parameters;
    gnomonFileSystemFormReader wrong_reader(wrong_parameters);

    QVERIFY(wrong_reader.timeMode() == gnomonTime::Mode::Unknown);
}

void gnomonFileSystemFormReaderTestCase::read(void)
{
    gnomonTime time(0, gnomonTime::ArbitraryTime);

    gnomonIntensityImagePtr form = d->reader->read(time).staticCast<gnomonIntensityImage>();;
    QVERIFY(form);

    QVERIFY(form->data()->dim() == 3);
    QVERIFY(form->data()->xDim() == 7);
    QVERIFY(form->data()->yDim() == 5);
    QVERIFY(form->data()->zDim() == 3);

    time = gnomonTime(1, gnomonTime::ArbitraryTime);
    form = d->reader->read(time).staticCast<gnomonIntensityImage>();;
    QVERIFY(form);

    QVERIFY(form->data()->dim() == 3);
    QVERIFY(form->data()->xDim() == 7);
    QVERIFY(form->data()->yDim() == 5);
    QVERIFY(form->data()->zDim() == 3);

    gnomonTime wrong_time(2, gnomonTime::ArbitraryTime);
    gnomonAbstractFormPtr wrong_form = d->reader->read(wrong_time);
    QVERIFY(!wrong_form);

    wrong_time = gnomonTime(0, gnomonTime::DateTime);
    wrong_form = d->reader->read(wrong_time);
    QVERIFY(!wrong_form);


}

void gnomonFileSystemFormReaderTestCase::cleanup(void)
{

}

void gnomonFileSystemFormReaderTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonFileSystemFormReaderTest, gnomonFileSystemFormReaderTestCase);
