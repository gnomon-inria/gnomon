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

#include "gnomonDataFrameReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameReaderCommandPrivate
{
public:
    gnomonDataFrameSeries *dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameReaderCommand::gnomonDataFrameReaderCommand() : d(new gnomonDataFrameReaderCommandPrivate)
{
    this->factory_name = "dataFrameReader";
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::dataFrameReader::pluginFactory().keys()) {
        auto algo = gnomonCore::dataFrameReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonDataFrameReaderCommand::~gnomonDataFrameReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonDataFrameReaderCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractDataFrameReader *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().empty())) {
        d->dataFrame = nullptr;
    } else {
        d->dataFrame = dataFrame;
    }
}

void gnomonDataFrameReaderCommand::undo()
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath("");
}

gnomonDataFrameSeries *gnomonDataFrameReaderCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataFrameReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonDataFrameReaderCommand::isEmpty()
{
    loadPluginGroup("dataFrameReader");
    return gnomonCore::dataFrameReader::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonDataFrameReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return types;
}

//
// gnomonDataFrameReaderCommand.cpp ends here
