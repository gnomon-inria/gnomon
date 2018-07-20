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
#include "gnomonAbstractForm.h"

#pragma message "will inherit gnomonAbstractFormReader"
class GNOMONCORE_EXPORT gnomonFileSystemFormReader
{
public:
#pragma message "Make it possible to have more parameters than just a file path"
    // gnomonFileSystemFormReader(const QVariantHash&);
    gnomonFileSystemFormReader(const QString&);
    virtual ~gnomonFileSystemFormReader(void);

#pragma message "Possibly add also a specific read for a specific type of data at a given time"
    gnomonAbstractForm * read(const gnomonTime&);
    // gnomonAbstractFormPtr read(const gnomonTime&);

    gnomonTime::Mode timeMode(void);

public:
    class gnomonFileSystemFormReaderPrivate *d;
};

//
// gnomonFileSystemFormReader.h ends here
