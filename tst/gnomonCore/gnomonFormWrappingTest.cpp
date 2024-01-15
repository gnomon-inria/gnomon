#include "gnomonFormWrappingTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand>

#include <dtkScript>


class gnomonFormWrappingTestCasePrivate
{
public:
    std::shared_ptr<gnomonImageSeries>  image_series;
};

gnomonFormWrappingTestCase::gnomonFormWrappingTestCase(void) : d(new gnomonFormWrappingTestCasePrivate)
{
}

gnomonFormWrappingTestCase::~gnomonFormWrappingTestCase(void)
{
    delete d;
}

void gnomonFormWrappingTestCase::initTestCase(void)
{
    gnomonForm::registerForms();

    int stat;
    dtkScriptInterpreterPython::instance()->init();
    dtkScriptInterpreterPython::instance()->interpret("import sys", &stat);
    QString plugin_file_path = QFINDTESTDATA("../resources/");
    dtkScriptInterpreterPython::instance()->interpret("sys.path.append(\"" + plugin_file_path + "\")", &stat);
    dtkScriptInterpreterPython::instance()->interpret("from dummy_package import dummyImageData", &stat);
    dtkScriptInterpreterPython::instance()->interpret("from dummy_package import dummyImageReader", &stat);
    dtkScriptInterpreterPython::instance()->interpret("from dummy_package import dummyImageWriter", &stat);
}

void gnomonFormWrappingTestCase::init(void)
{
}

void gnomonFormWrappingTestCase::resourcesLoadingTest(void)
{
    QStringList plugins_keys = gnomonCore::imageData::pluginFactory().keys();
    QString msg = "ImageData keys are " + plugins_keys.join(", ");
    QVERIFY2(plugins_keys.contains("dummyImageData"), msg.toStdString().c_str()); 

    plugins_keys = gnomonCore::imageReader::pluginFactory().keys();
    msg = "ImageReader keys are " + plugins_keys.join(", ");
    QVERIFY2(plugins_keys.contains("dummyImageReader"), msg.toStdString().c_str()); 

    plugins_keys = gnomonCore::imageWriter::pluginFactory().keys();
    msg = "ImageWriter keys are " + plugins_keys.join(", ");
    QVERIFY2(plugins_keys.contains("dummyImageWriter"), msg.toStdString().c_str()); 

}

void gnomonFormWrappingTestCase::toFormSeriesTest(void)
{
    //create an instance of a python plugin
    auto dummyImgReader = gnomonCore::imageReader::pluginFactory().create("dummyImageReader");
    QVERIFY(dummyImgReader);

    //not really used as we are using a dummy plugin
    dummyImgReader->setPath(QFINDTESTDATA("../resources/rect_t0.inr"));

    // dummy image will be created here by the plugin
    dummyImgReader->run();

    //try to get a form series from this plugin
    d->image_series = dummyImgReader->image();
    QVERIFY(d->image_series);

    //check that the form series is of the correct type
    QVERIFY(d->image_series->formName() == "gnomonImage");
    QVERIFY(d->image_series->times().contains(0.));
}

void gnomonFormWrappingTestCase::fromFormSeriesTest(void)
{
    auto dummyImgWriter = gnomonCore::imageWriter::pluginFactory().create("dummyImageWriter");
    QVERIFY(dummyImgWriter);

    //create temp file, needs to open it to set the fileName
    QTemporaryFile file;
    if (file.open()) {
        file.close();
    } else {
        QFAIL("Could not create temp file");
    }

    dummyImgWriter->setPath(file.fileName());
    dummyImgWriter->setImage(d->image_series);
    dummyImgWriter->run();

    if (file.open()) {
        int res = file.readAll().toInt();
        QVERIFY(res == 1);
        file.close();
    } else {
        QFAIL("Could not open file");
    }
}



void gnomonFormWrappingTestCase::cleanup(void)
{
}

void gnomonFormWrappingTestCase::cleanupTestCase(void)
{
    d->image_series.reset();
}

GNOMONTEST_MAIN_NOGUI(gnomonFormWrappingTest, gnomonFormWrappingTestCase);
