#pragma once

#include <gnomonCoreExport.h>

#include <memory>
#include <QtCore>
#include <dtkCore>

#include "gnomonDynamicFormMetadata"

class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void) = default;
    virtual ~gnomonAbstractDynamicForm(void) = default;

public:
    virtual gnomonAbstractDynamicForm *clone(void) const = 0;

public:
    virtual gnomonAbstractForm* at(double t) = 0;
    virtual gnomonAbstractForm* current(void) const = 0;
    //std::shared_ptr<gnomonAbstractForm> at(double t) const = 0;
    //std::shared_ptr<gnomonAbstractForm> current(void) const = 0;
    virtual double time(void) const = 0;
    virtual QList<double> times(void) const = 0;

    virtual void drop(double t) = 0;

    virtual QString formName(void) = 0;

    virtual QJsonObject serialize(void) {
        QJsonObject json;
        json["metadata"] = m_metadata.serialize();
        return json;
    }

    virtual void deserialize(QJsonObject & json) {
        m_metadata.deserialize(json);
    }

    gnomonDynamicFormMetadata& metadata(void) {
        return m_metadata;
    }

    void setMetadata(const gnomonDynamicFormMetadata& metadata) {
        m_metadata.clear();
        for(QString k : metadata.keys()) {
            m_metadata.set(k, metadata.get(k));
        }
    }

protected:
    gnomonDynamicFormMetadata m_metadata;
};
//
// gnomonAbstractDynamicForm.h ends here
