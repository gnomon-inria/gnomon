#include "gnomonSegmentationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonSegmentationCommand>
#include <gnomonCore/gnomonImagesSerie>
#include <gnomonImagesSerieReaderCommand>

#include <dtkScript>

#include <dtkImage>

class gnomonSegmentationCommandTestCasePrivate
{
public:
    gnomonImagesSerie              *images_serie = nullptr;
    gnomonSegmentationCommand      *command_segmentation = nullptr;
};

gnomonSegmentationCommandTestCase::gnomonSegmentationCommandTestCase(void) : d(new gnomonSegmentationCommandTestCasePrivate)
{
}

gnomonSegmentationCommandTestCase::~gnomonSegmentationCommandTestCase(void)
{
    delete d;
}

void gnomonSegmentationCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonSegmentationCommandTestCase::init(void)
{
    d->command_segmentation = new gnomonSegmentationCommand("seededWatershedSegmentation");
    Q_ASSERT(d->command_segmentation);
}

void gnomonSegmentationCommandTestCase::redo(void)
{
    gnomonImagesSerieReaderCommand* command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(command);

    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
    command->setPath(image_file_path);
    command->redo();
    d->images_serie = new gnomonImagesSerie(*command->imagesSerie());
    d->images_serie->setChannel("Ch2_PI");

    d->command_segmentation->setInput(d->images_serie);

    d->command_segmentation->setParameter("hmin", 1500.);
    d->command_segmentation->setParameter("gaussian_sigma", 0.5);
    d->command_segmentation->setParameter("segmentation_gaussian_sigma", 0.25);
    d->command_segmentation->setParameter("volume_threshold", 1000);
    d->command_segmentation->setParameter("background_label", 1);

    d->command_segmentation->redo();
}

void gnomonSegmentationCommandTestCase::undo(void)
{
    d->command_segmentation->undo();
}

void gnomonSegmentationCommandTestCase::cleanup(void)
{
    delete d->command_segmentation;
    d->command_segmentation = nullptr;

    delete d->images_serie;
    d->images_serie = nullptr;
}

void gnomonSegmentationCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonSegmentationCommandTest, gnomonSegmentationCommandTestCase);
