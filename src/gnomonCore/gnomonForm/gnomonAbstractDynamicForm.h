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
#include "gnomonDynamicFormMetadata.h"

//#include <gnomonTime.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////
class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
             gnomonAbstractDynamicForm(void) = default;
    virtual ~gnomonAbstractDynamicForm(void) {};

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
    virtual QJsonObject serialize(void) {
        QJsonObject json;
        json["metadata"] = metadata.serialize();
        return json;
    }

    virtual void deserialize(QJsonObject & json) {
        metadata.deserialize(json);
    }

    gnomonDynamicFormMetadata& getMetadata(void) {
        return metadata;
    }

protected:
    gnomonDynamicFormMetadata metadata;

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
