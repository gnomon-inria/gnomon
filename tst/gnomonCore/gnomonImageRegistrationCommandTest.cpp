#include "gnomonImageRegistrationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>


namespace registration{
bool t_run_called = false;
bool t_set_image_called = false;
bool t_set_parameter_called = false;
}

class dummyImageRegistrationPlugin : public gnomonAbstractImageRegistration {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {registration::t_set_parameter_called = true;};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};
    void updateParameters(void){};

    void run(void) override{ registration::t_run_called = true;};
    QString documentation(void) override {return "empty";};
    void setImage(std::shared_ptr<gnomonImageSeries> imageSeries) override {registration::t_set_image_called = true;};
    std::shared_ptr<gnomonImageSeries> image() override {return nullptr;};
    std::shared_ptr<gnomonImageSeries> output() override {return nullptr;};
    std::shared_ptr<gnomonDataDictSeries> outputTransformation() override {return nullptr;};
    void clearInputs(void) override {};
    void clearOutputs(void) override {};
};

inline gnomonAbstractImageRegistration* dummyImageRegistrationPluginCreator(void)  {
    return new dummyImageRegistrationPlugin();
}

class gnomonImageRegistrationCommandTestCasePrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image_series;
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
    gnomonCore::imageRegistration::pluginFactory().record("dummyImageRegistration", dummyImageRegistrationPluginCreator);

}

void gnomonImageRegistrationCommandTestCase::init(void)
{
    d->registration_command = new gnomonImageRegistrationCommand();
    Q_ASSERT(d->registration_command);
    d->registration_command->setAlgorithmName("dummyImageRegistration");
}

void gnomonImageRegistrationCommandTestCase::redo(void)
{
    d->image_series = std::make_shared<gnomonImageSeries>();
    d->registration_command->setImage(d->image_series);

    d->registration_command->setParameter("method", "rigid");
    d->registration_command->setNoAsync();
    d->registration_command->redo();

    QVERIFY(registration::t_set_image_called && registration::t_run_called && registration::t_set_parameter_called);
}

void gnomonImageRegistrationCommandTestCase::undo(void)
{
    d->registration_command->undo();
    QVERIFY(registration::t_set_image_called);
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
