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
    void redo(void);
    void undo(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonImagesSerieReaderCommandTestCasePrivate *d;
};
