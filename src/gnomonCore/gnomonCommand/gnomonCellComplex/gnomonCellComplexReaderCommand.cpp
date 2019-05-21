#include "gnomonCellComplexReaderCommand.h"

#include <dtkScript>

class gnomonCellComplexReaderCommandPrivate
{
public:
    QString path;
};

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand(const QString& key) : d(new gnomonCellComplexReaderCommandPrivate)
{
    QString command = "import gnomonCellComplexReader";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellComplexReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexReaderCommand::~gnomonCellComplexReaderCommand()
{
    delete d;
}

void gnomonCellComplexReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonCellComplexReaderCommand::undo(void)
{
    ((gnomonAbstractCellComplexReader *) this->action)->setPath("");
}

void gnomonCellComplexReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonCellComplex *gnomonCellComplexReaderCommand::cellComplex(void)
{
    return ((gnomonAbstractCellComplexReader *) this->action)->cellComplex()->current();
}
