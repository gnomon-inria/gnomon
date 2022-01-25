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
#include "gnomonForm/gnomonAbstractFormData"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractPointCloudData: public gnomonAbstractFormData
{
public:
             gnomonAbstractPointCloudData(void) = default;
    virtual ~gnomonAbstractPointCloudData(void) {};

    virtual gnomonAbstractPointCloudData* clone(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////

public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Point Id concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QList<long> pointIds(void) const = 0;
    virtual long pointCount(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Point edition concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual void setPointPosition(long pointId, double x, double y, double z) = 0;
    virtual long addPoint(double x, double y, double z) = 0;
    virtual void removePoint(long pointId) = 0;


//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////
    virtual QStringList pointPropertyNames(void) const = 0;
    virtual bool hasPointProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> pointProperty(const QString& propertyName) const = 0;
    virtual QMap<long, double> pointX(void) const = 0;
    virtual QMap<long, double> pointY(void) const = 0;
    virtual QMap<long, double> pointZ(void) const = 0;

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractPointCloudData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractPointCloudData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudData, GNOMONCORE_EXPORT, pointCloudData);
}

//
// gnomonAbstractPointCloudData.h ends here
