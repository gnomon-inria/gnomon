#include "gnomonImagesSerieReaderUndoCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderUndoCommand.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesSerieReaderUndoCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderUndoCommand *undo_command = nullptr;
};

gnomonImagesSerieReaderUndoCommandTestCase::gnomonImagesSerieReaderUndoCommandTestCase(void) : d(new gnomonImagesSerieReaderUndoCommandTestCasePrivate)
{
}

gnomonImagesSerieReaderUndoCommandTestCase::~gnomonImagesSerieReaderUndoCommandTestCase(void)
{
    delete d;
}

void gnomonImagesSerieReaderUndoCommandTestCase::initTestCase(void)
{
    QString command = "import gnomonImagesSerieReader";
    int stat;
    dtkScriptInterpreterPython::instance()->interpret(command, &stat);
}

void gnomonImagesSerieReaderUndoCommandTestCase::init(void)
{
    d->undo_command = new gnomonImagesSerieReaderUndoCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->undo_command);
}

void gnomonImagesSerieReaderUndoCommandTestCase::redo(void)
{
   QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->undo_command->setPath(image_file_path);

    d->undo_command->redo();

    dtkImage *image = d->undo_command->at(0);

    QVERIFY(image->xDim() == 7);
    QVERIFY(image->yDim() == 5);
    QVERIFY(image->zDim() == 3);
}

void gnomonImagesSerieReaderUndoCommandTestCase::undo(void)
{
    d->undo_command->undo();
}

void gnomonImagesSerieReaderUndoCommandTestCase::cleanup(void)
{
    delete d->undo_command;
    d->undo_command = nullptr;
}

void gnomonImagesSerieReaderUndoCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderUndoCommandTest, gnomonImagesSerieReaderUndoCommandTestCase);
