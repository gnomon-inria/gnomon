#include "gnomonDataFrameReaderCommand.h"

#include <dtkScript>

class gnomonDataFrameReaderCommandPrivate
{
public:
    QString path;
};

gnomonDataFrameReaderCommand::gnomonDataFrameReaderCommand(const QString& key) : d(new gnomonDataFrameReaderCommandPrivate)
{
    QString command = "import gnomonDataFrameReader";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::dataFrameReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonDataFrameReaderCommand::~gnomonDataFrameReaderCommand()
{
    delete d;
}

void gnomonDataFrameReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonDataFrameReaderCommand::undo(void)
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath("");
}

void gnomonDataFrameReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonDataFrame *gnomonDataFrameReaderCommand::dataFrame(void)
{
    return ((gnomonAbstractDataFrameReader *) this->action)->dataFrame()->current();
}
