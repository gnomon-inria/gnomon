#include "gnomonImageReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>


class gnomonImageReaderCommandTestCasePrivate
{
public:
    gnomonImageReaderCommand *command = nullptr;
};

gnomonImageReaderCommandTestCase::gnomonImageReaderCommandTestCase(void) : d(new gnomonImageReaderCommandTestCasePrivate)
{
}

gnomonImageReaderCommandTestCase::~gnomonImageReaderCommandTestCase(void)
{
    delete d;
}

void gnomonImageReaderCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImageReaderCommandTestCase::init(void)
{
    d->command = new gnomonImageReaderCommand("gnomonImageReader");
    Q_ASSERT(d->command);
}

void gnomonImageReaderCommandTestCase::readInr(void)
{
    QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->command->setPath(image_file_path);

    d->command->redo();

    gnomonImageSeries *image_series = d->command->image();

    QVERIFY(image_series->times().size() == 1);
    QVERIFY(image_series->times().first() == 0);

    dtkImage* image = dynamic_cast<gnomonImage *>(image_series->current())->image();

    QVERIFY(image->xDim() == 7);
    QVERIFY(image->yDim() == 5);
    QVERIFY(image->zDim() == 3);

    d->command->undo();

    delete image_series;
}

void gnomonImageReaderCommandTestCase::readCzi(void)
{
    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");

    d->command->setPath(image_file_path);

    d->command->redo();

    gnomonImageSeries *image_series = d->command->image();

    QVERIFY(image_series->times().size() == 1);
    QVERIFY(image_series->times().first() == 0);

    QStringList true_list = {"ChS1_EYFP", "Ch1_EBFP", "Ch2_PI", "Ch2_mCherry", "ChS1_EGFP"};

    QStringList channels = dynamic_cast<gnomonImage *>(image_series->current())->channels();

    QCOMPARE(channels, true_list);

    dtkImage* image = nullptr;

    for(auto& channel : channels)
    {
      image =  dynamic_cast<gnomonImage *>(image_series->current())->image(channel);

      QVERIFY(image->xDim() == 101);
      QVERIFY(image->yDim() == 101);
      QVERIFY(image->zDim() == 20);
    }

    d->command->undo();

    delete image_series;
}

void gnomonImageReaderCommandTestCase::cleanup(void)
{
    delete d->command;
    d->command = nullptr;
}

void gnomonImageReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageReaderCommandTest, gnomonImageReaderCommandTestCase);
