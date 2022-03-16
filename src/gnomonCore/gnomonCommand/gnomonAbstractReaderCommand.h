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

#include <QtCore>

class gnomonAbstractDynamicForm;

class GNOMONCORE_EXPORT gnomonAbstractReaderCommand : public gnomonAbstractCommand
{
public:
    ~gnomonAbstractReaderCommand() override;

public:
    QMap<QString, QString> descriptions() const;
    QMap<QString, QStringList>  extensions() const;
    QStringList algorithmNames() const;

public:
    void setAlgorithmName(const QString& algo_name) override;

public:
    const QString& path();
    void setPath(const QString& path);

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

protected:
    QString m_path = "";
    QMap<QString, QString> m_descriptions;
    QMap<QString, QStringList> m_extensions;
    QMap<QString, gnomonAbstractAlgorithm*> m_actions;
};

//
// gnomonAbstractReaderCommand.h ends here
