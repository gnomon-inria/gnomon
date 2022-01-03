#include "gnomonBinaryImageWriterCommand.h"

#include <dtkScript>

class gnomonBinaryImageWriterCommandPrivate
{
public:
    gnomonBinaryImageSeries *binaryImage = nullptr;
};

gnomonBinaryImageWriterCommand::gnomonBinaryImageWriterCommand() : d(new gnomonBinaryImageWriterCommandPrivate)
{
    this->factory_name = "binaryImageWriter";
    loadPluginGroup(this->factoryName());

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

void gnomonBinaryImageWriterCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractBinaryImageWriter *) this->action)->setBinaryImage(d->binaryImage);
    this->action->run();
}

void gnomonBinaryImageWriterCommand::undo()
{
    ((gnomonAbstractBinaryImageWriter *) this->action)->setPath("");
}

void gnomonBinaryImageWriterCommand::setBinaryImage(gnomonBinaryImageSeries *binaryImage)
{
    d->binaryImage = binaryImage;
}

void gnomonBinaryImageWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->binaryImage = dynamic_cast<gnomonBinaryImageSeries *>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["binaryImage"] = d->binaryImage;
    return inputs;
}

bool gnomonBinaryImageWriterCommand::isEmpty()
{
    loadPluginGroup("binaryImageWriter");
    return gnomonCore::binaryImageWriter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return input_types;
}

void gnomonBinaryImageWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "binaryImage") {
        this->setForm(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}
