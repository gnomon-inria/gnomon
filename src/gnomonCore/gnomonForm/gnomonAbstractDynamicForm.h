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

//#include <gnomonTime.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractForm;

// template <typename T, typename Enable = std::enable_if_t<std::is_base_of<gnomonAbstractForm,T>::value>>
template <typename T>
class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void) = default;
    virtual ~gnomonAbstractDynamicForm(void) {};

public:
    virtual gnomonAbstractDynamicForm *clone(void) const = 0;

public:
    virtual T *seek(double t) const = 0;
    virtual T *current(void) const = 0;
    virtual T *next(void) const = 0;
    virtual T *prev(void) const = 0;
    virtual double time(void) const = 0;
    virtual QList<double> times(void) const = 0;

    virtual void insert(double t, T *form) = 0;
//    virtual void insert(const T& form) = 0;
    virtual void drop(double t) = 0;

//public:
//    virtual void setInitialTime(gnomonTime T_i) = 0;
//    virtual gnomonAbstractForm* atTime(gnomonTime t) = 0;
//
//    virtual void insert(gnomonAbstractForm* form, gnomonTime t) = 0;
//    virtual void drop(gnomonTime t) = 0;
//
//    virtual QList<gnomonTime> availableTimes(void) = 0;
};

//typedef QSharedPointer<gnomonAbstractDynamicForm> gnomonAbstractDynamicFormPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

//DTK_DECLARE_OBJECT        (gnomonAbstractDynamicForm *)
// DTK_DECLARE_PLUGIN        (gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)
// DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

// namespace gnomonCore {
//     DTK_DECLARE_CONCEPT(gnomonAbstractDynamicForm, GNOMONCORE_EXPORT, dynamicForm);
// }

//
// gnomonAbstractDynamicForm.h ends here
