#pragma once

#include <QtCore>

class gnomonImageRegistrationCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImageRegistrationCommandTestCase(void);
    ~gnomonImageRegistrationCommandTestCase(void);

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
    class gnomonImageRegistrationCommandTestCasePrivate *d;
};
