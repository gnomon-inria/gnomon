#include "gnomonPipelineManager.h"

#include "gnomonPipeline.h"
#include "gnomonPipelineNode.h"
#include "gnomonPipelineEdge.h"
#include "gnomonPipelinePort.h"

#include "gnomonPipelineNodeAdapter.h"
#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeConstructor.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeWriter.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <dtkCore>


// /////////////////////////////////////////////////////////////////
// gnomonPipelineManagerPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineManagerPrivate
{
public:
    gnomonPipelineManager *q;

    QMap<QString, gnomonPipelineNode *> pipeline_nodes;

    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeReader *> reader_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> reader_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeWriter *> writer_nodes;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAdapter *> adapter_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> adapter_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeConstructor *> constructor_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> constructor_output;

    QMap<gnomonAbstractDynamicForm *, int> form_manager_index;

    QMap<gnomonPipelineNode *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;
    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;

public:
    void linkNodeInputs(gnomonPipelineNode *node);
    QVariantMap parameterVariantValues(const dtkCoreParameters&parameters);

public:
    bool hasNode(gnomonPipelineNode *);
};


void gnomonPipelineManagerPrivate::linkNodeInputs(gnomonPipelineNode *node)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = this->node_input_forms[node];
    for (auto it = input_forms.begin(); it != input_forms.end(); ++it) {
        auto&& input = it.key();
        qDebug()<<Q_FUNC_INFO<<input;
        gnomonAbstractDynamicForm *input_form = input_forms[input];
        if (input_form) {
            while (this->form_clones.contains(input_form) & !this->reader_nodes.contains(input_form)) {
                input_form = this->form_clones[input_form];
            }
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
            }
            if (edge) {
                if (gnomonPipelineNodeWriter *writer_node = dynamic_cast<gnomonPipelineNodeWriter *>(node)) {
                    edge->setTarget(writer_node->inputPorts()[input]);
                } else if (gnomonPipelineNodeAdapter *adapter_node = dynamic_cast<gnomonPipelineNodeAdapter *>(node)) {
                    edge->setTarget(adapter_node->inputPorts()[input]);
                } else if (gnomonPipelineNodeAlgorithm *algorithm_node = dynamic_cast<gnomonPipelineNodeAlgorithm *>(node)) {
                    edge->setTarget(algorithm_node->inputPorts()[input]);
                }
                edge->link();

                if (this->form_manager_index.contains(input_form)) {
                    edge->setFormIndex(this->form_manager_index[input_form]);
                }
            }
        }
    }
}

QVariantMap gnomonPipelineManagerPrivate::parameterVariantValues(const dtkCoreParameters&parameters)
{
    QVariantMap parameter_values;
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        auto&& parameter_name = it.key();
        auto&& param = it.value();
        parameter_values[parameter_name] = param->variant();
    }

    return parameter_values;
}

bool gnomonPipelineManagerPrivate::hasNode(gnomonPipelineNode *node)
{
    bool node_found = false;
    for (auto it = this->pipeline_nodes.begin(); it != this->pipeline_nodes.end(); ++it) {
        auto&& n = it.value();
        qDebug()<<Q_FUNC_INFO<<n<<node;
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


gnomonPipelineManager::gnomonPipelineManager(void)
{
    d = new gnomonPipelineManagerPrivate;
    d->q = this;
}

gnomonPipelineManager::~gnomonPipelineManager(void)
{
    delete d;
}

void gnomonPipelineManager::addReader(gnomonAbstractReaderCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> forms = command->outputs();

    gnomonPipelineNodeReader *node = new gnomonPipelineNodeReader(command->factoryName(),command->algorithmName(),command->path(),forms.keys());

    for (auto it = forms.begin(); it != forms.end(); ++it) {
        auto&& form_name = it.key();
        gnomonAbstractDynamicForm *form = forms[form_name];
        if (gnomonAbstractDynamicForm *clone = d->form_clones.key(form,nullptr)) {
            form = clone;
        }
        d->reader_nodes[form] = node;
        d->reader_output[form] = form_name;
    }
}

void gnomonPipelineManager::addWriter(gnomonAbstractWriterCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(),command->algorithmName(),command->path(),input_forms.keys());

    d->node_input_forms[node] = input_forms;
    d->linkNodeInputs(node);

    gnomonPipeline::instance()->addNode(node);
    d->pipeline_nodes[node->name()] = node;

}

void gnomonPipelineManager::addAdapter(gnomonAbstractAdapterCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();
    QMap<QString, gnomonAbstractDynamicForm *> output_forms = command->outputs();

    gnomonPipelineNodeAdapter *node = new gnomonPipelineNodeAdapter(command->factoryName(),command->algorithmName(),input_forms.keys(),output_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->adapter_nodes[output_forms[output]] = node;
        d->adapter_output[output_forms[output]] = output;
    }
}

void gnomonPipelineManager::addAlgorithm(gnomonAbstractCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();
    QMap<QString, gnomonAbstractDynamicForm *> output_forms = command->outputs();

    QVariantMap parameter_values = d->parameterVariantValues(command->parameters());

    gnomonPipelineNodeAlgorithm *node = new gnomonPipelineNodeAlgorithm(command->factoryName(),command->algorithmName(),parameter_values,input_forms.keys(),output_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->algorithm_nodes[output_forms[output]] = node;
        d->algorithm_output[output_forms[output]] = output;
        qDebug()<<Q_FUNC_INFO<<output<<output_forms[output];
    }
}


void gnomonPipelineManager::addConstructor(gnomonAbstractConstructorCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> output_forms = command->outputs();

    QVariantMap parameter_values = d->parameterVariantValues(command->parameters());

    gnomonPipelineNodeConstructor *node = new gnomonPipelineNodeConstructor(command->factoryName(),command->algorithmName(),parameter_values,output_forms.keys());

    for (auto it = output_forms.begin(); it != output_forms.end(); ++it) {
        auto&& output = it.key();
        d->constructor_nodes[output_forms[output]] = node;
        d->constructor_output[output_forms[output]] = output;
    }
}

void gnomonPipelineManager::addAdaptedForm(gnomonAbstractDynamicForm *form)
{
    if (d->adapter_nodes.contains(form))
    {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];
        if (!d->pipeline_nodes.values().contains(node)) {

            d->linkNodeInputs(node);
            gnomonPipeline::instance()->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addForm(gnomonAbstractDynamicForm *form)
{
    if (d->reader_nodes.contains(form)) {
        gnomonPipelineNodeReader *node = d->reader_nodes[form];

        if (!d->hasNode(node))
        {
            gnomonPipeline::instance()->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->adapter_nodes.contains(form)) {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            gnomonPipeline::instance()->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    } else if (d->constructor_nodes.contains(form)) {
        gnomonPipelineNodeConstructor *node = d->constructor_nodes[form];

        if (!d->hasNode(node))
        {
            gnomonPipeline::instance()->addNode(node);
            d->pipeline_nodes[node->name()] = node;
        }
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonPipelineNodeAlgorithm *node = d->algorithm_nodes[form];

        if (!d->hasNode(node))
        {
            d->linkNodeInputs(node);
            gnomonPipeline::instance()->addNode(node);
            d->pipeline_nodes[node->name()] = node;

        }
    }
}

void gnomonPipelineManager::addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone)
{
    d->form_clones[clone] = form;
    if (d->form_manager_index.contains(form)) {
        d->form_manager_index[clone] = d->form_manager_index[form];
    }
}

void gnomonPipelineManager::setFormIndex(gnomonAbstractDynamicForm *form, int index)
{
    if (index > -1) {
        d->form_manager_index[form] = index;
        if (d->form_clones.contains(form)) {
            this->setFormIndex(d->form_clones[form], index);
        }

        gnomonPipelinePort *output_port = nullptr;
        if (d->reader_nodes.contains(form)) {
            output_port= d->reader_nodes[form]->outputPorts()[d->reader_output[form]];
        } else if (d->constructor_nodes.contains(form)) {
            output_port = d->constructor_nodes[form]->outputPorts()[d->constructor_output[form]];
        } else if (d->adapter_nodes.contains(form)) {
            output_port = d->adapter_nodes[form]->outputPorts()[d->adapter_output[form]];
        } else if (d->algorithm_nodes.contains(form)) {
            output_port = d->algorithm_nodes[form]->outputPorts()[d->algorithm_output[form]];
        }
        if (output_port) {
            output_port->setFormIndex(d->form_manager_index[form]);
        }
    }
}


gnomonPipelineManager *gnomonPipelineManager::s_instance = nullptr;

//
// gnomonPipelineManager.cpp ends here