#include "gnomonSegmentationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonSegmentationCommand>
#include <gnomonImagesSerieReaderCommand>

#include <dtkScript>

#include <dtkImage>

class gnomonSegmentationCommandTestCasePrivate
{
public:
    gnomonSegmentationCommand      *command_segmentation = nullptr;
    gnomonImagesSerie              *images_serie = nullptr;
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
    d->command_czi_reader = new gnomonImagesSerieReaderCommand("gnomonCziImageReader");
    Q_ASSERT(d->command_czi_reader);

    // d->command_segmentation = new gnomonSegmentationCommand("seededWatershedSegmentation");
    // Q_ASSERT(d->command_segmentation);
}

void gnomonSegmentationCommandTestCase::redo(void)
{
    // QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");

    // d->command_czi_reader->setPath(image_file_path);
    // d->command_czi_reader->redo();

    // qDebug() << "d->command_czi_reader->image(\"Ch2_PI\")" << d->command_czi_reader->image("Ch2_PI");
    // d->command_segmentation->setImage(d->command_czi_reader->image("Ch2_PI"));

    // QMap<QString, gnomonParameter*> parameters = d->command_segmentation->parameters();
    // d->command_segmentation->setParameter("hmin", 1500.);
    // d->command_segmentation->setParameter("gaussian_sigma", 0.5);
    // d->command_segmentation->setParameter("segmentation_gaussian_sigma", 0.25);
    // d->command_segmentation->setParameter("volume_threshold", 1000);
    // d->command_segmentation->setParameter("background_label", 1);

    // d->command_segmentation->redo();
}

void gnomonSegmentationCommandTestCase::undo(void)
{
    d->command_segmentation->undo();
}

void gnomonSegmentationCommandTestCase::cleanup(void)
{
    delete d->command_segmentation;
    d->command_segmentation = nullptr;

    delete d->command_czi_reader;
    d->command_czi_reader = nullptr;

    //dtkScriptInterpreterPython::instance()->release();
}

void gnomonSegmentationCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonSegmentationCommandTest, gnomonSegmentationCommandTestCase);
