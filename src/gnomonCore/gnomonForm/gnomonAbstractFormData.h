#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

class GNOMONCORE_EXPORT gnomonAbstractFormData {
public:
    virtual QString serialize(void) = 0;
    virtual void deserialize(QString) = 0;

public:
    // METADATA
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;
};

