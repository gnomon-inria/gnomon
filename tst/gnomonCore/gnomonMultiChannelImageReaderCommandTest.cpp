#include "gnomonMultiChannelImageReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include "gnomonMultiChannelImage.h"
#include "gnomonMultiChannelImageReaderCommand.h"

#include <vtkImageData.h>

#include <dtkScript>

class gnomonMultiChannelImageReaderCommandTestCasePrivate
{
public:
    gnomonMultiChannelImageReaderCommand *undo_command = nullptr;
};

gnomonMultiChannelImageReaderCommandTestCase::gnomonMultiChannelImageReaderCommandTestCase(void) : d(new gnomonMultiChannelImageReaderCommandTestCasePrivate)
{
}

gnomonMultiChannelImageReaderCommandTestCase::~gnomonMultiChannelImageReaderCommandTestCase(void)
{
    delete d;
}

void gnomonMultiChannelImageReaderCommandTestCase::initTestCase(void)
{
}

void gnomonMultiChannelImageReaderCommandTestCase::init(void)
{
    d->undo_command = new gnomonMultiChannelImageReaderCommand("gnomonMultiChannelImageReaderCzi");
    Q_ASSERT(d->undo_command);
}

void gnomonMultiChannelImageReaderCommandTestCase::redo(void)
{
   QString image_file_path = QFINDTESTDATA("../resources/qDII-CLV3-PIN1-PI-E35-LD-SAM1-T0-Subset.czi");
   d->undo_command->setPath(image_file_path);

   d->undo_command->redo();

   gnomonMultiChannelImage *image = d->undo_command->image();

   QStringList true_list = {"ChS1_EYFP", "Ch1_EBFP", "Ch2_PI", "Ch2_mCherry", "ChS1_EGFP"};

   QCOMPARE(image->channelNames(), true_list);
   //qDebug() << image->channelImage("ChS1_EYFP")->GetNumberOfPoints();
   //qDebug() << image->channelImage("ChS1_EYFP")->GetNumberOfCells();
}

void gnomonMultiChannelImageReaderCommandTestCase::undo(void)
{
    d->undo_command->undo();
}

void gnomonMultiChannelImageReaderCommandTestCase::cleanup(void)
{
    delete d->undo_command;
    d->undo_command = nullptr;
}

void gnomonMultiChannelImageReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonMultiChannelImageReaderCommandTest, gnomonMultiChannelImageReaderCommandTestCase);
