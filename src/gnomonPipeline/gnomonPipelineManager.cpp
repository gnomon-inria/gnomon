#include "gnomonPipelineManager.h"

#include "gnomonPipeline.h"
#include "gnomonPipelineNode.h"
#include "gnomonPipelineEdge.h"
#include "gnomonPipelinePort.h"

#include "gnomonPipelineNodeAdapter.h"
#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeConstructor.h"
#include "gnomonPipelineNodeEvolutionModel.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeTask.h"
#include "gnomonPipelineNodeWriter.h"
#include "gnomonPipelineNodeMorphonet.h"

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractEvolutionModelCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonFormAlgorithmCommand>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand>

#include <gnomonProject>

#include <dtkCore>




// /////////////////////////////////////////////////////////////////
// gnomonPipelineManagerPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineManagerPrivate
{
public:
    gnomonPipelineManager *q;

    gnomonPipeline *pipeline;

    QMap<QString, gnomonPipelineNode *> pipeline_nodes;

    QMap<QString , gnomonPipelineNodeReader *> reader_nodes;
    QMap<QString , QString> reader_output;
    QMap<QString , gnomonPipelineNodeWriter *> writer_nodes;
    QMap<QString , gnomonPipelineNodeAdapter *> adapter_nodes;
    QMap<QString , QString> adapter_output;
    QMap<QString , gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<QString , QString> algorithm_output;
    QMap<QString , gnomonPipelineNodeTask *> task_nodes;
    QMap<QString , QString> task_output;
    QMap<QString , gnomonPipelineNodeMorphonet *> morphonet_nodes;
    QMap<QString , QString> morphonet_output;
    QMap<QString , gnomonPipelineNodeConstructor *> constructor_nodes;
    QMap<QString , QString> constructor_output;
    QMap<QString , gnomonPipelineNodeEvolutionModel *> evolution_model_nodes;
    QMap<QString , QString> evolution_model_output;

    QMap<gnomonPipelineNode *, QMap<QString, QString > > node_input_forms;
    //QMap<QString , QString > form_clones; //TODO check what is it used for

public:
    void linkNodeInputs(gnomonPipelineNode *node);
    QVariantMap parameterVariantValues(const dtkCoreParameters&parameters);
    QJsonObject parameterJson(const dtkCoreParameters& parameters);
    QMap<QString, QString> commandFormUuids(gnomonAbstractCommand *command, const QString& type);
    QVariantMap getNodeSpecificData(const QString& name, QString& node_id);

public:
    bool hasNode(gnomonPipelineNode *);
};


void gnomonPipelineManagerPrivate::linkNodeInputs(gnomonPipelineNode *node)
{
    QMap<QString, QString> input_forms = this->node_input_forms[node];
    for (auto it = input_forms.begin(); it != input_forms.end(); ++it) {
        auto&& input = it.key();
        QString input_form = it.value();
        if (!input_form.isEmpty()) {
            //while (this->form_clones.contains(input_form) & !this->reader_nodes.contains(input_form)) {
            //    input_form = this->form_clones[input_form];
            //}
            gnomonPipelineEdge *edge = nullptr;
            if (this->reader_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->reader_nodes[input_form]->outputPorts()[this->reader_output[input_form]]);
            } else if (this->constructor_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->constructor_nodes[input_form]->outputPorts()[this->constructor_output[input_form]]);
            } else if (this->adapter_nodes.contains(input_form)) {
                q->addAdaptedForm(input_form);
                edge = new gnomonPipelineEdge();
                edge->setSource(this->adapter_nodes[input_form]->outputPorts()[this->adapter_output[input_form]]);
            } else if (this->algorithm_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->algorithm_nodes[input_form]->outputPorts()[this->algorithm_output[input_form]]);
            }  else if (this->evolution_model_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->evolution_model_nodes[input_form]->outputPorts()[this->evolution_model_output[input_form]]);
            } else if (this->task_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->task_nodes[input_form]->outputPorts()[this->task_output[input_form]]);
            } else if (this->morphonet_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->morphonet_nodes[input_form]->outputPorts()[this->morphonet_output[input_form]]);
            }
            if (edge) {
                if (gnomonPipelineNodeWriter *writer_node = dynamic_cast<gnomonPipelineNodeWriter *>(node)) {
                    edge->setTarget(writer_node->inputPorts()[input]);
                } else if (gnomonPipelineNodeAdapter *adapter_node = dynamic_cast<gnomonPipelineNodeAdapter *>(node)) {
                    edge->setTarget(adapter_node->inputPorts()[input]);
                } else if (gnomonPipelineNodeAlgorithm *algorithm_node = dynamic_cast<gnomonPipelineNodeAlgorithm *>(node)) {
                    edge->setTarget(algorithm_node->inputPorts()[input]);
                }else if (gnomonPipelineNodeTask *task_node = dynamic_cast<gnomonPipelineNodeTask *>(node)) {
                    edge->setTarget(task_node->inputPorts()[input]);
                }
                edge->link();
                edge->setFormIndex(GNOMON_SESSION->getForm(it.value())->thumbnailId());
            }
        }
    }
}

QVariantMap gnomonPipelineManagerPrivate::parameterVariantValues(const dtkCoreParameters& parameters)
{
    QVariantMap parameter_values;
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        auto&& parameter_name = it.key();
        auto&& param = it.value();
        QVariantHash param_hash = param->toVariantHash();
        if (param_hash.contains("value")) {
            parameter_values[parameter_name] = param_hash["value"];
        } else if ((param_hash.contains("values"))&& (param_hash.contains("index"))) {
            parameter_values[parameter_name] = param_hash["values"].toStringList()[param_hash["index"].toInt()];
        }
    }

    return parameter_values;
}

QJsonObject gnomonPipelineManagerPrivate::parameterJson(const dtkCoreParameters& parameters)
{
    QJsonObject parameter_json;
    for (auto param_name : parameters.keys()){
        QVariantHash param_value = parameters[param_name]->toVariantHash();
        parameter_json.insert(param_name, QJsonObject::fromVariantHash(param_value));
    }
    return parameter_json;
}

QMap<QString, QString> gnomonPipelineManagerPrivate::commandFormUuids(gnomonAbstractCommand *command, const QString& type)
{
    QMap<QString, QString> form_uuids;
    if (command) {
        QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;
        if (type == "input") {
            forms = command->inputs();
        } else if (type == "output") {
            forms = command->outputs();
        }

        for (const auto &form_name: forms.keys()) {
            auto form = forms[form_name];
            if (form) {
                form_uuids[form_name] = form->uuid();
            } else {
                form_uuids[form_name] = "";
            }
        }
    }

    return form_uuids;
}

QVariantMap gnomonPipelineManagerPrivate::getNodeSpecificData(const QString& name, QString& node_id)
{
    auto convertToQVariantMap = [](const QMap<QString, QString>& input_map)->QVariantMap {
        QVariantMap convertedMap;
        for(const auto& [key, value]: input_map.asKeyValueRange())
            convertedMap[key] = QVariant(value);
        return convertedMap;
    };
    gnomonPipelineNode* node = this->pipeline->node(name);
    QVariantMap component;
    QString output;
    QVariantMap input_forms;
    component["node_name"] = name;
    component["node_type"] = node->type();
    switch (node->type()) {
        case gnomonPipelineNode::Type::NODE_ALGORITHM:
        case gnomonPipelineNode::Type::NODE_FILTER:
        case gnomonPipelineNode::Type::NODE_CONVERTER:
        {
            node_id = this->algorithm_nodes.key(dynamic_cast<gnomonPipelineNodeAlgorithm*>(node));
            output = this->algorithm_output[node_id];
            input_forms = convertToQVariantMap(this->node_input_forms[dynamic_cast<gnomonPipelineNodeAlgorithm*>(node)]);
            break;
        }
        case gnomonPipelineNode::Type::NODE_READER:
        {
            node_id = this->reader_nodes.key(dynamic_cast<gnomonPipelineNodeReader*>(node));
            output = this->reader_output[node_id];
            break;
        }
        case gnomonPipelineNode::Type::NODE_WRITER:
        {
            // exceptionally writer node is identified by it's own name
            node_id = name;
            input_forms = convertToQVariantMap(this->node_input_forms[dynamic_cast<gnomonPipelineNodeWriter*>(node)]);
            break;
        }
        case gnomonPipelineNode::Type::NODE_CONSTRUCTOR:
        {
            node_id = this->constructor_nodes.key(dynamic_cast<gnomonPipelineNodeConstructor*>(node));
            output = this->constructor_output[node_id];
            break;
        }
        case gnomonPipelineNode::Type::NODE_ADAPTER:
        {
            node_id = this->adapter_nodes.key(dynamic_cast<gnomonPipelineNodeAdapter*>(node));
            output = this->adapter_output[node_id];
            input_forms = convertToQVariantMap(this->node_input_forms[dynamic_cast<gnomonPipelineNodeAdapter*>(node)]);
            break;
        }
        case gnomonPipelineNode::Type::NODE_TASK:
        {
            node_id = this->task_nodes.key(dynamic_cast<gnomonPipelineNodeTask*>(node));
            output = this->task_output[node_id];
            input_forms = convertToQVariantMap(this->node_input_forms[dynamic_cast<gnomonPipelineNodeTask*>(node)]);
            break;
        }
        case gnomonPipelineNode::Type::NODE_MORPHONET:
        {
            node_id = this->morphonet_nodes.key(dynamic_cast<gnomonPipelineNodeMorphonet*>(node));
            output = this->morphonet_output[node_id];
            break;
        }
        case gnomonPipelineNode::Type::NODE_EVOLUTION_MODEL:
        {
            node_id = this->evolution_model_nodes.key(dynamic_cast<gnomonPipelineNodeEvolutionModel*>(node));
            output = this->evolution_model_output[node_id];
            break;
        }
        default:
        {
            qWarning()<<Q_FUNC_INFO<<"Not able to serialize node type: "<<node->type();
            break;
        }
    }
    component["node_id"] = node_id;
    component["output"] = output;
    component["input_forms"] = input_forms;

    return component;
}

bool gnomonPipelineManagerPrivate::hasNode(gnomonPipelineNode *node)
{
    bool node_found = false;
    for (auto it = this->pipeline_nodes.begin(); it != this->pipeline_nodes.end(); ++it) {
        auto&& n = it.value();
        if (n == node) {
           node_found = true;
           break;
        }
    }
    return node_found;
}

// /////////////////////////////////////////////////////////////////
// gnomonPipelineManager
// /////////////////////////////////////////////////////////////////

gnomonPipelineManager *gnomonPipelineManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonPipelineManager;

    return s_instance;
}

gnomonPipelineManager::gnomonPipelineManager(QObject *parent) : QObject(parent)
{
    d = new gnomonPipelineManagerPrivate;
    d->pipeline = new gnomonPipeline;
    d->q = this;
}

gnomonPipelineManager::~gnomonPipelineManager(void)
{
    delete d->pipeline;
    delete d;
}

gnomonPipeline *gnomonPipelineManager::pipeline(void)
{
    return d->pipeline;
}

void gnomonPipelineManager::addReader(gnomonAbstractReaderCommand *command)
{
    //TOCHECK TODO ?
    //qDebug() << "Form added, cannot be destroyed! ";
    //qDebug() << "todo tell reader command ? ";
    QMap<QString, QString > forms = d->commandFormUuids(command, "output");
    gnomonPipelineNodeReader *node = new gnomonPipelineNodeReader(command->factoryName(), command->algorithmName(),
                                                                  command->path(), forms.keys());
    node->setVersion(command->version());

    for (auto it = forms.begin(); it != forms.end(); ++it) {
        auto&& form_name = it.key();
        const QString& form_uuid = it.value();
        d->reader_nodes[form_uuid] = node;
        d->reader_output[form_uuid] = form_name;
    }
}

void gnomonPipelineManager::addWriter(gnomonAbstractWriterCommand *command)
{
    QMap<QString, QString > input_forms = d->commandFormUuids(command, "input");

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(), command->algorithmName(), command->path(), input_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;
    d->linkNodeInputs(node);

    d->pipeline->addNode(node);
    d->pipeline_nodes[node->name()] = node;

}


void gnomonPipelineManager::addAdapter(gnomonAbstractAdapterCommand *command)
{
    QMap<QString, QString > input_forms = d->commandFormUuids(command, "input");
    QMap<QString, QString > output_forms = d->commandFormUuids(command, "output");

    gnomonPipelineNodeAdapter *node = new gnomonPipelineNodeAdapter(command->factoryName(), command->algorithmName(), input_forms.keys(), output_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->adapter_nodes[it.value()] = node;
        d->adapter_output[it.value()] = output;
    }
}


void gnomonPipelineManager::addAlgorithm(gnomonAbstractAlgorithmCommand *command)
{
    QMap<QString, QString > input_forms = d->commandFormUuids(command, "input");
    QMap<QString, QString > output_forms = d->commandFormUuids(command, "output");

    QJsonObject parameter_json = d->parameterJson(command->parameters());
    if (auto python_command = dynamic_cast<gnomonFormAlgorithmCommand *>(command)) {
        QString code = python_command->pythonCode();
        parameter_json.insert("python_code", QJsonValue(code));
    }

    gnomonPipelineNodeAlgorithm *node = new gnomonPipelineNodeAlgorithm(command->factoryName(),
                                                                        command->algorithmName(), parameter_json,
                                                                        input_forms.keys(),
                                                                        output_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->algorithm_nodes[it.value()] = node;
        d->algorithm_output[it.value()] = output;
    }

}

void gnomonPipelineManager::addTask(const QString &task,
                                    QMap<QString, QString> inputs,
                                    QMap<QString, QString> outputs) {
    auto node = new gnomonPipelineNodeTask(task, inputs.keys(), outputs.keys());
    d->node_input_forms[node] = inputs;
    for (auto it = outputs.begin(); it != outputs.end(); ++it) {
        auto&& output = it.key();
        d->task_nodes[it.value()] = node;
        d->task_output[it.value()] = output;
    }

}

void gnomonPipelineManager::addConstructor(gnomonAbstractConstructorCommand *command)
{
    QMap<QString, QString > output_forms = d->commandFormUuids(command, "output");

    QJsonObject parameter_json = d->parameterJson(command->parameters());
    gnomonPipelineNodeConstructor *node = new gnomonPipelineNodeConstructor(command->factoryName(), command->algorithmName(), parameter_json, output_forms.keys());
    node->setVersion(command->version());

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->constructor_nodes[it.value()] = node;
        d->constructor_output[it.value()] = output;
    }
}

void gnomonPipelineManager::addEvolutionModel(gnomonAbstractEvolutionModelCommand *command)
{
    QMap<QString, QString > input_forms = d->commandFormUuids(command, "input");
    QMap<QString, QString > output_forms = d->commandFormUuids(command, "output");

    QJsonObject parameter_json = d->parameterJson(command->parameters());
    if (auto lsystem_command = dynamic_cast<gnomonLStringEvolutionModelCommand *>(command)) {
        QString code = lsystem_command->lSystemCode();
        parameter_json.insert("lsystem_code", QJsonValue(code));

        int derivation_length = lsystem_command->derivationLength();
        parameter_json.insert("derivation_length", QJsonValue(derivation_length));
    }

    gnomonPipelineNodeEvolutionModel *node = new gnomonPipelineNodeEvolutionModel(command->factoryName(), command->modelName(), parameter_json, output_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->evolution_model_nodes[it.value()] = node;
        d->evolution_model_output[it.value()] = output;
    }
}

void gnomonPipelineManager::addAdaptedForm(const QString& form_uuid)
{
    if (d->adapter_nodes.contains(form_uuid))
    {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form_uuid];
        if (!d->pipeline_nodes.values().contains(node)) {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addMorphoForm(const QString& form_uuid, int id, double voxelsize, int time_start, int time_end)
{
    QJsonObject morphonet_data;
    morphonet_data.insert("id", id);
    morphonet_data.insert("voxelsize", voxelsize);
    morphonet_data.insert("start_time", time_start);
    morphonet_data.insert("end_time", time_end);

    const QString& form_name = GNOMON_SESSION->getForm(form_uuid)->formName().remove("gnomon");
    gnomonPipelineNodeMorphonet *node = new gnomonPipelineNodeMorphonet(form_name, morphonet_data);
    d->morphonet_nodes[form_uuid] = node;
    d->morphonet_output[form_uuid] = form_name;
}

void gnomonPipelineManager::addForm(const QString& form_uuid)
{
    this->setFormIndex(form_uuid, GNOMON_SESSION->getForm(form_uuid)->thumbnailId());
    if (d->reader_nodes.contains(form_uuid)) {
        gnomonPipelineNodeReader *node = d->reader_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->adapter_nodes.contains(form_uuid)) {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->constructor_nodes.contains(form_uuid)) {
        gnomonPipelineNodeConstructor *node = d->constructor_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->algorithm_nodes.contains(form_uuid)) {
        gnomonPipelineNodeAlgorithm *node = d->algorithm_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->evolution_model_nodes.contains(form_uuid)) {
        gnomonPipelineNodeEvolutionModel *node = d->evolution_model_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            //d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->task_nodes.contains(form_uuid)) {
        auto *node = d->task_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->morphonet_nodes.contains(form_uuid)) {
        gnomonPipelineNodeMorphonet *node = d->morphonet_nodes[form_uuid];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addClonedForm(const QString& form_uuid, QString clone)
{
    qDebug() << Q_FUNC_INFO << "nothing is done";
    //d->form_clones[clone] = form;
}

void gnomonPipelineManager::setFormIndex(const QString& form_uuid, int index)
{
    if (index > -1) {
        gnomonPipelinePort *output_port = nullptr;
        if (d->reader_nodes.contains(form_uuid)) {
            output_port= d->reader_nodes[form_uuid]->outputPorts()[d->reader_output[form_uuid]];
        } else if (d->constructor_nodes.contains(form_uuid)) {
            output_port = d->constructor_nodes[form_uuid]->outputPorts()[d->constructor_output[form_uuid]];
        } else if (d->adapter_nodes.contains(form_uuid)) {
            output_port = d->adapter_nodes[form_uuid]->outputPorts()[d->adapter_output[form_uuid]];
        } else if (d->algorithm_nodes.contains(form_uuid)) {
            output_port = d->algorithm_nodes[form_uuid]->outputPorts()[d->algorithm_output[form_uuid]];
        } else if (d->task_nodes.contains(form_uuid)) {
            output_port = d->task_nodes[form_uuid]->outputPorts()[d->task_output[form_uuid]];
        } else if(d->morphonet_nodes.contains(form_uuid)) {
            output_port = d->morphonet_nodes[form_uuid]->outputPorts()[d->morphonet_output[form_uuid]];
        }
        if (output_port) {
            output_port->setFormIndex(index);
        }
    }
}

bool gnomonPipelineManager::removeForm(const QString& form_uuid)
{
    auto removeNodeFromPipeline = [=](gnomonPipelineNode *node){
        if(!node->outputEdgeCount()) {
            for(auto edge:node->inputEdges()) {
                edge->source()->node()->removeOutputEdge(edge);
                node->removeInputEdge(edge);
            }
            d->node_input_forms.remove(node);
            d->pipeline->removeNode(node);
            d->pipeline_nodes.remove(node->name());
            return true;
        }
        return false;
    };

    bool res = false;
    if (d->reader_nodes.contains(form_uuid)) {
        auto *node = d->reader_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->reader_output.remove(form_uuid);
    } else if (d->adapter_nodes.contains(form_uuid)) {
        auto *node = d->adapter_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->adapter_output.remove(form_uuid);
    } else if (d->constructor_nodes.contains(form_uuid)) {
        auto *node = d->constructor_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->constructor_output.remove(form_uuid);
    } else if (d->algorithm_nodes.contains(form_uuid)) {
        auto *node = d->algorithm_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->algorithm_output.remove(form_uuid);
    } else if (d->task_nodes.contains(form_uuid)) {
        auto *node = d->task_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->task_output.remove(form_uuid);
    } else if (d->morphonet_nodes.contains(form_uuid)) {
        auto *node = d->morphonet_nodes[form_uuid];
        res = removeNodeFromPipeline(node);
        if(res) d->morphonet_output.remove(form_uuid);
    }

    return res;
}

std::pair<QString, gnomonPipelineNodeReader *> gnomonPipelineManager::cacheNode(const QString& form_uuid)
{
    if (d->reader_nodes.contains(form_uuid)) {
        auto *node = d->reader_nodes.take(form_uuid);
        QString name = d->reader_output.take(form_uuid);
        return std::make_pair(name, node);
    } else {
        qWarning() << "only reader_nodes can be cached! form " << form_uuid;
        return std::make_pair("", nullptr);
    }
}

void gnomonPipelineManager::decacheNode(const QString& form_uuid, std::pair<QString, gnomonPipelineNodeReader *> name_and_node)
{
        d->reader_output[form_uuid] = name_and_node.first;
        d->reader_nodes[form_uuid] = name_and_node.second;
}

QJsonObject gnomonPipelineManager::dumpState(void)
{
    QJsonObject state;
    for(const auto &name: d->pipeline->nodeNames()){
        QString node_id;
        auto component = d->getNodeSpecificData(name, node_id);
        QString node_name = component["node_name"].toString();
        state[node_name] = QJsonObject::fromVariantMap(component);
    }
    return state;
}

void gnomonPipelineManager::loadState(const QJsonObject& state, std::shared_ptr<gnomonPipeline> pipeline)
{
    auto addNodeInputForms = [=](gnomonPipelineNode* node, const QVariantMap& input_forms){
        for(const auto& [key, value]: input_forms.asKeyValueRange())
            d->node_input_forms[node][key] = value.toString();
    };

    for(auto node_name: pipeline->scheduledNodeNames()) {
        auto component = state[node_name].toObject().toVariantMap();
        auto node_id = component["node_id"].toString();

        gnomonPipelineNode *pipeline_node = pipeline->node(node_name);

        switch (component["node_type"].toInt()) {
            case gnomonPipelineNode::Type::NODE_ALGORITHM:
            case gnomonPipelineNode::Type::NODE_FILTER:
            case gnomonPipelineNode::Type::NODE_CONVERTER: {
                gnomonPipelineNodeAlgorithm *node = dynamic_cast<gnomonPipelineNodeAlgorithm *>(pipeline_node);
                d->algorithm_nodes[node_id] = node;
                d->algorithm_output[node_id] = component["output"].toString();
                addNodeInputForms(node, component["input_forms"].toMap());
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_READER: {
                gnomonPipelineNodeReader *node = dynamic_cast<gnomonPipelineNodeReader *>(pipeline_node);
                d->reader_nodes[node_id] = node;
                d->reader_output[node_id] = component["output"].toString();
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_WRITER: {
                gnomonPipelineNodeWriter *node = dynamic_cast<gnomonPipelineNodeWriter *>(pipeline_node);
                addNodeInputForms(node, component["input_forms"].toMap());
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_CONSTRUCTOR: {
                gnomonPipelineNodeConstructor *node = dynamic_cast<gnomonPipelineNodeConstructor *>(pipeline_node);
                d->constructor_nodes[node_id] = node;
                d->constructor_output[node_id] = component["output"].toString();
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_ADAPTER: {
                gnomonPipelineNodeAdapter *node = dynamic_cast<gnomonPipelineNodeAdapter *>(pipeline_node);
                d->adapter_nodes[node_id] = node;
                d->adapter_output[node_id] = component["output"].toString();
                addNodeInputForms(node, component["input_forms"].toMap());
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_TASK: {
                gnomonPipelineNodeTask *node = dynamic_cast<gnomonPipelineNodeTask *>(pipeline_node);
                d->task_nodes[node_id] = node;
                d->task_output[node_id] = component["output"].toString();
                addNodeInputForms(node, component["input_forms"].toMap());
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_MORPHONET: {
                gnomonPipelineNodeMorphonet *node = dynamic_cast<gnomonPipelineNodeMorphonet *>(pipeline_node);
                d->morphonet_nodes[node_id] = node;
                d->morphonet_output[node_id] = component["output"].toString();
                d->pipeline_nodes[node_name] = node;
                break;
            }
            case gnomonPipelineNode::Type::NODE_EVOLUTION_MODEL: {
                gnomonPipelineNodeEvolutionModel *node = dynamic_cast<gnomonPipelineNodeEvolutionModel *>(pipeline_node);
                d->evolution_model_nodes[node_id] = node;
                d->evolution_model_output[node_id] = component["output"].toString();
                d->pipeline_nodes[node_name] = node;
                break;
            }
            default: {
                break;
            }
        }
        d->pipeline->addNode(pipeline_node);

        auto input_forms = component["input_forms"].toMap();
        for (auto e: pipeline_node->inputEdges()) {
            if (input_forms.contains(e->target()->name())) {
                QString form_id = input_forms[e->target()->name()].toString();
                if(GNOMON_SESSION->getForm(form_id)) {
                    e->setFormIndex(GNOMON_SESSION->getForm(form_id)->thumbnailId());
                }
            }
        }
    }
}

gnomonPipelineManager *gnomonPipelineManager::s_instance = nullptr;

//
// gnomonPipelineManager.cpp ends here
