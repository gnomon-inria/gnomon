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

#include "gnomonCellImageReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageReaderCommandPrivate
{
public:
    gnomonCellImageSeries *cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageReaderCommand::gnomonCellImageReaderCommand() : d(new gnomonCellImageReaderCommandPrivate)
{
    this->factory_name = "cellImageReader";
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::cellImageReader::pluginFactory().keys()) {
        auto algo = gnomonCore::cellImageReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonCellImageReaderCommand::~gnomonCellImageReaderCommand()
{
    this->action = nullptr;

    delete d;
}

void gnomonCellImageReaderCommand::predo(void)
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath(this->m_path);
}

void gnomonCellImageReaderCommand::postdo(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageReader *) this->action)->cellImage();

    if ((!cellImage)||(cellImage->times().empty())) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
}

void gnomonCellImageReaderCommand::undo()
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

gnomonCellImageSeries *gnomonCellImageReaderCommand::cellImage()
{
    return d->cellImage;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    return outputs;
}

bool gnomonCellImageReaderCommand::isEmpty()
{
    loadPluginGroup("cellImageReader");
    return gnomonCore::cellImageReader::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellImageReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return types;
}

//
// gnomonCellImageReaderCommand.cpp ends here
