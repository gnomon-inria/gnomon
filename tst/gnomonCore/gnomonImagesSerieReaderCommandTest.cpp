#include "gnomonImagesSerieReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesSerieReaderCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderCommand *undo_command_inr = nullptr;
    gnomonImagesSerieReaderCommand *undo_command_czi = nullptr;
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
    d->undo_command_inr = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->undo_command_inr);

    d->undo_command_czi = new gnomonImagesSerieReaderCommand("gnomonCziImageReader");
    Q_ASSERT(d->undo_command_czi);
}

void gnomonImagesSerieReaderCommandTestCase::redoInr(void)
{
   QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->undo_command_inr->setPath(image_file_path);

    d->undo_command_inr->redo();

    dtkImage *image = d->undo_command_inr->image();

    QVERIFY(image->xDim() == 7);
    QVERIFY(image->yDim() == 5);
    QVERIFY(image->zDim() == 3);
}

void gnomonImagesSerieReaderCommandTestCase::undoInr(void)
{
    d->undo_command_inr->undo();
}

void gnomonImagesSerieReaderCommandTestCase::redoCzi(void)
{
   QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
   d->undo_command_czi->setPath(image_file_path);

   d->undo_command_czi->redo();

   QStringList true_list = {"ChS1_EYFP", "Ch1_EBFP", "Ch2_PI", "Ch2_mCherry", "ChS1_EGFP"};

   QCOMPARE(d->undo_command_czi->channels(), true_list);
   QCOMPARE(d->undo_command_czi->image("ChS1_EYFP")->xDim(), 101);
   QCOMPARE(d->undo_command_czi->image("ChS1_EYFP")->yDim(), 101);
   QCOMPARE(d->undo_command_czi->image("ChS1_EYFP")->zDim(), 20);
}

void gnomonImagesSerieReaderCommandTestCase::undoCzi(void)
{
    d->undo_command_czi->undo();
}


void gnomonImagesSerieReaderCommandTestCase::cleanup(void)
{
    delete d->undo_command_inr;
    d->undo_command_inr = nullptr;

    delete d->undo_command_czi;
    d->undo_command_czi = nullptr;

    //dtkScriptInterpreterPython::instance()->release();
}

void gnomonImagesSerieReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderCommandTest, gnomonImagesSerieReaderCommandTestCase);
