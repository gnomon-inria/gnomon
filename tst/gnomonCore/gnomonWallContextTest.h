#pragma once

#include <QtCore>

class gnomonWallContextTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonWallContextTestCase(void);
    ~gnomonWallContextTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void sphereExpansionTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);
};
