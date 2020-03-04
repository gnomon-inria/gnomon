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

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataFrameData
{
public:
             gnomonAbstractDataFrameData(void) = default;
    virtual ~gnomonAbstractDataFrameData(void) {};

    virtual gnomonAbstractDataFrameData* clone(void) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////

public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  DataFrame concept
//  ///////////////////////////////////////////////////////////////////

public:
    virtual QList<long> index(void) const = 0;

    virtual QStringList columnNames(void) const = 0;
    virtual QMap<long, QVariant>& column(QString columnName) const = 0;

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractDataFrameData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataFrameData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataFrameData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataFrameData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataFrameData, GNOMONCORE_EXPORT, dataFrameData);
}

//
// gnomonDataFrameData.h ends here
