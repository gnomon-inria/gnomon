#include "gnomonSegmentationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonSegmentationCommand>
#include <gnomonCore/gnomonForm/gnomonImagesSerie>
#include <gnomonImagesSerieReaderCommand>

#include <dtkScript>

#include <dtkImage>

using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class gnomonSegmentationCommandTestCasePrivate
{
public:
    gnomonImagesSeriePtr           images_serie = gnomonImagesSeriePtr(nullptr);
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
    d->command_segmentation = new gnomonSegmentationCommand("seededWatershedSegmentationTimagetk");
    Q_ASSERT(d->command_segmentation);
}

void gnomonSegmentationCommandTestCase::redo(void)
{
    gnomonImagesSerieReaderCommand* command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(command);

    QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
    command->setPath(image_file_path);
    command->redo();
    d->images_serie = gnomonImagesSeriePtr(command->imagesSerie());
    d->images_serie->setChannel("Ch2_PI");

    d->command_segmentation->setInput(d->images_serie.data());

    d->command_segmentation->setParameter("hmin", 1500);
    d->command_segmentation->setParameter("gaussian_sigma", 0.5);
    d->command_segmentation->setParameter("segmentation_gaussian_sigma", 0.25);
    d->command_segmentation->setParameter("volume_threshold", 1000);
    d->command_segmentation->setParameter("background_label", 1);

    d->command_segmentation->redo();

    delete command;
}

void gnomonSegmentationCommandTestCase::undo(void)
{
    d->command_segmentation->undo();
}

void gnomonSegmentationCommandTestCase::cleanup(void)
{
    delete d->command_segmentation;
    d->command_segmentation = nullptr;
}

void gnomonSegmentationCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonSegmentationCommandTest, gnomonSegmentationCommandTestCase);
