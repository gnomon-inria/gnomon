#include "gnomonImageWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

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

gnomonImageWriterCommand::gnomonImageWriterCommand() : d(new gnomonImageWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageWriter::pluginFactory().keys();
    if (!keys.empty()) {
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

        delete this->action;
    this->action = gnomonCore::imageWriter::pluginFactory().create(algo_name);
}

void gnomonImageWriterCommand::predo(void)
{
    ((gnomonAbstractImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractImageWriter *) this->action)->setImage(d->image);
}

void gnomonImageWriterCommand::postdo(void)
{

}

void gnomonImageWriterCommand::undo()
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    return inputs;
}

bool gnomonImageWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonImageWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonImageWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    return input_types;
}

void gnomonImageWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "image") {
        this->setImage(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonImageWriterCommand.cpp ends here
