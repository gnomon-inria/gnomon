#include "gnomonBinaryImageWriterCommand.h"

#include <dtkScript>

class gnomonBinaryImageWriterCommandPrivate
{
public:
    gnomonBinaryImageSeries *binaryImage = nullptr;
};

gnomonBinaryImageWriterCommand::gnomonBinaryImageWriterCommand(void) : d(new gnomonBinaryImageWriterCommandPrivate)
{
    this->factory_name = "binaryImageWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::binaryImageWriter::pluginFactory().keys();
    if (keys.size() > 0) {
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
    if (this->action)
        delete this->action;
    this->action = gnomonCore::binaryImageWriter::pluginFactory().create(algo_name);
}

void gnomonBinaryImageWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractBinaryImageWriter *) this->action)->setBinaryImage(d->binaryImage);
    this->action->run();
}

void gnomonBinaryImageWriterCommand::undo(void)
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["binaryImage"] = d->binaryImage;
    return inputs;
}

bool gnomonBinaryImageWriterCommand::isEmpty(void)
{
    loadPluginGroup("binaryImageWriter");
    return gnomonCore::binaryImageWriter::pluginFactory().keys().size() == 0;
}