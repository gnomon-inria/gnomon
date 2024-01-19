#pragma once

#include <QtCore>

class gnomonFormWrappingTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonFormWrappingTestCase(void);
    ~gnomonFormWrappingTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void resourcesLoadingTest(void);
    void toFormSeriesTest(void);
    void fromFormSeriesTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonFormWrappingTestCasePrivate *d;
};
