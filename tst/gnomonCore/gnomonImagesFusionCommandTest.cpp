#include "gnomonImagesFusionCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesFusionCommand.h"
#include "gnomonImagesSerieReaderCommand.h"

#include <gnomonCore/gnomonImagesSerie>

#include <dtkScript>

#include <dtkImage>

using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class gnomonImagesFusionCommandTestCasePrivate
{
public:
    QVector< gnomonImagesSeriePtr > images_series;
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
    d->fusion_command = new gnomonImagesFusionCommand("gnomonImagesFusion");
    Q_ASSERT(d->fusion_command);
}

void gnomonImagesFusionCommandTestCase::redo(void)
{
    gnomonImagesSerieReaderCommand* command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(command);

    QString image_0_file_path = QFINDTESTDATA("../resources/time_0_cut_resampled.inr");
    command->setPath(image_0_file_path);
    command->redo();
    d->images_series.push_back(gnomonImagesSeriePtr(command->imagesSerie()));

    QString image_1_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated1_resampled.inr");
    command->setPath(image_1_file_path);
    command->redo();
    d->images_series.push_back(gnomonImagesSeriePtr(command->imagesSerie()));

    QString image_2_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated2_resampled.inr");
    command->setPath(image_2_file_path);
    command->redo();
    d->images_series.push_back(gnomonImagesSeriePtr(command->imagesSerie()));

    d->fusion_command->addImagesSerie(d->images_series[0].data());
    d->fusion_command->addImagesSerie(d->images_series[1].data());
    d->fusion_command->addImagesSerie(d->images_series[2].data());
 
    d->fusion_command->setParameter("nb_iterations", 0);
    d->fusion_command->setParameter("n_job", 1);
    d->fusion_command->redo();
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
