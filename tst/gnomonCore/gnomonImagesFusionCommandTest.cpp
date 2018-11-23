#include "gnomonImagesFusionCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerie.h"
#include "gnomonImagesFusionCommand.h"
#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

class gnomonImagesFusionCommandTestCasePrivate
{
public:
    gnomonImagesSerieReaderCommand *serie_reader_command = nullptr;
    gnomonImagesFusionCommand *fusion_command = nullptr;
};

gnomonImagesFusionCommandTestCase::gnomonImagesFusionCommandTestCase(void) : d(new gnomonImagesFusionCommandTestCasePrivate)
{
}

gnomonImagesFusionCommandTestCase::~gnomonImagesFusionCommandTestCase(void)
{
    delete d;
}

void gnomonImagesFusionCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImagesFusionCommandTestCase::init(void)
{
    // d->serie_reader_command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    // Q_ASSERT(d->serie_reader_command);

    // d->fusion_command = new gnomonImagesFusionCommand("gnomonImagesFusion");
    // Q_ASSERT(d->fusion_command);
}

void gnomonImagesFusionCommandTestCase::redo(void)
{
    // QString image_0_file_path = QFINDTESTDATA("../resources/time_0_cut_resampled.inr");
    // d->serie_reader_command->setPath(image_0_file_path);
    // d->serie_reader_command->redo();
    // dtkImage *image_0 = new dtkImage(*d->serie_reader_command->image());
    // gnomonImagesSerie *images_serie_0 = new gnomonImagesSerie;
    // images_serie_0->setImage(image_0);

    // QString image_1_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated1_resampled.inr");
    // d->serie_reader_command->setPath(image_1_file_path);
    // d->serie_reader_command->redo();
    // dtkImage *image_1 = new dtkImage(*d->serie_reader_command->image());
    // gnomonImagesSerie *images_serie_1 = new gnomonImagesSerie;
    // images_serie_1->setImage(image_1);

    // QString image_2_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated2_resampled.inr");
    // d->serie_reader_command->setPath(image_2_file_path);
    // d->serie_reader_command->redo();
    // dtkImage *image_2 = new dtkImage(*d->serie_reader_command->image());
    // gnomonImagesSerie *images_serie_2 = new gnomonImagesSerie;
    // images_serie_2->setImage(image_2);

    // d->fusion_command->addImagesSerie(images_serie_0);
    // d->fusion_command->addImagesSerie(images_serie_1);
    // d->fusion_command->addImagesSerie(images_serie_2);
 
    // d->fusion_command->setParameter("nb_iterations", 0);
    // d->fusion_command->setParameter("n_job", 1);
    // d->fusion_command->redo();
}

void gnomonImagesFusionCommandTestCase::undo(void)
{
    d->fusion_command->undo();
}

void gnomonImagesFusionCommandTestCase::cleanup(void)
{
    delete d->fusion_command;
    d->fusion_command = nullptr;
}

void gnomonImagesFusionCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesFusionCommandTest, gnomonImagesFusionCommandTestCase);
