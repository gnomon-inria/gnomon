#include "gnomonTimeSeries.h"
#include "gnomonTime.h"

#include <QtGlobal>

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : gnomonAbstractDynamicForm()
{
    for (const auto& time : o.m_forms.keys()) {
        m_forms[time] = std::make_shared<T>(*(o.m_forms[time].get()));
    }
    m_current_time = o.m_current_time;
    m_uuid = o.m_uuid;
    *(this->p_metadata) = *(o.p_metadata);
}


template <typename T> std::shared_ptr<gnomonAbstractDynamicForm> gnomonTimeSeries<T>::clone(void) const
{
    qWarning() << Q_FUNC_INFO << "CLONE USED. check usage";

    gnomonTimeSeries<T> *o = new gnomonTimeSeries<T>(*this);
    return std::shared_ptr<gnomonAbstractDynamicForm>(o);
}


template <typename T> gnomonTimeSeries<T>& gnomonTimeSeries<T>::operator=(const gnomonTimeSeries<T>& o)
{
    if (this == &o)
        return *this;

    m_forms = o.m_forms;
    m_current_time = o.m_current_time;
    *(p_metadata) = *(o.p_metadata);

    return (*this);
}

template <typename T> std::shared_ptr<T> gnomonTimeSeries<T>::at(double t)
{
    if(m_forms.contains(t)) {
        m_current_time = t;
        return this->current();
    } else {
        dtkWarn() << Q_FUNC_INFO << "Invalid time position : the form is not defined at this time:" << t;
        return nullptr;
    }
}

template <typename T> T *gnomonTimeSeries<T>::at_impl(double t)
{
    qDebug() << Q_FUNC_INFO << "at_impl for t " << t << "TOCHECKKKKK";
    return this->at(t).get();
}

template <typename T> std::shared_ptr<T> gnomonTimeSeries<T>::current(void) const
{
    return m_forms[m_current_time];
}

template <typename T> T *gnomonTimeSeries<T>::current_impl(void) const
{
    qDebug() << Q_FUNC_INFO << "current_impl TOCHECKKKKK";
    return this->current().get();
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return m_current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return m_forms.keys();
}

template <typename T> QMap<QString,QString> gnomonTimeSeries<T>::metadataAtT(double t) const
{
    if(this->times().contains(t))
        return m_forms[t]->metadata();
    else
        return QMap<QString, QString>();
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, std::shared_ptr<T> form)
{
    if (m_forms.size() == 0) {
        m_current_time = t;
    }
    m_forms.insert(t, form);
}

//void gnomonTimeSeries::insert(const gnomonTimeSeries& dynamic_form)
//{
//    for(auto it = dynamic_form.m_forms.begin(); it != dynamic_form.m_forms.end(); ++it) { // Iterates on the times
//        if(m_forms.contains(it.key())) { // The time already exists
//            for(auto jt = it->begin(); jt != it->end(); ++jt) { // Iterates on the forms of a given time
//                auto forms = m_forms[it.key()];
//                if(!forms.contains(jt.key())) { // The form doesn't exist at the given time
//                    forms.insert(jt.key(), jt.value()); // Insert the form at the given time
//                } else { // The form already exists at the given time
//                    dtkError() << "Invalid form : the form is already defined at this time";
//                }
//            }
//        } else { // The time doesn't exist yet
//            m_forms.insert(it.key(), it.value());  // Insert the time and the form
//        }
//    }
//}

template <typename T> void gnomonTimeSeries<T>::drop(const double t)
{
    Q_ASSERT_X(m_forms.contains(t), "drop", "Invalid time position : the form is not defined at this time");
    m_forms.remove(t);
    if ((m_current_time = t) && (m_forms.size() > 0)) {
        m_current_time = m_forms.keys()[0];
    }
}

template<typename T>
QJsonObject gnomonTimeSeries<T>::serialize(void) {
    QJsonObject out(gnomonAbstractDynamicForm::serialize());
    out["current_time"] = m_current_time;
    out["uuid"] = m_uuid;
    QJsonObject forms;
    for(auto& t: times()) {
        forms[QString::number(t)] = m_forms[t]->serialize();
    }
    out["forms"] = forms;
    return out;
}

template<typename T>
void gnomonTimeSeries<T>::deserialize(QJsonObject &serialization) {
    gnomonAbstractDynamicForm::deserialize(serialization);

    m_current_time = serialization["current_time"].toDouble();
    m_uuid = serialization["uuid"].toString();
    // emptying current forms map and deleting forms
    m_forms.clear();
    QJsonObject forms = serialization["forms"].toObject();
    for(auto& key: forms.keys()) {
        auto formSerialization = forms[key].toObject();
        m_forms[key.toDouble()] = std::make_shared<T>(formSerialization);
    }
}

template<typename T>
void gnomonTimeSeries<T>::compose(std::shared_ptr<gnomonAbstractDynamicForm> pForm) {
    if(auto second_form = std::dynamic_pointer_cast<gnomonTimeSeries<T>>(pForm)) {
        auto times1 = this->times();
        auto times2 = pForm->times();
        double offset = times2.first() > times1.last() ? 0. : times1.last() - times2.first() + 1.;
        for(const auto &t: times2) {
            this->m_forms[t+offset] = second_form->at(t);
        }
    }
}

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

//namespace gnomonCore {
//    GNOMON_DEFINE_CONCEPT(gnomonTimeSeries, discreteDynamicForm, gnomonCore);
//}

//
// gnomonTimeSeries.tpp ends here
