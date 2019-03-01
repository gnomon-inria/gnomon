#include "gnomonImagesSerieReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonForm/gnomonImagesSerie>
#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesSerieReaderCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderCommand *command = nullptr;
};

gnomonImagesSerieReaderCommandTestCase::gnomonImagesSerieReaderCommandTestCase(void) : d(new gnomonImagesSerieReaderCommandTestCasePrivate)
{
}

gnomonImagesSerieReaderCommandTestCase::~gnomonImagesSerieReaderCommandTestCase(void)
{
    delete d;
}

void gnomonImagesSerieReaderCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImagesSerieReaderCommandTestCase::init(void)
{
    d->command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->command);
}

void gnomonImagesSerieReaderCommandTestCase::readInr(void)
{
    QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->command->setPath(image_file_path);

    d->command->redo();

    gnomonImagesSerie *images_serie = d->command->imagesSerie();

    QVERIFY(images_serie->times().size() == 1);
    QVERIFY(images_serie->times().first() == 0);

    dtkImage* image = images_serie->image();

    QVERIFY(image->xDim() == 7);
    QVERIFY(image->yDim() == 5);
    QVERIFY(image->zDim() == 3);

    d->command->undo();

    delete images_serie;
}

void gnomonImagesSerieReaderCommandTestCase::readCzi(void)
{
    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");

    d->command->setPath(image_file_path);

    d->command->redo();

    gnomonImagesSerie *images_serie = d->command->imagesSerie();

    QVERIFY(images_serie->times().size() == 1);
    QVERIFY(images_serie->times().first() == 0);

    QStringList true_list = {"ChS1_EYFP", "Ch1_EBFP", "Ch2_PI", "Ch2_mCherry", "ChS1_EGFP"};

    QCOMPARE(images_serie->channels(), true_list);

    dtkImage* image = nullptr;

    for(auto& channel : images_serie->channels())
    {
      images_serie->setChannel(channel);
      image = images_serie->image();

      QVERIFY(images_serie->channel() == channel);
      QVERIFY(image->xDim() == 101);
      QVERIFY(image->yDim() == 101);
      QVERIFY(image->zDim() == 20);
    }

    d->command->undo();

    delete images_serie;
}

void gnomonImagesSerieReaderCommandTestCase::cleanup(void)
{
    delete d->command;
    d->command = nullptr;
}

void gnomonImagesSerieReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderCommandTest, gnomonImagesSerieReaderCommandTestCase);
