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

gnomonCellImageReaderCommand::gnomonCellImageReaderCommand(void) : d(new gnomonCellImageReaderCommandPrivate)
{
    this->factory_name = "cellImageReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::cellImageReader::pluginFactory().keys()) {
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

void gnomonCellImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageReader *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
}

void gnomonCellImageReaderCommand::undo(void)
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

void gnomonCellImageReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonCellImageSeries *gnomonCellImageReaderCommand::cellImage(void)
{
    return d->cellImage;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    return outputs;
}

bool gnomonCellImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("cellImageReader");
    return gnomonCore::cellImageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageReaderCommand.cpp ends here
