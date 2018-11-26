#include "gnomonImagesRegistrationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonImagesSerie.h"
#include "gnomonImagesRegistrationCommand.h"
#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

#include <dtkImage>

using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class gnomonImagesRegistrationCommandTestCasePrivate
{
public:
    QVector< gnomonImagesSeriePtr > images_series;
    gnomonImagesRegistrationCommand *registration_command = nullptr;
};

gnomonImagesRegistrationCommandTestCase::gnomonImagesRegistrationCommandTestCase(void) : d(new gnomonImagesRegistrationCommandTestCasePrivate)
{
}

gnomonImagesRegistrationCommandTestCase::~gnomonImagesRegistrationCommandTestCase(void)
{
    delete d;
}

void gnomonImagesRegistrationCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
}

void gnomonImagesRegistrationCommandTestCase::init(void)
{
    d->registration_command = new gnomonImagesRegistrationCommand("gnomonImagesRegistration");
    Q_ASSERT(d->registration_command);
}

void gnomonImagesRegistrationCommandTestCase::redo(void)
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

    d->registration_command->addImagesSerie(d->images_series[0].data());
    d->registration_command->addImagesSerie(d->images_series[1].data());
    d->registration_command->addImagesSerie(d->images_series[2].data());
 
    d->registration_command->setParameter("method", "rigid");
    d->registration_command->redo();

    delete command;
}

void gnomonImagesRegistrationCommandTestCase::undo(void)
{
    d->registration_command->undo();
}

void gnomonImagesRegistrationCommandTestCase::cleanup(void)
{
    delete d->registration_command;
    d->registration_command = nullptr;
}

void gnomonImagesRegistrationCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImagesRegistrationCommandTest, gnomonImagesRegistrationCommandTestCase);
