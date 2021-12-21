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

#include <gnomonCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonAbstractCommand;

class gnomonAbstractWriterCommand : public gnomonAbstractCommand
{

public:
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs() {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual const QString& path() {return this->m_path;}
    virtual void setPath(const QString & path) { m_path = path; };
    virtual void setForm(gnomonAbstractDynamicForm* form) = 0;

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override {
        return {};
    }

    orderedMap outputTypes() override {
        return {};
    }

protected:
    QString m_path = "";
};


//
// gnomonAbstractWriterCommand.h ends here
