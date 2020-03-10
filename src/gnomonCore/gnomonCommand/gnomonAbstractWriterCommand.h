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

protected:
    QString m_path = "";
};


//
// gnomonAbstractWriterCommand.h ends here
