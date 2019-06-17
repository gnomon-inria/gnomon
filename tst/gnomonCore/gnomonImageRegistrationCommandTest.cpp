#include "gnomonImageRegistrationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>


class gnomonImageRegistrationCommandTestCasePrivate
{
public:
    QVector< gnomonImageSeries * > image_series;
    gnomonImageRegistrationCommand *registration_command = nullptr;
};

gnomonImageRegistrationCommandTestCase::gnomonImageRegistrationCommandTestCase(void) : d(new gnomonImageRegistrationCommandTestCasePrivate)
{
}

gnomonImageRegistrationCommandTestCase::~gnomonImageRegistrationCommandTestCase(void)
{
    delete d;
}

void gnomonImageRegistrationCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImageRegistrationCommandTestCase::init(void)
{
    d->registration_command = new gnomonImageRegistrationCommand("backwardRegistrationTimagetk");
    Q_ASSERT(d->registration_command);
}

void gnomonImageRegistrationCommandTestCase::redo(void)
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

    d->registration_command->addImage(d->image_series[0]);
    d->registration_command->addImage(d->image_series[1]);
    d->registration_command->addImage(d->image_series[2]);
 
    d->registration_command->setParameter("method", "rigid");
    d->registration_command->redo();

    delete command;
}

void gnomonImageRegistrationCommandTestCase::undo(void)
{
    d->registration_command->undo();
}

void gnomonImageRegistrationCommandTestCase::cleanup(void)
{
    delete d->registration_command;
    d->registration_command = nullptr;
}

void gnomonImageRegistrationCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageRegistrationCommandTest, gnomonImageRegistrationCommandTestCase);
