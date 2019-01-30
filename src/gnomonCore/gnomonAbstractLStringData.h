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

class dtkImage;

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractLStringData
{
public:
             gnomonAbstractLStringData(void) = default;
    virtual ~gnomonAbstractLStringData(void) = default;

    virtual gnomonAbstractLStringData* clone(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QVariantMap metadata(void) const = 0;


public:
    virtual void fromString(const QString&) const = 0;
    virtual QString toString(void) const = 0;
    virtual bool isValid(void) const = 0;
    virtual bool isEmpty(void) const = 0;

public:
    virtual QList<long> moduleIds(void) const = 0;
    virtual long moduleCount(void) const = 0;
    virtual bool hasModule(const QString&) const = 0;
    virtual bool hasModule(long moduleId) const = 0;

    virtual QString moduleName(long moduleId) const = 0;
    virtual QList<QVariant> moduleVariables(long moduleId) const = 0;
    virtual QString moduleVariableString(long moduleId) const = 0;

    virtual QList<long> rootIds(void) const = 0;

public:
    virtual bool hasChildren(long moduleId) const = 0;
    virtual QList<long> childrenIds(long moduleId) const = 0;
    virtual long childrenCount(long moduleId) const = 0;

    virtual bool hasParent(long moduleId) const = 0;
    virtual long parentId(long moduleId) const = 0;

public:
    virtual bool hasSuccessor(long moduleId) const = 0;
    virtual long successorId(long moduleId) const = 0;

    virtual bool hasLaterals(long moduleId) const = 0;
    virtual QList<long> lateralIds(long moduleId) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractLStringData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractLStringData, GNOMONCORE_EXPORT, lStringData);
}

// gnomonAbstractLStringData.h ends here
