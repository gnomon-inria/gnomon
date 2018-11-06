#include "gnomonImagesSerieReaderUndoCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderUndoCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

#include <dtkScript>

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
    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
    command->setPath(image_file_path);

    d->undo_command->redo();
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
