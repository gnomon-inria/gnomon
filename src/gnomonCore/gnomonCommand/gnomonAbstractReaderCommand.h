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
#include "gnomonAbstractCommand.h"

#include <QtCore>

class gnomonAbstractReaderCommand : public gnomonAbstractCommand
{
public:
    ~gnomonAbstractReaderCommand(void);

public:
    QMap<QString, QString> descriptions(void) const;
    QMap<QString, QStringList>  extensions(void) const;
    QStringList algorithmNames(void) const;

public:
    void setAlgorithmName(const QString& algo_name) override;

public:
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;
    virtual const QString& path(void);

protected:
    QString m_path = "";
    QMap<QString, QString> m_descriptions;
    QMap<QString, QStringList> m_extensions;
    QMap<QString, gnomonAbstractAlgorithm*> m_actions;
};

//
// gnomonAbstractReaderCommand.h ends here
