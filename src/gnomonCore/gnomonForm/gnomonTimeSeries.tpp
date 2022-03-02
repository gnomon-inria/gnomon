#include "gnomonTimeSeries.h"
#include "gnomonTime.h"

#include <QtGlobal>


template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(void): gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{

}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{
    for (const auto& time : o.d_ptr()->forms.keys()) {
        d_ptr()->forms[time] = dynamic_cast<T *>(o.d_ptr()->forms[time]->clone());
    }
    d_ptr()->current_time = o.d_ptr()->current_time;
    delete d->metadata;
    d->metadata = new gnomonDynamicFormMetadata(*o.d->metadata);
}

template <typename T> gnomonTimeSeries<T>::~gnomonTimeSeries(void)
{
}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(gnomonTimeSeriesPrivate<T>* otherPrivate) : gnomonAbstractDynamicForm(otherPrivate)
{
}

template <typename T> gnomonAbstractDynamicForm *gnomonTimeSeries<T>::clone(void) const
{
    return new gnomonTimeSeries(*this);
}

template <typename T> gnomonTimeSeries<T>& gnomonTimeSeries<T>::operator=(const gnomonTimeSeries<T>& o)
{
    if (this == &o)
        return *this;

    d_ptr()->forms = o.d_ptr()->forms;
    d_ptr()->current_time = o.d_ptr()->current_time;

    return (*this);
}

template <typename T> T *gnomonTimeSeries<T>::at(double t) const
{
    Q_ASSERT_X(d_ptr()->forms.contains(t), "at", "Invalid time position : the form is not defined at this time");

    d_ptr()->current_time = t;
    return this->current();
}

template <typename T> T *gnomonTimeSeries<T>::current(void) const
{
    return d_ptr()->forms[d_ptr()->current_time];
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return d_ptr()->current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return d_ptr()->forms.keys();
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, T* form)
{
    //Q_ASSERT_X(d_ptr()->forms.contains(t), "insert", "Invalid time position : the form is already defined at this time");
    qDebug() << Q_FUNC_INFO << d_ptr()->forms;
    qDebug() << Q_FUNC_INFO << d_ptr()->forms.size();
    if (d_ptr()->forms.size() == 0) {
        d_ptr()->current_time = t;
    }
    d_ptr()->forms.insert(t, form);
}

//void gnomonTimeSeries::insert(const gnomonTimeSeries& dynamic_form)
//{
//    for(auto it = dynamic_form.d_ptr()->forms.begin(); it != dynamic_form.d_ptr()->forms.end(); ++it) { // Iterates on the times
//        if(d_ptr()->forms.contains(it.key())) { // The time already exists
//            for(auto jt = it->begin(); jt != it->end(); ++jt) { // Iterates on the forms of a given time
//                auto forms = d_ptr()->forms[it.key()];
//                if(!forms.contains(jt.key())) { // The form doesn't exist at the given time
//                    forms.insert(jt.key(), jt.value()); // Insert the form at the given time
//                } else { // The form already exists at the given time
//                    dtkError() << "Invalid form : the form is already defined at this time";
//                }
//            }
//        } else { // The time doesn't exist yet
//            d_ptr()->forms.insert(it.key(), it.value());  // Insert the time and the form
//        }
//    }
//}

template <typename T> void gnomonTimeSeries<T>::drop(const double t)
{
    Q_ASSERT_X(d_ptr()->forms.contains(t), "drop", "Invalid time position : the form is not defined at this time");
    d_ptr()->forms.remove(t);
    if ((d_ptr()->current_time = t) && (d_ptr()->forms.size() > 0)) {
        d_ptr()->current_time = d_ptr()->forms.keys()[0];
    }
}

template<typename T>
QJsonObject gnomonTimeSeries<T>::serialize(void) {
    QJsonObject out(gnomonAbstractDynamicForm::serialize());
    out["current_time"] = d_ptr()->current_time;
    QJsonObject forms;
    for(auto& t: times()) {
        forms[QString::number(t)] = d_ptr()->forms[t]->serialize();
    }
    out["forms"] = forms;
    return out;
}

template<typename T>
void gnomonTimeSeries<T>::deserialize(QJsonObject &serialization) {
    gnomonAbstractDynamicForm::deserialize(serialization);

    d_ptr()->current_time = serialization["current_time"].toDouble();
    // emptying current forms map and deleting forms
    for(auto& key: d_ptr()->forms.keys()) {
        delete d_ptr()->forms[key];
        d_ptr()->forms[key] = nullptr;
    }
    // filling back
    d_ptr()->forms.clear();
    QJsonObject forms = serialization["forms"].toObject();
    for(auto& key: forms.keys()) {
        auto formSerialization = forms[key].toObject();
        d_ptr()->forms[key.toDouble()] = new T(formSerialization);
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
