#pragma once

#include <QtCore>

class gnomonImageFilterCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImageFilterCommandTestCase(void);
    ~gnomonImageFilterCommandTestCase(void);

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
    class gnomonImageFilterCommandTestCasePrivate *d;
};
