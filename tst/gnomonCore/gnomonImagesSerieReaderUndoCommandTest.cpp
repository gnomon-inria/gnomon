#include "gnomonImagesSerieReaderUndoCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieReaderUndoCommand.h"

class gnomonImagesSerieReaderUndoCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderUndoCommand *command = nullptr;
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
    d->command = new gnomonImagesSerieReaderUndoCommand();
}

void gnomonImagesSerieReaderUndoCommandTestCase::redo(void)
{
    d->command->redo();
}

void gnomonImagesSerieReaderUndoCommandTestCase::undo(void)
{
    d->command->undo();
}

void gnomonImagesSerieReaderUndoCommandTestCase::cleanup(void)
{
    delete d->command;
    d->command = nullptr;
}

void gnomonImagesSerieReaderUndoCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieReaderUndoCommandTest, gnomonImagesSerieReaderUndoCommandTestCase);
