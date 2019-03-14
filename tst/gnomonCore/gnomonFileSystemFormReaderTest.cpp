#include "gnomonFileSystemFormReaderTest.h"

#include <gnomonCore>
#include <gnomonCore/gnomonForm/gnomonIntensityImage/gnomonIntensityImage>
#include <gnomonTest>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkLog>

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
    dtkLogger::instance().attachConsole();
    dtkImaging::setVerboseLoading(true);

    dtkImaging::setAutoLoading(false);
    dtkImaging::initialize();
    dtkImaging::manager().pluginManagers()["dtkImageReader"]->loadFromName("dtkVtkImageReader");
}

void gnomonFileSystemFormReaderTestCase::init(void)
{
    // QVariantHash parameters;
    QString configuration_file_path = QFINDTESTDATA("../resources/test.dyform");
    // parameters["configuration_file_path"] = configuration_file_path;

    // d->reader = new gnomonFileSystemFormReader(parameters);
    d->reader = new gnomonFileSystemFormReader(configuration_file_path);

    QVERIFY(d->reader->timeMode() == gnomonTime::Mode::ArbitraryTime);

    // QVariantHash wrong_parameters;
    QString wrong_configuration_file_path = QFINDTESTDATA("../resources/wrong_test.dyform");

    // gnomonFileSystemFormReader wrong_reader(wrong_parameters);
    gnomonFileSystemFormReader wrong_reader(wrong_configuration_file_path);

    QVERIFY(wrong_reader.timeMode() == gnomonTime::Mode::Unknown);

}

void gnomonFileSystemFormReaderTestCase::read(void)
{
    gnomonTime time(0, gnomonTime::ArbitraryTime);

    // gnomonIntensityImage* form = d->reader->read(time).staticCast<gnomonIntensityImage>();
    gnomonIntensityImage* form = (gnomonIntensityImage *) d->reader->read(time);
    QVERIFY(form);

    QVERIFY(form->data()->dim() == 3);
    QVERIFY(form->data()->xDim() == 7);
    QVERIFY(form->data()->yDim() == 5);
    QVERIFY(form->data()->zDim() == 3);

    time = gnomonTime(1, gnomonTime::ArbitraryTime);
    delete form;
    form = (gnomonIntensityImage *) d->reader->read(time);
    QVERIFY(form);

    QVERIFY(form->data()->dim() == 3);
    QVERIFY(form->data()->xDim() == 7);
    QVERIFY(form->data()->yDim() == 5);
    QVERIFY(form->data()->zDim() == 3);

    delete form;
    gnomonTime wrong_time(2, gnomonTime::ArbitraryTime);
    gnomonAbstractForm* wrong_form = d->reader->read(wrong_time);
    QVERIFY(!wrong_form);

    wrong_time = gnomonTime(0, gnomonTime::DateTime);
    wrong_form = d->reader->read(wrong_time);
    QVERIFY(!wrong_form);


}

void gnomonFileSystemFormReaderTestCase::cleanup(void)
{
    delete d->reader;
}

void gnomonFileSystemFormReaderTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonFileSystemFormReaderTest, gnomonFileSystemFormReaderTestCase);
