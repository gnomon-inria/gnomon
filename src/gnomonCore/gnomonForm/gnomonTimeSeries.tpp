// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonCore.h"
#include "gnomonTime.h"

#include <QtGlobal>


// ///////////////////////////////////////////////////////////////////
// gnomonTimeSeries
// ///////////////////////////////////////////////////////////////////

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(void) : d(new gnomonTimeSeriesPrivate<T>)
{
}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : d(new gnomonTimeSeriesPrivate<T>)
{
    for (const auto& time : o.d->forms.keys()) {
        d->forms[time] = dynamic_cast<T *>(o.d->forms[time]->clone());
    }
    d->current_time = o.d->current_time;
    delete metadata;
    metadata = new gnomonDynamicFormMetadata(*o.metadata);
}

template <typename T> gnomonTimeSeries<T>::~gnomonTimeSeries(void)
{
    delete d;
}

template <typename T> gnomonAbstractDynamicForm *gnomonTimeSeries<T>::clone(void) const
{
    return new gnomonTimeSeries(*this);
};

template <typename T> gnomonTimeSeries<T>& gnomonTimeSeries<T>::operator=(const gnomonTimeSeries<T>& o)
{
    if (this == &o)
        return *this;

    d->forms = o.d->forms;
    d->current_time = o.d->current_time;

    return (*this);
}

template <typename T> T *gnomonTimeSeries<T>::at(double t) const
{
    Q_ASSERT_X(d->forms.contains(t), "at", "Invalid time position : the form is not defined at this time");

    d->current_time = t;
    return this->current();
}

template <typename T> T *gnomonTimeSeries<T>::current(void) const
{
    return d->forms[d->current_time];
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return d->current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return d->forms.keys();
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, T* form)
{
    //Q_ASSERT_X(d->forms.contains(t), "insert", "Invalid time position : the form is already defined at this time");
    if (d->forms.size() == 0) {
        d->current_time = t;
    }
    d->forms.insert(t, form);
}

//void gnomonTimeSeries::insert(const gnomonTimeSeries& dynamic_form)
//{
//    for(auto it = dynamic_form.d->forms.begin(); it != dynamic_form.d->forms.end(); ++it) { // Iterates on the times
//        if(d->forms.contains(it.key())) { // The time already exists
//            for(auto jt = it->begin(); jt != it->end(); ++jt) { // Iterates on the forms of a given time
//                auto forms = d->forms[it.key()];
//                if(!forms.contains(jt.key())) { // The form doesn't exist at the given time
//                    forms.insert(jt.key(), jt.value()); // Insert the form at the given time
//                } else { // The form already exists at the given time
//                    dtkError() << "Invalid form : the form is already defined at this time";
//                }
//            }
//        } else { // The time doesn't exist yet
//            d->forms.insert(it.key(), it.value());  // Insert the time and the form
//        }
//    }
//}

template <typename T> void gnomonTimeSeries<T>::drop(const double t)
{
    Q_ASSERT_X(d->forms.contains(t), "drop", "Invalid time position : the form is not defined at this time");
    d->forms.remove(t);
    if ((d->current_time = t) && (d->forms.size() > 0)) {
        d->current_time = d->forms.keys()[0];
    }
}

template<typename T>
QJsonObject gnomonTimeSeries<T>::serialize(void) {
    QJsonObject out(gnomonAbstractDynamicForm::serialize());
    out["current_time"] = d->current_time;
    QJsonObject forms;
    for(auto& t: times()) {
        forms[QString::number(t)] = d->forms[t]->serialize();
    }
    out["forms"] = forms;
    return out;
}

template<typename T>
void gnomonTimeSeries<T>::deserialize(QJsonObject &serialization) {
    gnomonAbstractDynamicForm::deserialize(serialization);

    d->current_time = serialization["current_time"].toDouble();
    // emptying current forms map and deleting forms
    for(auto& key: d->forms.keys()) {
        delete d->forms[key];
        d->forms[key] = nullptr;
    }
    // filling back
    d->forms.clear();
    QJsonObject forms = serialization["forms"].toObject();
    for(auto& key: forms.keys()) {
        auto formSerialization = forms[key].toObject();
        d->forms[key.toDouble()] = new T(formSerialization);
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
