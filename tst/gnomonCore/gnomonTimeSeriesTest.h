#pragma once

#include <QtCore>

class gnomonTimeSeriesTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonTimeSeriesTestCase(void);
    ~gnomonTimeSeriesTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void createEmptyFormTest(void);
    void createFormSerieFromStringTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonTimeSeriesTestCasePrivate *d;

};
