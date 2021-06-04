#pragma once

#include <QtCore>

class gnomonImageReaderCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImageReaderCommandTestCase(void);
    ~gnomonImageReaderCommandTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void readInr(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonImageReaderCommandTestCasePrivate *d;
};
