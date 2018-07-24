#pragma once

#include <QtCore>

class gnomonExpansionSphereModelTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonExpansionSphereModelTestCase(void);
    ~gnomonExpansionSphereModelTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void sphereExpansionTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonExpansionSphereModelTestCasePrivate *d;
};
