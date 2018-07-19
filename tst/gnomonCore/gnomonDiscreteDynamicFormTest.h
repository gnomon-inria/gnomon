// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:


#pragma once

#include <QtCore>

class gnomonDiscreteDynamicFormTestCase : public QObject
{
    Q_OBJECT

public:
    gnomonDiscreteDynamicFormTestCase(void);
    ~gnomonDiscreteDynamicFormTestCase(void);

private slots:
    void initTestCase(void);
    void init(void);

private slots:
    void createEmptyFormTest(void);

private slots:
    void cleanupTestCase(void);
    void cleanup(void);

private:
    class gnomonDiscreteDynamicFormTestCasePrivate *d;

};
