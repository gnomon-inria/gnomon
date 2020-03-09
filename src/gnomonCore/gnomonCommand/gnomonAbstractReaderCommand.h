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

class gnomonAbstractReaderCommand : public gnomonAbstractCommand
{

public:
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual const QString& path(void) {return this->m_path;}

protected:
    QString m_path = "";
};


//
// gnomonAbstractReaderCommand.h ends here
