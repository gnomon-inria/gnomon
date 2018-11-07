#include "gnomonImagesSerieReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesSerieReaderCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderCommand *undo_command = nullptr;
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
}

void gnomonImagesSerieReaderCommandTestCase::init(void)
{
    d->undo_command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->undo_command);
}

void gnomonImagesSerieReaderCommandTestCase::redo(void)
{
   QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->undo_command->setPath(image_file_path);

    d->undo_command->redo();

    dtkImage *image = d->undo_command->at(0);

    QVERIFY(image->xDim() == 7);
    QVERIFY(image->yDim() == 5);
    QVERIFY(image->zDim() == 3);
}

void gnomonImagesSerieReaderCommandTestCase::undo(void)
{
    d->undo_command->undo();
}

void gnomonImagesSerieReaderCommandTestCase::cleanup(void)
{
    delete d->undo_command;
    d->undo_command = nullptr;
}

void gnomonImagesSerieReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderCommandTest, gnomonImagesSerieReaderCommandTestCase);
