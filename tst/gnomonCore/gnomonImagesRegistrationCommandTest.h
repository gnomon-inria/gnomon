#pragma once

#include <QtCore>

class gnomonImagesRegistrationCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonImagesRegistrationCommandTestCase(void);
    ~gnomonImagesRegistrationCommandTestCase(void);

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
    class gnomonImagesRegistrationCommandTestCasePrivate *d;
};
