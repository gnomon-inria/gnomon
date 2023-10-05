#pragma once

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport.h>
#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonForm/gnomonAbstractFormData"

class vtkImageData;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellImageData: public gnomonAbstractFormData
{
public:
             gnomonAbstractCellImageData(void) = default;
    virtual ~gnomonAbstractCellImageData(void) {};

    virtual gnomonAbstractCellImageData* clone(void) const = 0;

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

public:
    virtual void setImage(vtkImageData* image) = 0;
    virtual vtkImageData* image(void) const = 0;

    virtual QList<long> cellIds(void) const = 0;
    virtual long cellCount(void) const = 0;
    virtual QList<long> adjacentCellIds(long cellId) const = 0;
    virtual long adjacentCellCount(long cellId) const = 0;

    virtual QStringList cellPropertyNames(void) const = 0;
    virtual bool hasCellProperty(const QString& propertyName) const = 0;

    virtual QMap<long, QVariant> cellProperty(const QString& propertyName) const = 0;

    virtual void addCellProperty(const QString& propertyName) = 0;
    virtual void updateCellProperty(const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) = 0;
    virtual void removeCellProperty(const QString& propertyName) = 0;

    virtual QMap<long, QVariant> computeCellProperty(const QString& propertyName) = 0;

    virtual QList<long> wallIds(void) const= 0;
    virtual QList<long> wallCellIds(long wallId) = 0;

    virtual QStringList wallPropertyNames(void) const = 0;
    virtual bool hasWallProperty(const QString& propertyName) const = 0;

    virtual QMap<long, QVariant> wallProperty(const QString& propertyName) const = 0;

    virtual void addWallProperty(const QString& propertyName) = 0;
    virtual void updateWallProperty(const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) = 0;
    virtual void removeWallProperty(const QString& propertyName) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractCellImageData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellImageData, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageData, GNOMONCORE_EXPORT, cellImageData, gnomonCore)


// gnomonAbstractCellImageData.h ends here
