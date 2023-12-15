#include "gnomonMeshWriterCommand.h"

#include <dtkLog.h>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> mesh = nullptr;
    QMap<QString, QStringList> extensions;
    QMap<QString, gnomonAbstractAlgorithm*> actions;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshWriterCommand::gnomonMeshWriterCommand() : d(new gnomonMeshWriterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonMeshWriterCommand)

    for (const auto& key: gnomonCore::meshWriter::pluginFactory().keys()) {
        auto algo = gnomonCore::meshWriter::pluginFactory().create(key);
        if (!this->action) {
            this->algorithm_name = key;
            this->action = algo;
        }
        d->extensions.insert(key, algo->extensions());
        d->actions.insert(key, algo);
    }
}

gnomonMeshWriterCommand::~gnomonMeshWriterCommand()
{
    for (auto algo : d->actions) {
        delete algo;
    }
    d->actions.clear();

    delete d;
}

void gnomonMeshWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    this->action = d->actions[algo_name];
}

void gnomonMeshWriterCommand::predo(void)
{
    //check extension, and set algo depending on extension!
    QFileInfo f_info(this->m_path);
    QString extension = f_info.completeSuffix().toLower();
    if(!((gnomonAbstractMeshWriter *) this->action)->extensions().contains(extension)) {
        for(auto algo : d->extensions.keys()) {
            if(d->extensions[algo].contains(extension)) {
                dtkInfo() << "Set writer to " << algo << " to write with extension " << extension;
                this->setAlgorithmName(algo);
            }
        }
    }

    ((gnomonAbstractMeshWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractMeshWriter *) this->action)->setMesh(d->mesh);
}

void gnomonMeshWriterCommand::postdo(void)
{

}

void gnomonMeshWriterCommand::undo()
{
    ((gnomonAbstractMeshWriter *) this->action)->setPath("");
}

void gnomonMeshWriterCommand::setMesh(std::shared_ptr<gnomonMeshSeries> mesh)
{
    d->mesh = mesh;
}

void gnomonMeshWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["mesh"] = d->mesh;
    return inputs;
}

QStringList gnomonMeshWriterCommand::extensions(void)
{
    QStringList res;
    for(auto ext : d->extensions)
        res += ext;
    return res;
}

gnomonAbstractCommand::orderedMap gnomonMeshWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    return input_types;
}

void gnomonMeshWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "mesh") {
        this->setMesh(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

GNOMON_REGISTER_TYPE(gnomonMeshWriterCommand)

//
// gnomonMeshWriterCommand.cpp ends here
