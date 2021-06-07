#pragma once

#include <QtCore>

class gnomonCellImageFromImageCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonCellImageFromImageCommandTestCase(void);
    ~gnomonCellImageFromImageCommandTestCase(void);

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
    class gnomonCellImageFromImageCommandTestCasePrivate *d;
};
