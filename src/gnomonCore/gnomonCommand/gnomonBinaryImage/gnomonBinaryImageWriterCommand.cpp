#include "gnomonBinaryImageWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageWriter.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonBinaryImageWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage = nullptr;
};

gnomonBinaryImageWriterCommand::gnomonBinaryImageWriterCommand() : d(new gnomonBinaryImageWriterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonBinaryImageWriterCommand)

    //see MeshWriterCommand for what to do when there is multiple writers
    QStringList keys = gnomonCore::binaryImageWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::binaryImageWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonBinaryImageWriterCommand::~gnomonBinaryImageWriterCommand()
{
    delete d;
}

void gnomonBinaryImageWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::binaryImageWriter::pluginFactory().create(algo_name);
}

void gnomonBinaryImageWriterCommand::predo(void)
{
    ((gnomonAbstractBinaryImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractBinaryImageWriter *) this->action)->setBinaryImage(d->binaryImage);
}

void gnomonBinaryImageWriterCommand::postdo(void)
{

}

void gnomonBinaryImageWriterCommand::undo()
{
    ((gnomonAbstractBinaryImageWriter *) this->action)->setPath("");
}

void gnomonBinaryImageWriterCommand::setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage)
{
    d->binaryImage = binaryImage;
}

void gnomonBinaryImageWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->binaryImage = std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> gnomonBinaryImageWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> inputs;
    inputs["binaryImage"] = d->binaryImage;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return input_types;
}

void gnomonBinaryImageWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "binaryImage") {
        this->setForm(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

GNOMON_REGISTER_TYPE(gnomonBinaryImageWriterCommand)
