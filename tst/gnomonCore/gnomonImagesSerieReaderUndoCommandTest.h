#pragma once

#include <QtCore>

class gnomonImagesSerieReaderUndoCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImagesSerieReaderUndoCommandTestCase(void);
    ~gnomonImagesSerieReaderUndoCommandTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void getCommand(void);
    void redo(void);
    void undo(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonImagesSerieReaderUndoCommandTestCasePrivate *d;
};
