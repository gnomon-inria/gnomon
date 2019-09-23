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

#include <gnomonCoreExport>

//class dtkImage;
#include <dtkImagingCore>

class GNOMONCORE_EXPORT gnomonDataManager: public QObject
{
    Q_OBJECT

public:
    static gnomonDataManager *instance(void);

protected:
    static gnomonDataManager *s_instance;

public:
    dtkImage *get(const QString& name);

private:
     gnomonDataManager(void);
    ~gnomonDataManager(void);

private:
    class gnomonDataManagerPrivate *d;
};

//
// gnomonDataManager.h ends here
