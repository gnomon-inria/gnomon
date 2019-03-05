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

#include "gnomonAbstractLStringData.h"
#include "gnomonForm/gnomonAbstractForm.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonLString : public gnomonAbstractForm
{
protected:
    gnomonAbstractLStringData *m_data;

public:
    explicit gnomonLString(void) : m_data(nullptr) {}
    explicit gnomonLString(gnomonAbstractLStringData *data) : m_data(data) {}
    gnomonLString(const gnomonLString& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonLString(*this); };

    ~gnomonLString(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonLString& operator = (const gnomonLString& o)
    {
        if (m_data != o.m_data) {
            if (m_data != nullptr) {
                delete m_data;
            }
            if(o.m_data != nullptr) {
                m_data = o.m_data->clone();
            } else {
                m_data = nullptr;
            }
        }
        return *this;
    }

public:
    QString name(void) const override { return"gnomonLString"; }
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }

public:
    const gnomonAbstractLStringData *data(void) const { return m_data; }
          gnomonAbstractLStringData *data(void)       { return m_data; }

    void setData(gnomonAbstractLStringData* data) { m_data = data; }


public:
    void fromString(const QString& lstring) const { return m_data->fromString(lstring); };
    QString toString(void) const { return m_data->toString(); };
    bool isEmpty(void) const { return m_data->isEmpty(); };
    bool isValid(void) const { return m_data->isValid(); };

public:
    QList<long> moduleIds(void) const { return m_data->moduleIds(); };
    long moduleCount(void) const { return m_data->moduleCount(); };
    bool hasModule(long moduleId) const { return m_data->hasModule(moduleId); };
    bool hasModule(const QString& module_name) const { return m_data->hasModule(module_name); };

    QString moduleName(long moduleId) const { return m_data->moduleName(moduleId); };
    QList<QVariant> moduleVariables(long moduleId) const { return m_data->moduleVariables(moduleId); };
    QString moduleVariableString(long moduleId) const { return m_data->moduleVariableString(moduleId); };

    QList<long> rootIds(void) const { return m_data->rootIds(); };

public:
    bool hasChildren(long moduleId) const { return m_data->hasChildren(moduleId); };
    QList<long> childrenIds(long moduleId) const { return m_data->childrenIds(moduleId); };
    long childrenCount(long moduleId) const { return m_data->childrenCount(moduleId); };

    bool hasParent(long moduleId) const { return m_data->hasParent(moduleId); };
    long parentId(long moduleId) const { return m_data->parentId(moduleId); };

public:
    bool hasSuccessor(long moduleId) const { return m_data->hasSuccessor(moduleId); };
    long successorId(long moduleId) const { return m_data->successorId(moduleId); };

    bool hasLaterals(long moduleId) const { return m_data->hasLaterals(moduleId); };
    QList<long> lateralIds(long moduleId) const { return m_data->lateralIds(moduleId); };
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonLString *)

//
// gnomonLString.h ends here
