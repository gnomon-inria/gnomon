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

#include "gnomonImageWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageWriterCommandPrivate
{
public:
    gnomonImageSeries* image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageWriterCommand::gnomonImageWriterCommand(void) : d(new gnomonImageWriterCommandPrivate)
{
    this->factory_name = "imageWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageWriterCommand::~gnomonImageWriterCommand()
{
    delete d;
}

void gnomonImageWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::imageWriter::pluginFactory().create(algo_name);
}

void gnomonImageWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractImageWriter *) this->action)->setImage(d->image);
    this->action->run();
}

void gnomonImageWriterCommand::undo(void)
{
    ((gnomonAbstractImageWriter *) this->action)->setPath("");
}

void gnomonImageWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->image = dynamic_cast<gnomonImageSeries*>(form);
}

void gnomonImageWriterCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    return inputs;
}

bool gnomonImageWriterCommand::isEmpty(void)
{
    loadPluginGroup("imageWriter");
    return gnomonCore::imageWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonImageWriterCommand.cpp ends here
