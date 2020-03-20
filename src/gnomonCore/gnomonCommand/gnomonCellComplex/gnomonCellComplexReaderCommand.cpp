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
    gnomonCellComplexSeries *cellComplex = nullptr;
};

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand(const QString& key) : d(new gnomonCellComplexReaderCommandPrivate)
{
    this->factory_name = "cellComplexReader";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
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

    ((gnomonAbstractCellComplexReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexReader *) this->action)->cellComplex();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        d->cellComplex = nullptr;
    } else {
        d->cellComplex = cellComplex;
    }
}

void gnomonCellComplexReaderCommand::undo(void)
{
    ((gnomonAbstractCellComplexReader *) this->action)->setPath("");
}

void gnomonCellComplexReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonCellComplexSeries *gnomonCellComplexReaderCommand::cellComplex(void)
{
    return d->cellComplex;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellComplex"] = this->cellComplex();
    return outputs;
}

bool gnomonCellComplexReaderCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexReader");
    return gnomonCore::cellComplexReader::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexReaderCommand.cpp ends here
