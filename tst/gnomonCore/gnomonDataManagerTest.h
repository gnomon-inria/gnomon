#pragma once

#include <QtCore>

class gnomonDataManagerTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonDataManagerTestCase(void);
    ~gnomonDataManagerTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void getExistingImageTest(void);
    void getNonExistingImageTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonDataManagerTestCasePrivate *d;

};
