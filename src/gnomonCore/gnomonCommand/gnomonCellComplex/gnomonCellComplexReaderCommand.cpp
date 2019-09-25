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

#include "gnomonCellComplexReaderCommand.h"

#include <dtkScript>

class gnomonCellComplexReaderCommandPrivate
{
public:
    QString path;
};

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand(const QString& key) : d(new gnomonCellComplexReaderCommandPrivate)
{
    loadPluginGroup("gnomonCellComplexReader");

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

gnomonCellComplexSeries *gnomonCellComplexReaderCommand::cellComplex(void)
{
    return ((gnomonAbstractCellComplexReader *) this->action)->cellComplex();
}

//
// gnomonCellComplexReaderCommand.cpp ends here
