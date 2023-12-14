#include "gnomonCellImageWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageWriterCommand::gnomonCellImageWriterCommand() : d(new gnomonCellImageWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    //see MeshWriterCommand for what to do when there is multiple writers

    QStringList keys = gnomonCore::cellImageWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageWriterCommand::~gnomonCellImageWriterCommand()
{
    delete d;
}

void gnomonCellImageWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageWriter::pluginFactory().create(algo_name);
}

void gnomonCellImageWriterCommand::predo(void)
{
    ((gnomonAbstractCellImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractCellImageWriter *) this->action)->setCellImage(d->cellImage);
}

void gnomonCellImageWriterCommand::postdo(void)
{

}

void gnomonCellImageWriterCommand::undo()
{
    ((gnomonAbstractCellImageWriter *) this->action)->setPath("");
}

void gnomonCellImageWriterCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    d->cellImage = cellImage;
}

void gnomonCellImageWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["cellImage"] = d->cellImage;
    return inputs;
}

bool gnomonCellImageWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonCellImageWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonCellImageWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

void gnomonCellImageWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

GNOMON_REGISTER_TYPE(gnomonCellImageWriterCommand)
//
// gnomonCellImageWriterCommand.cpp ends here
