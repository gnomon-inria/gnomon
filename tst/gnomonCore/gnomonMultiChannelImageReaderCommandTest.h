#pragma once

#include <QtCore>

class gnomonMultiChannelImageReaderCommandTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonMultiChannelImageReaderCommandTestCase(void);
    ~gnomonMultiChannelImageReaderCommandTestCase(void);

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
    class gnomonMultiChannelImageReaderCommandTestCasePrivate *d;
};
