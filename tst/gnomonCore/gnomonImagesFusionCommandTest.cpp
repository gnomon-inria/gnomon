#include "gnomonImagesFusionCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

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
    d->serie_reader_command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
    Q_ASSERT(d->serie_reader_command);

    d->fusion_command = new gnomonImagesFusionCommand("gnomonImagesFusion");
    Q_ASSERT(d->fusion_command);
}

void gnomonImagesFusionCommandTestCase::redo(void)
{
    QString image_0_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->serie_reader_command->setPath(image_0_file_path);
    d->serie_reader_command->redo();

    dtkImage *image_0 = d->serie_reader_command->at(0);
    qDebug() << "image_0" << image_0;

    QString image_1_file_path = QFINDTESTDATA("../resources/rect_t1.inr");
    d->serie_reader_command->setPath(image_1_file_path);
    d->serie_reader_command->redo();

    dtkImage *image_1 = d->serie_reader_command->at(0);

    d->fusion_command->addImage(image_0);
    d->fusion_command->addImage(image_0);

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
