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

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand() : d(new gnomonCellComplexReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

     for (const auto& key: gnomonCore::cellComplexReader::pluginFactory().keys()) {
        auto algo = gnomonCore::cellComplexReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonCellComplexReaderCommand::~gnomonCellComplexReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonCellComplexReaderCommand::predo(void)
{
    ((gnomonAbstractCellComplexReader *) this->action)->setPath(this->m_path);
}

void gnomonCellComplexReaderCommand::postdo(void)
{
    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexReader *) this->action)->cellComplex();

    if((!cellComplex)||(cellComplex->times().empty())) {
        d->cellComplex = nullptr;
    } else {
        d->cellComplex = cellComplex;
    }
}

void gnomonCellComplexReaderCommand::undo()
{
    ((gnomonAbstractCellComplexReader *) this->action)->setPath("");
}

gnomonCellComplexSeries *gnomonCellComplexReaderCommand::cellComplex()
{
    return d->cellComplex;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellComplex"] = this->cellComplex();
    return outputs;
}

bool gnomonCellComplexReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellComplex", "gnomonCellComplex"));
    return types;
}

QStringList gnomonCellComplexReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

//
// gnomonCellComplexReaderCommand.cpp ends here
