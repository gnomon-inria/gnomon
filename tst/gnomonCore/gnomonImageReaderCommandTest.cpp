#include "gnomonImageReaderCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

#include <dtkImage>

namespace reader {
bool t_set_path = false;
bool t_run_called = false;
}


class dummyImageReaderPlugin : public gnomonAbstractImageReader {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};

    void run(void) override{ reader::t_run_called = true;};
    QString documentation(void) override {return "empty";};
    void setPath(const QString& path) override {reader::t_set_path = true;};
    std::shared_ptr<gnomonImageSeries> image() override {return nullptr;};
    QStringList extensions(void) override {return QStringList();};
};

inline gnomonAbstractImageReader* dummyImageReaderPluginCreator(void)  {
    return new dummyImageReaderPlugin();
}

class gnomonImageReaderCommandTestCasePrivate
{
public:
    gnomonImageReaderCommand *command = nullptr;
};

gnomonImageReaderCommandTestCase::gnomonImageReaderCommandTestCase(void) : d(new gnomonImageReaderCommandTestCasePrivate)
{
}

gnomonImageReaderCommandTestCase::~gnomonImageReaderCommandTestCase(void)
{
    delete d;
}

void gnomonImageReaderCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
    gnomonCore::imageReader::pluginFactory().record("dummyImageReader", dummyImageReaderPluginCreator);
}

void gnomonImageReaderCommandTestCase::init(void)
{
    d->command = new gnomonImageReaderCommand();
    QVERIFY(d->command);
    d->command->setAlgorithmName("dummyImageReader");
}

void gnomonImageReaderCommandTestCase::readInr(void)
{
    QString image_file_path = QFINDTESTDATA("../resources/rect_t0.inr");
    d->command->setPath(image_file_path);
    QVERIFY(!reader::t_set_path);

    d->command->redo();
    QVERIFY(reader::t_set_path && reader::t_run_called);
}


void gnomonImageReaderCommandTestCase::cleanup(void)
{
    delete d->command;
    d->command = nullptr;
}

void gnomonImageReaderCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageReaderCommandTest, gnomonImageReaderCommandTestCase);
