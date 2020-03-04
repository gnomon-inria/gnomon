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

class gnomonCellComplexReaderCommandPrivate
{
public:
    QString path;
};

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand(const QString& key) : d(new gnomonCellComplexReaderCommandPrivate)
{
    loadPluginGroup("cellComplexReader");

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
    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexReader *) this->action)->cellComplex();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        return nullptr;
    } else {
        return cellComplex;
    }
}

bool gnomonCellComplexReaderCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexReader");
    return gnomonCore::cellComplexReader::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexReaderCommand.cpp ends here
