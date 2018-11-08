#pragma once

#include <QtCore>

class gnomonImagesSerieReaderCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImagesSerieReaderCommandTestCase(void);
    ~gnomonImagesSerieReaderCommandTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void redoInr(void);
    void undoInr(void);
    void redoCzi(void);
    void undoCzi(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonImagesSerieReaderCommandTestCasePrivate *d;
};
