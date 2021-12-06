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

#include "gnomonPointCloudReaderCommand.h"

#include <dtkScript>

class gnomonPointCloudReaderCommandPrivate
{
public:
    gnomonPointCloudSeries *pointCloud = nullptr;
};

gnomonPointCloudReaderCommand::gnomonPointCloudReaderCommand(void) : d(new gnomonPointCloudReaderCommandPrivate)
{
    this->factory_name = "pointCloudReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::pointCloudReader::pluginFactory().keys()) {
        auto algo = gnomonCore::pointCloudReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonPointCloudReaderCommand::~gnomonPointCloudReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonPointCloudReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudReader *) this->action)->pointCloud();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        d->pointCloud = nullptr;
    } else {
        d->pointCloud = pointCloud;
    }
}

void gnomonPointCloudReaderCommand::undo(void)
{
    ((gnomonAbstractPointCloudReader *) this->action)->setPath("");
}

void gnomonPointCloudReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonPointCloudSeries *gnomonPointCloudReaderCommand::pointCloud(void)
{
    return d->pointCloud;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["pointCloud"] = this->pointCloud();
    return outputs;
}

bool gnomonPointCloudReaderCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudReader");
    return gnomonCore::pointCloudReader::pluginFactory().keys().size() == 0;
}
//
// gnomonPointCloudReaderCommand.cpp ends here
