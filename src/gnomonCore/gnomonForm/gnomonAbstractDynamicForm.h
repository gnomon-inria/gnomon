#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>
#include "gnomonAbstractDynamicForm_p.h"

//#include <gnomonTime.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void): d(new gnomonAbstractDynamicFormPrivate) {

             }
    virtual ~gnomonAbstractDynamicForm(void) {
                 delete d;
             };

public:
    virtual gnomonAbstractDynamicForm *clone(void) const = 0;

public:
    virtual gnomonAbstractForm *at(double t) const = 0;
    virtual gnomonAbstractForm *current(void) const = 0;
    virtual double time(void) const = 0;
    virtual QList<double> times(void) const = 0;

//    virtual void insert(double t, T *form) = 0;
//    virtual void insert(const T& form) = 0;
    virtual void drop(double t) = 0;

    virtual QString formName(void) = 0;

    virtual QJsonObject serialize(void) {
        QJsonObject json;
        json["metadata"] = d->metadata->serialize();
        return json;
    }

    virtual void deserialize(QJsonObject & json) {
        d->metadata->deserialize(json);
    }

    gnomonDynamicFormMetadata* metadata(void) {
        return d->metadata;
    }

    void setMetadata(gnomonDynamicFormMetadata* metadata) {
        delete d->metadata;
        d->metadata = metadata;
    }

protected:
    // for subclassing, see https://wiki.qt.io/D-Pointer#Inheriting_d-pointers_for_optimization
    explicit gnomonAbstractDynamicForm(gnomonAbstractDynamicFormPrivate* otherPrivate): d(otherPrivate) {}

    gnomonAbstractDynamicFormPrivate* d;

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
