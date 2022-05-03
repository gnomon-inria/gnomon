#include "gnomonTimeSeries.h"
#include "gnomonTime.h"

#include <QtGlobal>

/*
template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(void): gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{

}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{
    for (const auto& time : o.m_forms.keys()) {
        m_forms[time] = dynamic_cast<T *>(o.m_forms[time]->clone());
    }
    m_current_time = o.m_current_time;
    delete d->metadata;
    d->metadata = new gnomonDynamicFormMetadata(*o.d->metadata);
}

template <typename T> gnomonTimeSeries<T>::~gnomonTimeSeries(void)
{
}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(gnomonTimeSeriesPrivate<T>* otherPrivate) : gnomonAbstractDynamicForm(otherPrivate)
{
}
*/

template <typename T> gnomonAbstractDynamicForm *gnomonTimeSeries<T>::clone(void) const
{
    gnomonTimeSeries<T> *o = new gnomonTimeSeries<T>();
    o->m_forms = this->m_forms;
    o->m_current_time = this->m_current_time;
    o->setMetadata(this->m_metadata);
    return o;
}

template <typename T> gnomonTimeSeries<T>& gnomonTimeSeries<T>::operator=(const gnomonTimeSeries<T>& o)
{
    if (this == &o)
        return *this;

    m_forms = o.m_forms;
    m_current_time = o.m_current_time;
    this->setMetadata(o.m_metadata);

    return (*this);
}

template <typename T> T *gnomonTimeSeries<T>::at(double t)
{
    if(m_forms.contains(t)) {
        m_current_time = t;
        return this->current();
    } else { 
        dtkWarn() << Q_FUNC_INFO << "Invalid time position : the form is not defined at this time:" << t;
        return nullptr;
    }
}

template <typename T> T *gnomonTimeSeries<T>::current(void) const
{
    return m_forms[m_current_time];
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return m_current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return m_forms.keys();
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, T* form)
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
    // emptying current forms map and deleting forms
    m_forms.clear();
    QJsonObject forms = serialization["forms"].toObject();
    for(auto& key: forms.keys()) {
        auto formSerialization = forms[key].toObject();
        m_forms[key.toDouble()] = new T(formSerialization);
    }
}

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

//namespace gnomonCore {
//    DTK_DEFINE_CONCEPT(gnomonTimeSeries, discreteDynamicForm, gnomonCore);
//}

//
// gnomonTimeSeries.tpp ends here
