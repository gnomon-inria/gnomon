#include "gnomonImageRegistrationCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>


namespace registration{
bool t_run_called = false;
int t_add_image_called = 0;
bool t_set_parameter_called = false;
}

class dummyImageRegistrationPlugin : public gnomonAbstractImageRegistration {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {registration::t_set_parameter_called = true;};
    QMap<QString, gnomonCoreParameter *> parameters(void) const override {return QMap<QString, gnomonCoreParameter *>();};

    void run(void) override{ registration::t_run_called = true;};
    QString documentation(void) override {return "empty";};
    void addImage(gnomonImageSeries *) override {registration::t_add_image_called++;};
    void removeImages(void) override{registration::t_add_image_called = 0;};
    gnomonImageSeries* output() override {return nullptr;};
};

inline gnomonAbstractImageRegistration* dummyImageRegistrationPluginCreator(void)  {
    return new dummyImageRegistrationPlugin();
}

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
    gnomonCore::imageRegistration::pluginFactory().record("dummyImageRegistration", dummyImageRegistrationPluginCreator);

}

void gnomonImageRegistrationCommandTestCase::init(void)
{
    d->registration_command = new gnomonImageRegistrationCommand("dummyImageRegistration");
    Q_ASSERT(d->registration_command);
}

void gnomonImageRegistrationCommandTestCase::redo(void)
{
    d->image_series.push_back(new gnomonImageSeries());
    d->image_series.push_back(new gnomonImageSeries());
    d->image_series.push_back(new gnomonImageSeries());

    d->registration_command->addImage(d->image_series[0]);
    d->registration_command->addImage(d->image_series[1]);
    d->registration_command->addImage(d->image_series[2]);

    d->registration_command->setParameter("method", "rigid");
    d->registration_command->redo();

    QVERIFY(registration::t_add_image_called == 3 && registration::t_run_called && registration::t_set_parameter_called);
}

void gnomonImageRegistrationCommandTestCase::undo(void)
{
    d->registration_command->undo();
    QVERIFY(d->registration_command->inputs().count() == 0);
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
