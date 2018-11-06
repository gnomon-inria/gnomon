#include "gnomonImagesSerieReaderUndoCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderUndoCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

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
}

void gnomonImagesSerieReaderUndoCommandTestCase::init(void)
{
    d->undo_command = new gnomonImagesSerieReaderUndoCommand("gnomonImagesSerieReader");
    qDebug() << "d->undo_command" << d->undo_command;
    Q_ASSERT(d->undo_command);
}

void gnomonImagesSerieReaderUndoCommandTestCase::redo(void)
{
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
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderUndoCommandTest, gnomonImagesSerieReaderUndoCommandTestCase);
