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

#include <gnomonCoreExport>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm.h>
#include "gnomonAbstractCommand.h"

class gnomonAbstractDynamicForm;

class GNOMONCORE_EXPORT  gnomonAbstractWriterCommand : public gnomonAbstractCommand
{

public:
    virtual const QString& path() {return this->m_path;}
    virtual void setPath(const QString & path) { m_path = path; };
    virtual void setForm(gnomonAbstractDynamicForm* form) = 0;

public:
    virtual QStringList extensions(void);

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override {
        return {};
    }

    orderedMap outputTypes() override {
        return {};
    }

    void deserializeResults(QJsonObject &serialization) override {

    }

    QJsonObject serializeResults(void) override {
        return QJsonObject();
    }

protected:
    QString m_path = "";
};


//
// gnomonAbstractWriterCommand.h ends here
