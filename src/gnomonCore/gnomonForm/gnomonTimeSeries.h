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

#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>

#include "gnomonAbstractDynamicForm.h"

class gnomonAbstractForm;


//template <typename T, typename Enable = std::enable_if_t<std::is_base_of<gnomonAbstractForm,T>::value>>
template <typename T>
class gnomonTimeSeriesPrivate
{
public:
    QMap<double, T*> forms;
    double current_time;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

//template <typename T, typename Enable = std::enable_if_t<std::is_base_of<gnomonAbstractForm,T>::value>>
template <typename T>
class GNOMONCORE_EXPORT gnomonTimeSeries : public gnomonAbstractDynamicForm<T>
{

public:
     gnomonTimeSeries(void);
     gnomonTimeSeries(const gnomonTimeSeries& o);
    ~gnomonTimeSeries(void);

public:
    gnomonTimeSeries<T>& operator = (const gnomonTimeSeries<T>& o);

public:
    gnomonAbstractDynamicForm<T> *clone(void) const override;

public:
    T *seek(double t) const override;
    T *current(void) const override;
    T *next(void) const override;
    T *prev(void) const override;
    double time(void) const override;
    QList<double> times(void) const override;

    void insert(double t, T *form) override;
//    void insert(const T& form) override;
    void drop(double t) override;

private:
    gnomonTimeSeriesPrivate<T> *d;

};

#include "gnomonTimeSeries.tpp"

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

//DTK_DECLARE_OBJECT        (gnomonTimeSeries<gnomonAbstractForm *> *)
//DTK_DECLARE_PLUGIN        (gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_FACTORY(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

//namespace gnomonCore {
//    DTK_DECLARE_CONCEPT(gnomonTimeSeries<gnomonAbstractForm *>, GNOMONCORE_EXPORT, discreteDynamicForm);
//}

//
// gnomonTimeSeries.h ends here
