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

class gnomonAbstractAlgorithmCommand : public gnomonAbstractCommand
{

public:
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const {QMap<QString, gnomonCoreParameter *> empty; return empty;};;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) {QMap<QString, gnomonAbstractDynamicForm *> empty; return empty;};
    virtual const QString& path(void) {return this->m_path;}

protected:
    QString m_path = "";
};


//
// gnomonAbstractAlgorithmCommand.h ends here
