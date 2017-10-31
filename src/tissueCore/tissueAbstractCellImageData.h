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

class dtkImage;

#include <QtCore>

#include <dtkCore>

#include <tissueCoreExport.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class TISSUECORE_EXPORT tissueAbstractCellImageData
{
public:
             tissueAbstractCellImageData(void) = default;
    virtual ~tissueAbstractCellImageData(void) {};

    virtual tissueAbstractCellImageData* clone(void) const = 0;

public:
    virtual void setImage(dtkImage *image) = 0;
    virtual const dtkImage& image(void) const = 0;

    virtual QList<long> cellIds(void) const = 0;
    virtual long cellCount(void) const = 0;
    virtual QList<long> adjacentCellIds(long cellId) const = 0;
    virtual long adjacentCellCount(long cellId) const = 0;

    virtual QList<QString> cellPropertyNames(void) const = 0;
    virtual bool hasCellProperty(const QString& propertyName) const = 0;

    virtual const QMap<long, QVariant>& cellProperty(const QString& propertyName) const = 0;
    virtual       QMap<long, QVariant>& cellProperty(const QString& propertyName) = 0;

    virtual void addCellProperty(const QString& propertyName) = 0;
    virtual void updateCellProperty(const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) = 0;
    virtual void removeCellProperty(const QString& propertyName) = 0;

    virtual QMap<long, QVariant>& computeCellProperty(const QString& propertyName) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (tissueAbstractCellImageData *)
DTK_DECLARE_PLUGIN        (tissueAbstractCellImageData, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(tissueAbstractCellImageData, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(tissueAbstractCellImageData, TISSUECORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to tissueCore layer
// /////////////////////////////////////////////////////////////////

namespace tissueCore {
    DTK_DECLARE_CONCEPT(tissueAbstractCellImageData, TISSUECORE_EXPORT, cellimagedata);
}

// tissueAbstractCellImageData.h ends here
