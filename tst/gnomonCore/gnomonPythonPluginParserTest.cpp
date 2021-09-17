#include "gnomonPythonPluginParserTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkScript>


class gnomonPythonPluginParserTestCasePrivate
{
public:
    gnomonPythonPluginParser *parser = nullptr;
};

gnomonPythonPluginParserTestCase::gnomonPythonPluginParserTestCase(void) : d(new gnomonPythonPluginParserTestCasePrivate)
{
}

gnomonPythonPluginParserTestCase::~gnomonPythonPluginParserTestCase(void)
{
    delete d;
}

void gnomonPythonPluginParserTestCase::initTestCase(void)
{
}

void gnomonPythonPluginParserTestCase::init(void)
{
    d->parser = new gnomonPythonPluginParser();
    QVERIFY(d->parser);
}

void gnomonPythonPluginParserTestCase::emptyParserTest(void)
{
     QMap<QString, gnomonFormDescription> input_forms;
     input_forms= d->parser->inputForms();
     QVERIFY(input_forms.size() == 0);
}

void gnomonPythonPluginParserTestCase::parseCodeTest(void)
{
    QString plugin_file_path = QFINDTESTDATA("../resources/gnomonImageAlgo.py");
    QFile plugin_file(plugin_file_path);

    QVERIFY(plugin_file.open(QFile::ReadOnly | QFile::Text));
    QTextStream in(&plugin_file);
    QString plugin_code = in.readAll();

    d->parser->parsePluginCode(plugin_code);

    QVERIFY(d->parser->inputForms().contains("in_img"));
    QVERIFY(d->parser->outputForms().contains("out_img"));
    QVERIFY(d->parser->parameters().contains("sigma"));
}

void gnomonPythonPluginParserTestCase::cleanup(void)
{
    delete d->parser;
    d->parser = nullptr;
}

void gnomonPythonPluginParserTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonPythonPluginParserTest, gnomonPythonPluginParserTestCase);
