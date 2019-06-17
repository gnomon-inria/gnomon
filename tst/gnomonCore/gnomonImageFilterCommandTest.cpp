#include "gnomonImageFilterCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>

class gnomonImageFilterCommandTestCasePrivate
{
public:
    gnomonImageSeries *image_series;
    gnomonImageFilterCommand *filter_command = nullptr;
};

gnomonImageFilterCommandTestCase::gnomonImageFilterCommandTestCase(void) : d(new gnomonImageFilterCommandTestCasePrivate)
{
}

gnomonImageFilterCommandTestCase::~gnomonImageFilterCommandTestCase(void)
{
    delete d;
}

void gnomonImageFilterCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImageFilterCommandTestCase::init(void)
{
    d->filter_command = new gnomonImageFilterCommand("morphoContrastTimagetk");
    Q_ASSERT(d->filter_command);
}

void gnomonImageFilterCommandTestCase::redo(void)
{
    gnomonImageReaderCommand* command = new gnomonImageReaderCommand("gnomonImageReader");
    Q_ASSERT(command);

    QString image_0_file_path = QFINDTESTDATA("../resources/time_0_cut_resampled.inr");
    command->setPath(image_0_file_path);
    command->redo();
    d->image_series = command->image();

    d->filter_command->setInput(d->image_series);
 
    d->filter_command->redo();

    delete command;
}

void gnomonImageFilterCommandTestCase::undo(void)
{
    d->filter_command->undo();
}

void gnomonImageFilterCommandTestCase::cleanup(void)
{
    delete d->filter_command;
    d->filter_command = nullptr;
}

void gnomonImageFilterCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageFilterCommandTest, gnomonImageFilterCommandTestCase);
