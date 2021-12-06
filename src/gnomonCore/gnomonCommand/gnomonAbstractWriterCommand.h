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
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual const QString& path(void) {return this->m_path;}
    virtual void setPath(const QString & path) { m_path = path; };
    virtual void setForm(gnomonAbstractDynamicForm* form) = 0;

protected:
    QString m_path = "";
};


//
// gnomonAbstractWriterCommand.h ends here
