#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

class GNOMONCORE_EXPORT gnomonAbstractFormData {
public:
    virtual QString serialize(void) {
        dtkError() << Q_FUNC_INFO << "No serialization defined !";
        return "";
    };
    virtual void deserialize(QString) {
        dtkError() << Q_FUNC_INFO << "No deserialization defined !";
    };

public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;
};

