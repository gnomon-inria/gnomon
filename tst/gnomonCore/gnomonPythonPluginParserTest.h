#pragma once

#include <QtCore>

class gnomonPythonPluginParserTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonPythonPluginParserTestCase(void);
    ~gnomonPythonPluginParserTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void emptyParserTest(void);
    void parseCodeTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonPythonPluginParserTestCasePrivate *d;
};
