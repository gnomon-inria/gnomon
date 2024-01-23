#include "gnomonImageWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageWriterCommand::gnomonImageWriterCommand() : d(new gnomonImageWriterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonImageWriterCommand)

        //see MeshWriterCommand for what to do when there is multiple writers
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

void gnomonImageWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->image = std::dynamic_pointer_cast<gnomonImageSeries>(form);
}

void gnomonImageWriterCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    d->image = image;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = d->image;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    return input_types;
}

void gnomonImageWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonImageWriterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->image = nullptr;
}

GNOMON_REGISTER_TYPE(gnomonImageWriterCommand)

//
// gnomonImageWriterCommand.cpp ends here
