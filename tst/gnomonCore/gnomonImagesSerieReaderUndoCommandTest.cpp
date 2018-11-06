#include "gnomonImagesSerieReaderUndoCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderUndoCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesSerieReaderUndoCommandTestCasePrivate
{
public:
    dtkScriptInterpreterPython *interpreter = nullptr;
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
    d->interpreter = new dtkScriptInterpreterPython;

    QString command = "import gnomonImagesSerieReader";
    int stat;
    d->interpreter->interpret(command, &stat);
}

void gnomonImagesSerieReaderUndoCommandTestCase::init(void)
{
    d->undo_command = new gnomonImagesSerieReaderUndoCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->undo_command);
}


void gnomonImagesSerieReaderUndoCommandTestCase::getCommand(void)
{
    gnomonAbstractImagesSerieReader *command = d->undo_command->getCommand();
    QVERIFY(command);
}

void gnomonImagesSerieReaderUndoCommandTestCase::redo(void)
{
    gnomonAbstractImagesSerieReader *command = d->undo_command->getCommand();
    QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    command->setPath(image_file_path);

    d->undo_command->redo();

    dtkImage *image = command->at(0);

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
    delete d->interpreter;
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderUndoCommandTest, gnomonImagesSerieReaderUndoCommandTestCase);
