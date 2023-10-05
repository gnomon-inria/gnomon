#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonForm/gnomonAbstractFormData"

class vtkImageData;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageData: public gnomonAbstractFormData
{
public:
             gnomonAbstractImageData(void) = default;
    virtual ~gnomonAbstractImageData(void) {};

    virtual gnomonAbstractImageData* clone(void) const = 0;

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

public:
    virtual vtkImageData *image(QString channel="") const = 0;
    virtual void setImage(vtkImageData *, QString channel="") = 0;

public:
    virtual void setChannelName(const QString& channel, const QString& name) = 0;

public:
    virtual QStringList channels(void) const = 0;

public:
    virtual int minValue(const QString& channel) const = 0;
    virtual int maxValue(const QString& channel) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageData, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageData, GNOMONCORE_EXPORT, imageData, gnomonCore)


//
// gnomonAbstractImageData.h ends here
