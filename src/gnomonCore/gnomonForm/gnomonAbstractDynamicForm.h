#pragma once

#include <gnomonCoreExport.h>

#include <memory>
#include <QtCore>
#include <dtkCore>

#include "gnomonAbstractForm"
#include "gnomonDynamicFormMetadata"

class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void) : p_metadata(new gnomonDynamicFormMetadata) {};
    virtual ~gnomonAbstractDynamicForm(void) {delete p_metadata;}


public:
    virtual std::shared_ptr<gnomonAbstractDynamicForm> clone(void) const = 0;

public:
    std::shared_ptr<gnomonAbstractForm> at(double t) {
        qWarning() << Q_FUNC_INFO << "MAKING A CLONE";
        return std::shared_ptr<gnomonAbstractForm>(this->at_impl(t)->clone());
    };
    std::shared_ptr<gnomonAbstractForm> current(void) const {
        qWarning() << Q_FUNC_INFO << "MAKING A CLONE";
        return std::shared_ptr<gnomonAbstractForm>(this->current_impl()->clone());
    };

    virtual QMap<QString,QString> metadataAtT(double t) const = 0;

    virtual double time(void) const = 0;
    virtual QList<double> times(void) const = 0;
    virtual void compose(std::shared_ptr<gnomonAbstractDynamicForm> pForm) = 0;

    virtual void drop(double t) = 0;

    virtual QString formName(void) = 0; //TODO CRTP to be able to put that as virtual static

    virtual QJsonObject serialize(void) {
        QJsonObject json;
        json["metadata"] = p_metadata->serialize();
        return json;
    }

    virtual void deserialize(QJsonObject & json) {
        p_metadata->deserialize(json);
    }

    gnomonDynamicFormMetadata* metadata(void) {
        return p_metadata;
    }

protected:
    virtual gnomonAbstractForm *at_impl(double t) = 0;
    virtual gnomonAbstractForm *current_impl(void) const = 0;

protected:
    gnomonDynamicFormMetadata *p_metadata = nullptr;
};
//
// gnomonAbstractDynamicForm.h ends here
