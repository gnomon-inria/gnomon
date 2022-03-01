#include "gnomonTimeSeries.h"
#include "gnomonTime.h"

#include <QtGlobal>


template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(void): gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{

}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : gnomonAbstractDynamicForm(new gnomonTimeSeriesPrivate<T>())
{
    for (const auto& time : dynamic_cast<gnomonTimeSeriesPrivate<T>*>(o.d)->forms.keys()) {
        dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[time] = dynamic_cast<T *>(dynamic_cast<gnomonTimeSeriesPrivate<T>*>(o.d)->forms[time]->clone());
    }
    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(o.d)->current_time;
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

    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(o.d)->forms;
    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(o.d)->current_time;

    return (*this);
}

template <typename T> T *gnomonTimeSeries<T>::at(double t) const
{
    Q_ASSERT_X(dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.contains(t), "at", "Invalid time position : the form is not defined at this time");

    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = t;
    return this->current();
}

template <typename T> T *gnomonTimeSeries<T>::current(void) const
{
    return dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time];
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.keys();
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, T* form)
{
    //Q_ASSERT_X(dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.contains(t), "insert", "Invalid time position : the form is already defined at this time");
    qDebug() << Q_FUNC_INFO << dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms;
    qDebug() << Q_FUNC_INFO << dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.size();
    if (dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.size() == 0) {
        dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = t;
    }
    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.insert(t, form);
}

//void gnomonTimeSeries::insert(const gnomonTimeSeries& dynamic_form)
//{
//    for(auto it = dynamic_form.dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.begin(); it != dynamic_form.dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.end(); ++it) { // Iterates on the times
//        if(dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.contains(it.key())) { // The time already exists
//            for(auto jt = it->begin(); jt != it->end(); ++jt) { // Iterates on the forms of a given time
//                auto forms = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[it.key()];
//                if(!forms.contains(jt.key())) { // The form doesn't exist at the given time
//                    forms.insert(jt.key(), jt.value()); // Insert the form at the given time
//                } else { // The form already exists at the given time
//                    dtkError() << "Invalid form : the form is already defined at this time";
//                }
//            }
//        } else { // The time doesn't exist yet
//            dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.insert(it.key(), it.value());  // Insert the time and the form
//        }
//    }
//}

template <typename T> void gnomonTimeSeries<T>::drop(const double t)
{
    Q_ASSERT_X(dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.contains(t), "drop", "Invalid time position : the form is not defined at this time");
    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.remove(t);
    if ((dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = t) && (dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.size() > 0)) {
        dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.keys()[0];
    }
}

template<typename T>
QJsonObject gnomonTimeSeries<T>::serialize(void) {
    QJsonObject out(gnomonAbstractDynamicForm::serialize());
    out["current_time"] = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time;
    QJsonObject forms;
    for(auto& t: times()) {
        forms[QString::number(t)] = dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[t]->serialize();
    }
    out["forms"] = forms;
    return out;
}

template<typename T>
void gnomonTimeSeries<T>::deserialize(QJsonObject &serialization) {
    gnomonAbstractDynamicForm::deserialize(serialization);

    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->current_time = serialization["current_time"].toDouble();
    // emptying current forms map and deleting forms
    for(auto& key: dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.keys()) {
        delete dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[key];
        dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[key] = nullptr;
    }
    // filling back
    dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms.clear();
    QJsonObject forms = serialization["forms"].toObject();
    for(auto& key: forms.keys()) {
        auto formSerialization = forms[key].toObject();
        dynamic_cast<gnomonTimeSeriesPrivate<T>*>(d)->forms[key.toDouble()] = new T(formSerialization);
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
