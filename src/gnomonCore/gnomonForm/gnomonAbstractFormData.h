#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonAbstractForm"

class GNOMONCORE_EXPORT gnomonAbstractFormData {
public:
             gnomonAbstractFormData(void) = default;
    virtual ~gnomonAbstractFormData(void) = default;

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

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;
};

