#pragma once

#include <QtCore>

class gnomonCellImageTrackingCommandTestCase : public QObject
{
Q_OBJECT

public:
    gnomonCellImageTrackingCommandTestCase(void);
    ~gnomonCellImageTrackingCommandTestCase(void);

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
    class gnomonCellImageTrackingCommandTestCasePrivate *d;
};
