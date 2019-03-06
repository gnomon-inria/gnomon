#include "gnomonImagesSerieFilterCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerieFilterCommand.h"
#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class gnomonImagesSerieFilterCommandTestCasePrivate
{
public:
    gnomonImagesSeriePtr images_serie;
    gnomonImagesSerieFilterCommand *filter_command = nullptr;
};

gnomonImagesSerieFilterCommandTestCase::gnomonImagesSerieFilterCommandTestCase(void) : d(new gnomonImagesSerieFilterCommandTestCasePrivate)
{
}

gnomonImagesSerieFilterCommandTestCase::~gnomonImagesSerieFilterCommandTestCase(void)
{
    delete d;
}

void gnomonImagesSerieFilterCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImagesSerieFilterCommandTestCase::init(void)
{
    d->filter_command = new gnomonImagesSerieFilterCommand("morphoContrastTimagetk");
    Q_ASSERT(d->filter_command);
}

void gnomonImagesSerieFilterCommandTestCase::redo(void)
{
    gnomonImagesSerieReaderCommand* command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(command);

    QString image_0_file_path = QFINDTESTDATA("../resources/time_0_cut_resampled.inr");
    command->setPath(image_0_file_path);
    command->redo();
    d->images_serie = gnomonImagesSeriePtr(command->imagesSerie());

    d->filter_command->setInput(d->images_serie.data());
 
    d->filter_command->redo();

    delete command;
}

void gnomonImagesSerieFilterCommandTestCase::undo(void)
{
    d->filter_command->undo();
}

void gnomonImagesSerieFilterCommandTestCase::cleanup(void)
{
    delete d->filter_command;
    d->filter_command = nullptr;
}

void gnomonImagesSerieFilterCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesSerieFilterCommandTest, gnomonImagesSerieFilterCommandTestCase);
