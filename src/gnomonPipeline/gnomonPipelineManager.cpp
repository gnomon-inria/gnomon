#include "gnomonPipelineManager.h"

#include "gnomonPipeline.h"
#include "gnomonPipelineNode.h"
#include "gnomonPipelineEdge.h"
#include "gnomonPipelinePort.h"

#include "gnomonPipelineNodeAdapter.h"
#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeConstructor.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeTask.h"
#include "gnomonPipelineNodeWriter.h"
#include "gnomonPipelineNodeMorphonet.h"

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonFormAlgorithmCommand>

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

    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeReader *> reader_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> reader_output;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeWriter *> writer_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeAdapter *> adapter_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> adapter_output;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> algorithm_output;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeTask *> task_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> task_output;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeMorphonet *> morphonet_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> morphonet_output;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , gnomonPipelineNodeConstructor *> constructor_nodes;
    QMap<std::shared_ptr<gnomonAbstractDynamicForm> , QString> constructor_output;

    QMap<gnomonPipelineNode *, QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > > node_input_forms;
    //QMap<std::shared_ptr<gnomonAbstractDynamicForm> , std::shared_ptr<gnomonAbstractDynamicForm> > form_clones; //TODO check what is it used for

public:
    void linkNodeInputs(gnomonPipelineNode *node);
    QVariantMap parameterVariantValues(const dtkCoreParameters&parameters);
    QJsonObject parameterJson(const dtkCoreParameters& parameters);

public:
    bool hasNode(gnomonPipelineNode *);
};


void gnomonPipelineManagerPrivate::linkNodeInputs(gnomonPipelineNode *node)
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> input_forms = this->node_input_forms[node];
    for (auto it = input_forms.begin(); it != input_forms.end(); ++it) {
        auto&& input = it.key();
        std::shared_ptr<gnomonAbstractDynamicForm> input_form = input_forms[input];
        if (input_form) {
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
                edge->setFormIndex(it.value()->thumbnailId());
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
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms = command->outputs();

    gnomonPipelineNodeReader *node = new gnomonPipelineNodeReader(command->factoryName(), command->algorithmName(),
                                                                  command->path(), forms.keys());
    node->setVersion(command->version());

    for (auto it = forms.begin(); it != forms.end(); ++it) {
        auto&& form_name = it.key();
        std::shared_ptr<gnomonAbstractDynamicForm> form = forms[form_name];
        d->reader_nodes[form] = node;
        d->reader_output[form] = form_name;
    }
}

void gnomonPipelineManager::addWriter(gnomonAbstractWriterCommand *command)
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > input_forms = command->inputs();

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(), command->algorithmName(), command->path(), input_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;
    d->linkNodeInputs(node);

    d->pipeline->addNode(node);
    d->pipeline_nodes[node->name()] = node;

}


void gnomonPipelineManager::addAdapter(gnomonAbstractAdapterCommand *command)
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > input_forms = command->inputs();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > output_forms = command->outputs();

    gnomonPipelineNodeAdapter *node = new gnomonPipelineNodeAdapter(command->factoryName(), command->algorithmName(), input_forms.keys(), output_forms.keys());
    node->setVersion(command->version());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->adapter_nodes[output_forms[output]] = node;
        d->adapter_output[output_forms[output]] = output;
    }
}


void gnomonPipelineManager::addAlgorithm(gnomonAbstractCommand *command)
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > input_forms = command->inputs();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > output_forms = command->outputs();

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
        d->algorithm_nodes[output_forms[output]] = node;
        d->algorithm_output[output_forms[output]] = output;
    }

}

void gnomonPipelineManager::addTask(const QString &task,
                                    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> inputs,
                                    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> outputs) {
    auto node = new gnomonPipelineNodeTask(task, inputs.keys(), outputs.keys());
    d->node_input_forms[node] = inputs;
    for (auto it = outputs.begin(); it != outputs.end(); ++it) {
        auto&& output = it.key();
        d->task_nodes[outputs[output]] = node;
        d->task_output[outputs[output]] = output;
    }

}

void gnomonPipelineManager::addConstructor(gnomonAbstractConstructorCommand *command)
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > output_forms = command->outputs();

    QJsonObject parameter_json = d->parameterJson(command->parameters());
    gnomonPipelineNodeConstructor *node = new gnomonPipelineNodeConstructor(command->factoryName(), command->algorithmName(), parameter_json, output_forms.keys());
    node->setVersion(command->version());

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->constructor_nodes[output_forms[output]] = node;
        d->constructor_output[output_forms[output]] = output;
    }
}

void gnomonPipelineManager::addEvolutionModel(gnomonAbstractEvolutionModelCommand *command)
{
    /*QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > output_forms = command->outputs();

    QJsonObject parameter_json = d->parameterJson(command->parameters());
    gnomonPipelineNodeConstructor *node = new gnomonPipelineNodeConstructor(command->factoryName(), command->algorithmName(), parameter_json, output_forms.keys());
    node->setVersion(command->version());

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->constructor_nodes[output_forms[output]] = node;
        d->constructor_output[output_forms[output]] = output;
    }*/
}

void gnomonPipelineManager::addAdaptedForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (d->adapter_nodes.contains(form))
    {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];
        if (!d->pipeline_nodes.values().contains(node)) {

            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addMorphoForm(std::shared_ptr<gnomonAbstractDynamicForm> form, int id, double voxelsize, int time_start, int time_end)
{
    QJsonObject morphonet_data;
    morphonet_data.insert("id", id);
    morphonet_data.insert("voxelsize", voxelsize);
    morphonet_data.insert("start_time", time_start);
    morphonet_data.insert("end_time", time_end);

    QString form_name = form->formName().remove("gnomon");
    gnomonPipelineNodeMorphonet *node = new gnomonPipelineNodeMorphonet(form_name, morphonet_data);
    d->morphonet_nodes[form] = node;
    d->morphonet_output[form] = form_name;  
}

void gnomonPipelineManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (d->reader_nodes.contains(form)) {
        gnomonPipelineNodeReader *node = d->reader_nodes[form];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->adapter_nodes.contains(form)) {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->constructor_nodes.contains(form)) {
        gnomonPipelineNodeConstructor *node = d->constructor_nodes[form];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonPipelineNodeAlgorithm *node = d->algorithm_nodes[form];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->task_nodes.contains(form)) {
        auto *node = d->task_nodes[form];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->morphonet_nodes.contains(form)) {
        gnomonPipelineNodeMorphonet *node = d->morphonet_nodes[form];

        if (!d->hasNode(node))
        {
            d->pipeline->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addClonedForm(std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractDynamicForm> clone)
{
    qDebug() << Q_FUNC_INFO << "nothing is done";
    //d->form_clones[clone] = form;
}

void gnomonPipelineManager::setFormIndex(std::shared_ptr<gnomonAbstractDynamicForm> form, int index)
{
    if (index > -1) {
        gnomonPipelinePort *output_port = nullptr;
        if (d->reader_nodes.contains(form)) {
            output_port= d->reader_nodes[form]->outputPorts()[d->reader_output[form]];
        } else if (d->constructor_nodes.contains(form)) {
            output_port = d->constructor_nodes[form]->outputPorts()[d->constructor_output[form]];
        } else if (d->adapter_nodes.contains(form)) {
            output_port = d->adapter_nodes[form]->outputPorts()[d->adapter_output[form]];
        } else if (d->algorithm_nodes.contains(form)) {
            output_port = d->algorithm_nodes[form]->outputPorts()[d->algorithm_output[form]];
        }else if (d->task_nodes.contains(form)) {
            output_port = d->task_nodes[form]->outputPorts()[d->task_output[form]];
        } else if(d->morphonet_nodes.contains(form)) {
            output_port = d->morphonet_nodes[form]->outputPorts()[d->morphonet_output[form]];
        }
        if (output_port) {
            output_port->setFormIndex(index);
        }
    }
}

bool gnomonPipelineManager::removeForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
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
    if (d->reader_nodes.contains(form)) {
        auto *node = d->reader_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->reader_output.remove(form);
    } else if (d->adapter_nodes.contains(form)) {
        auto *node = d->adapter_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->adapter_output.remove(form);
    } else if (d->constructor_nodes.contains(form)) {
        auto *node = d->constructor_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->constructor_output.remove(form);
    } else if (d->algorithm_nodes.contains(form)) {
        auto *node = d->algorithm_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->algorithm_output.remove(form);
    } else if (d->task_nodes.contains(form)) {
        auto *node = d->task_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->task_output.remove(form);
    } else if (d->morphonet_nodes.contains(form)) {
        auto *node = d->morphonet_nodes[form];
        res = removeNodeFromPipeline(node);
        if(res) d->morphonet_output.remove(form);
    }

    return res;
}

std::pair<QString, gnomonPipelineNodeReader *> gnomonPipelineManager::cacheNode(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (d->reader_nodes.contains(form)) {
        auto *node = d->reader_nodes.take(form);
        QString name = d->reader_output.take(form);
        return std::make_pair(name, node);
    } else {
        qWarning() << "only reader_nodes can be cached! form " << form->formName();
        return std::make_pair("", nullptr);
    }
}

void gnomonPipelineManager::decachNode(std::shared_ptr<gnomonAbstractDynamicForm> form, std::pair<QString, gnomonPipelineNodeReader *> name_and_node)
{
        d->reader_output[form] = name_and_node.first;
        d->reader_nodes[form] = name_and_node.second;
}

gnomonPipelineManager *gnomonPipelineManager::s_instance = nullptr;

//
// gnomonPipelineManager.cpp ends here
