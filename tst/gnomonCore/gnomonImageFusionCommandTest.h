#pragma once

#include <QtCore>

class gnomonImageFusionCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImageFusionCommandTestCase(void);
    ~gnomonImageFusionCommandTestCase(void);

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
    class gnomonImageFusionCommandTestCasePrivate *d;
};
