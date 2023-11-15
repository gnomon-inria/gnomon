#include "gnomonAbstractReaderCommand.h"

gnomonAbstractReaderCommand::~gnomonAbstractReaderCommand()
{
    for (auto algo : m_actions) {
        delete algo;
    }

    m_actions.clear();
    this->action = nullptr;
}

/*
QMap<QString, gnomonAbstractDynamicForm *> gnomonAbstractReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> empty;
    return empty;
}
*/

void gnomonAbstractReaderCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    this->action = m_actions[algo_name];
}

const QString& gnomonAbstractReaderCommand::path()
{
    return this->m_path;
}

QMap<QString,QString> gnomonAbstractReaderCommand::descriptions() const
{
    return m_descriptions;
}

QMap<QString, QString> gnomonAbstractReaderCommand::preview() const
{
    return m_preview;
}

QMap<QString,QStringList> gnomonAbstractReaderCommand::extensions() const
{
    return m_extensions;
}

QStringList gnomonAbstractReaderCommand::algorithmNames() const
{
    return m_actions.keys();
}

void gnomonAbstractReaderCommand::setPath(const QString &path) {
    this->m_path = path;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonAbstractReaderCommand::inputs() {
    return {};
}

gnomonAbstractCommand::orderedMap gnomonAbstractReaderCommand::inputTypes() {
    return {};
}

void gnomonAbstractReaderCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    dtkWarn() << Q_FUNC_INFO << "Reader takes no input form";
}

const QString &gnomonAbstractReaderCommand::source() {
    return m_source;
}

void gnomonAbstractReaderCommand::setSource(const QString &source) {
    m_source = source;
}
