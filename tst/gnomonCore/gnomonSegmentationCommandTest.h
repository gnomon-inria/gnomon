#pragma once

#include <QtCore>

class gnomonSegmentationCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonSegmentationCommandTestCase(void);
    ~gnomonSegmentationCommandTestCase(void);

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
    class gnomonSegmentationCommandTestCasePrivate *d;
};
