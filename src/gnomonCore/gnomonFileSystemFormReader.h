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

#include <gnomonCoreExport.h>

#include <QtCore>

#include "gnomonTime.h"

#pragma message "will inherit gnomonAbstractFormReader"
class GNOMONCORE_EXPORT gnomonFileSystemFormReader
{
public:
    gnomonFileSystemFormReader(const QVariantHash&);
    virtual ~gnomonFileSystemFormReader(void);

#pragma message "Possibly add also a specific read for a specific type of data at a given time"
    void read(const gnomonTime&);

    gnomonTime::Mode timeMode(void);
public:
    class gnomonFileSystemFormReaderPrivate *d;
};

//
// gnomonFileSystemFormReader.h ends here
