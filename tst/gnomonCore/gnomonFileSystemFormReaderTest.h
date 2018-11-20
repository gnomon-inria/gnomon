#pragma once

#include <QtCore>

class gnomonFileSystemFormReaderTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonFileSystemFormReaderTestCase(void);
    ~gnomonFileSystemFormReaderTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void read();

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonFileSystemFormReaderTestCasePrivate *d;
};
