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

gnomonDataFrameReaderCommand::gnomonDataFrameReaderCommand(void) : d(new gnomonDataFrameReaderCommandPrivate)
{
    this->factory_name = "dataFrameReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::dataFrameReader::pluginFactory().keys()) {
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

void gnomonDataFrameReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractDataFrameReader *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        d->dataFrame = nullptr;
    } else {
        d->dataFrame = dataFrame;
    }
}

void gnomonDataFrameReaderCommand::undo(void)
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath("");
}

void gnomonDataFrameReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonDataFrameSeries *gnomonDataFrameReaderCommand::dataFrame(void)
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataFrameReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonDataFrameReaderCommand::isEmpty(void)
{
    loadPluginGroup("dataFrameReader");
    return gnomonCore::dataFrameReader::pluginFactory().keys().size() == 0;
}

//
// gnomonDataFrameReaderCommand.cpp ends here
