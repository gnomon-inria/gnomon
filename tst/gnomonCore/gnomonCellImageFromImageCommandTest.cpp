#include "gnomonCellImageFromImageCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand>

#include <dtkScript>

#include <dtkImage>


class gnomonCellImageFromImageCommandTestCasePrivate
{
public:
    gnomonImageSeries * image_series = nullptr;
    gnomonCellImageFromImageCommand * command_cellImageFromImage = nullptr;
};

gnomonCellImageFromImageCommandTestCase::gnomonCellImageFromImageCommandTestCase(void) : d(new gnomonCellImageFromImageCommandTestCasePrivate)
{
}

gnomonCellImageFromImageCommandTestCase::~gnomonCellImageFromImageCommandTestCase(void)
{
    delete d;
}

void gnomonCellImageFromImageCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonCellImageFromImageCommandTestCase::init(void)
{
    d->command_cellImageFromImage = new gnomonCellImageFromImageCommand("seededWatershedCellImageFromImageTimagetk");
    Q_ASSERT(d->command_cellImageFromImage);
}

void gnomonCellImageFromImageCommandTestCase::redo(void)
{
    gnomonImageReaderCommand* command = new gnomonImageReaderCommand("gnomonImageReader");
    Q_ASSERT(command);

    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
    command->setPath(image_file_path);
    command->redo();
    d->image_series = (gnomonImageSeries *) command->image()->clone();

    d->command_cellImageFromImage->setInput(d->image_series);

    d->command_cellImageFromImage->setParameter("hmin", 1500);
    d->command_cellImageFromImage->setParameter("channel", "CH2_PI");
    d->command_cellImageFromImage->setParameter("gaussian_sigma", 0.5);
    d->command_cellImageFromImage->setParameter("segmentation_gaussian_sigma", 0.25);
    d->command_cellImageFromImage->setParameter("volume_threshold", 1000);
    d->command_cellImageFromImage->setParameter("background_label", 1);

    d->command_cellImageFromImage->redo();

    delete command;
}

void gnomonCellImageFromImageCommandTestCase::undo(void)
{
    d->command_cellImageFromImage->undo();
}

void gnomonCellImageFromImageCommandTestCase::cleanup(void)
{
    delete d->command_cellImageFromImage;
    d->command_cellImageFromImage = nullptr;
}

void gnomonCellImageFromImageCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonCellImageFromImageCommandTest, gnomonCellImageFromImageCommandTestCase);
