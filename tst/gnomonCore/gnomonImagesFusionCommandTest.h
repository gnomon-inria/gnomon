#pragma once

#include <QtCore>

class gnomonImagesFusionCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImagesFusionCommandTestCase(void);
    ~gnomonImagesFusionCommandTestCase(void);

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
    class gnomonImagesFusionCommandTestCasePrivate *d;
};
