#pragma once

#include <QtCore>

class gnomonSystemScenarioTestCase : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void sphereExpansionLambdaTest(void);
    void sphereExpansionClassTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);
};
