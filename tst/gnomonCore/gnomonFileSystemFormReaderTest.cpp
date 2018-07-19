#include "gnomonFileSystemFormReaderTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>

#include <gnomonFileSystemFormReader.h>
#include <gnomonTime.h>

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
    QVERIFY(d->reader->read(time));

    gnomonTime wrong_time(0, gnomonTime::DateTime);
    QVERIFY(!d->reader->read(wrong_time));

}

void gnomonFileSystemFormReaderTestCase::cleanup(void)
{

}

void gnomonFileSystemFormReaderTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonFileSystemFormReaderTest, gnomonFileSystemFormReaderTestCase);
