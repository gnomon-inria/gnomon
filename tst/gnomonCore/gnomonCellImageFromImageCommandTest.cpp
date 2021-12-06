#include "gnomonCellImageFromImageCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand>

#include <dtkScript>

#include <dtkImage>


namespace imagefromimage{
bool t_run_called = false;
bool t_set_input_called= false;
bool t_set_parameter_called = false;
}

class dummyImageFromImagePlugin : public gnomonAbstractCellImageFromImage {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override { imagefromimage::t_set_parameter_called = true;};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};

    void run(void) override{ imagefromimage::t_run_called = true;};
    QString documentation(void) override {return "empty";};
    void setInput(gnomonImageSeries *image_series) override { imagefromimage::t_set_input_called = true;};
    void setCellPoints(gnomonPointCloudSeries *pointCloud_series) override{};
    gnomonImageSeries *input(void) const override{ return nullptr;};
    gnomonPointCloudSeries *cellPoints(void) const override{ return nullptr;};
    gnomonCellImageSeries *output(void) const override{ return nullptr;};

    };

inline gnomonAbstractCellImageFromImage* dummyImageFromImagePluginCreator(void)  {
    return new dummyImageFromImagePlugin();
}


class gnomonCellImageFromImageCommandTestCasePrivate
{
public:
    gnomonImageSeries * image_series = nullptr;
    gnomonCellImageFromImageCommand * command_cellImageFromImage = nullptr;
};

gnomonCellImageFromImageCommandTestCase::gnomonCellImageFromImageCommandTestCase(void) : d(new gnomonCellImageFromImageCommandTestCasePrivate)
{
}

gnomonCellImageFromImageCommandTestCase::~gnomonCellImageFromImageCommandTestCase(void)
{
    delete d;
}

void gnomonCellImageFromImageCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
    gnomonCore::cellImageFromImage::pluginFactory().record("dummyImage", dummyImageFromImagePluginCreator);
}

void gnomonCellImageFromImageCommandTestCase::init(void)
{
    d->command_cellImageFromImage = new gnomonCellImageFromImageCommand();
    Q_ASSERT(d->command_cellImageFromImage);
    d->command_cellImageFromImage->setAlgorithmName("dummyImage");
}

void gnomonCellImageFromImageCommandTestCase::redo(void)
{
    d->command_cellImageFromImage->setInput(d->image_series);
    QVERIFY(imagefromimage::t_set_input_called);

    d->command_cellImageFromImage->setParameter("hmin", 1500);
    QVERIFY(imagefromimage::t_set_parameter_called);

    d->command_cellImageFromImage->redo();
    QVERIFY(imagefromimage::t_run_called);
}

void gnomonCellImageFromImageCommandTestCase::undo(void)
{
    d->command_cellImageFromImage->undo();
}

void gnomonCellImageFromImageCommandTestCase::cleanup(void)
{
    delete d->command_cellImageFromImage;
    d->command_cellImageFromImage = nullptr;
}

void gnomonCellImageFromImageCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonCellImageFromImageCommandTest, gnomonCellImageFromImageCommandTestCase);
