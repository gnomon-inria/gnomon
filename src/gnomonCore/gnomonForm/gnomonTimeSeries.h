#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractDynamicForm.h"

//template <typename T, typename Enable = std::enable_if_t<std::is_base_of<gnomonAbstractForm,T>::value>>
template <typename T>
class GNOMONCORE_EXPORT gnomonTimeSeries : public gnomonAbstractDynamicForm
{

public:
    explicit gnomonTimeSeries(void) : m_uuid(QUuid::createUuid().toString(QUuid::WithoutBraces)) {};
    gnomonTimeSeries(const gnomonTimeSeries& o);
    gnomonTimeSeries(const QString& uuid);
    ~gnomonTimeSeries(void) = default;

public:
    gnomonTimeSeries<T>& operator = (const gnomonTimeSeries<T>& o);

public:
    std::shared_ptr<gnomonAbstractDynamicForm> clone(void) const override;

public:
    std::shared_ptr<T> at(double t);
    std::shared_ptr<T> current(void) const;
    double time(void) const override;
    QList<double> times(void) const override;
    QMap<QString,QString> metadataAtT(double t) const override;

    void insert(double t, std::shared_ptr<T> form);
//    void insert(const T& form) override;
    void drop(double t) override;

    void compose(std::shared_ptr<gnomonAbstractDynamicForm> pForm) override;

public:
    virtual inline QString formName(void) override { return T::formName(); }

    virtual inline QString uuid(void) override { return this->m_uuid; };

    QJsonObject serialize(void) override;

    void deserialize(QJsonObject &serialization) override;

protected:
    T *at_impl(double t) override;
    T *current_impl(void) const override;

protected:
    QMap<double, std::shared_ptr<T>> m_forms;
    double m_current_time = 0.;
    QString m_uuid;
};

#include "gnomonTimeSeries.tpp"

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

//DTK_DECLARE_OBJECT        (gnomonTimeSeries<gnomonAbstractForm *> *)
//DTK_DECLARE_PLUGIN        (gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)
//GNOMON_DECLARE_PLUGIN_FACTORY(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)
////DTK_DECLARE_PLUGIN_MANAGER(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

//namespace gnomonCore {
//    GNOMON_DECLARE_CONCEPT(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT, discreteDynamicForm);
//}

//
// gnomonTimeSeries.h ends here
