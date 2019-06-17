#include "gnomonImageFusionCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFusionCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>

class gnomonImageFusionCommandTestCasePrivate
{
public:
    QVector< gnomonImageSeries * > image_series;
    gnomonImageFusionCommand *fusion_command = nullptr;
};

gnomonImageFusionCommandTestCase::gnomonImageFusionCommandTestCase(void) : d(new gnomonImageFusionCommandTestCasePrivate)
{
}

gnomonImageFusionCommandTestCase::~gnomonImageFusionCommandTestCase(void)
{
    delete d;
}

void gnomonImageFusionCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImageFusionCommandTestCase::init(void)
{
    d->fusion_command = new gnomonImageFusionCommand("gnomonImageFusion");
    Q_ASSERT(d->fusion_command);
}

void gnomonImageFusionCommandTestCase::redo(void)
{
    gnomonImageReaderCommand* command = new gnomonImageReaderCommand("gnomonImageReader");
    Q_ASSERT(command);

    QString image_0_file_path = QFINDTESTDATA("../resources/time_0_cut_resampled.inr");
    command->setPath(image_0_file_path);
    command->redo();
    d->image_series.push_back(command->image());

    QString image_1_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated1_resampled.inr");
    command->setPath(image_1_file_path);
    command->redo();
    d->image_series.push_back(command->image());

    QString image_2_file_path = QFINDTESTDATA("../resources/time_0_cut_rotated2_resampled.inr");
    command->setPath(image_2_file_path);
    command->redo();
    d->image_series.push_back(command->image());

    d->fusion_command->addImage(d->image_series[0]);
    d->fusion_command->addImage(d->image_series[1]);
    d->fusion_command->addImage(d->image_series[2]);
 
    d->fusion_command->setParameter("nb_iterations", 0);
    d->fusion_command->setParameter("n_job", 1);
    d->fusion_command->redo();
}

void gnomonImageFusionCommandTestCase::undo(void)
{
    d->fusion_command->undo();
}

void gnomonImageFusionCommandTestCase::cleanup(void)
{
    delete d->fusion_command;
    d->fusion_command = nullptr;
}

void gnomonImageFusionCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageFusionCommandTest, gnomonImageFusionCommandTestCase);
