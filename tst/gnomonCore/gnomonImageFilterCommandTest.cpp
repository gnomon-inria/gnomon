#include "gnomonImageFilterCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>

namespace filter{
bool t_run_called = false;
bool t_set_input_called = false;
bool t_set_parameter_called = false;
bool t_set_mask_called = false;
}

class dummyImageFilterPlugin : public gnomonAbstractImageFilter {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {filter::t_set_parameter_called = true;};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};

    int run(void) override{ filter::t_run_called = true; return 0;};
    QString documentation(void) override {return "empty";};
    void setInput(std::shared_ptr<gnomonImageSeries> image_series) override {filter::t_set_input_called = true;};
    std::shared_ptr<gnomonImageSeries> input() override {return nullptr;};
    std::shared_ptr<gnomonImageSeries> output() override {return nullptr;};
    std::shared_ptr<gnomonBinaryImageSeries> mask() override {return nullptr;}
    void setMask(std::shared_ptr<gnomonBinaryImageSeries> init) override {filter::t_set_mask_called = true;};
    void clearInputs(void) override {};
    void clearOutputs(void) override {};
};

inline gnomonAbstractImageFilter* dummyImageFilterPluginCreator(void)  {
    return new dummyImageFilterPlugin();
}


class gnomonImageFilterCommandTestCasePrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image_series;
    gnomonImageFilterCommand *filter_command = nullptr;
};

gnomonImageFilterCommandTestCase::gnomonImageFilterCommandTestCase(void) : d(new gnomonImageFilterCommandTestCasePrivate)
{
}

gnomonImageFilterCommandTestCase::~gnomonImageFilterCommandTestCase(void)
{
    delete d;
}

void gnomonImageFilterCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
    gnomonCore::imageFilter::pluginFactory().record("dummyImageFilter", dummyImageFilterPluginCreator);
}

void gnomonImageFilterCommandTestCase::init(void)
{
    d->filter_command = new gnomonImageFilterCommand();
    QVERIFY(d->filter_command);
    d->filter_command->setAlgorithmName("dummyImageFilter");
}

void gnomonImageFilterCommandTestCase::redo(void)
{
    d->filter_command->setInput(std::make_shared<gnomonImageSeries>());
    d->filter_command->setNoAsync();
    d->filter_command->redo();

    QVERIFY(filter::t_run_called && filter::t_set_input_called);

}

void gnomonImageFilterCommandTestCase::undo(void)
{
    d->filter_command->undo();
    QVERIFY(filter::t_set_input_called);
}

void gnomonImageFilterCommandTestCase::cleanup(void)
{
    delete d->filter_command;
    d->filter_command = nullptr;
}

void gnomonImageFilterCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageFilterCommandTest, gnomonImageFilterCommandTestCase);
