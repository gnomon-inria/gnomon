#include "gnomonCellComplexWriterCommand.h"

#include <dtkScript>

class gnomonCellComplexWriterCommandPrivate
{
public:
    QString path;
    gnomonCellComplex* cellComplex = nullptr;
};

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand(const QString& key) : d(new gnomonCellComplexWriterCommandPrivate)
{
    QString command = "import gnomonCellComplexWriter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellComplexWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexWriterCommand::~gnomonCellComplexWriterCommand()
{
    delete d;
}

void gnomonCellComplexWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractCellComplexWriter *) this->action)->setCellComplex(d->cellComplex);
    this->action->run();
}

void gnomonCellComplexWriterCommand::undo(void)
{
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath("");
}

void gnomonCellComplexWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonCellComplexWriterCommand::setCellComplex(gnomonCellComplex *cellComplex)
{
    d->cellComplex = cellComplex;
}