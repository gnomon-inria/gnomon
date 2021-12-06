#include "gnomonAbstractReaderCommand.h"

gnomonAbstractReaderCommand::~gnomonAbstractReaderCommand(void)
{
    for (auto algo: m_actions.values())
        delete algo;

    m_actions.clear();
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonAbstractReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> empty;
    return empty;
}

void gnomonAbstractReaderCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    this->action = m_actions[algo_name];
}

const QString& gnomonAbstractReaderCommand::path(void)
{
    return this->m_path;
}

QMap<QString,QString> gnomonAbstractReaderCommand::descriptions(void) const
{
    return m_descriptions;
}

QMap<QString,QStringList> gnomonAbstractReaderCommand::extensions(void) const
{
    return m_extensions;
}

QStringList gnomonAbstractReaderCommand::algorithmNames(void) const
{
    return m_actions.keys();
}
