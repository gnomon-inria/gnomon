#pragma once

#include <QtCore>

class gnomonImagesSerieFilterCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImagesSerieFilterCommandTestCase(void);
    ~gnomonImagesSerieFilterCommandTestCase(void);

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
    class gnomonImagesSerieFilterCommandTestCasePrivate *d;
};
