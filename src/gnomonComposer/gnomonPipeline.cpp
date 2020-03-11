// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonPipeline.h"

#include "gnomonPipelineNode.h"

#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeWriter.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <dtkCore>
#include <dtkComposer>


// /////////////////////////////////////////////////////////////////
// gnomonPipelinePrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelinePrivate
{
public:
    QStringList pipeline_node_names;
    QMap<QString, int> node_type_count;
    QMap<QString, gnomonPipelineNode *> pipeline_nodes;

    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeReader *> reader_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> reader_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeWriter *> writer_nodes;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;

    QMap<gnomonPipelineNodeAlgorithm *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;

    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipeline
// /////////////////////////////////////////////////////////////////

gnomonPipeline *gnomonPipeline::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonPipeline;

    return s_instance;
}


gnomonPipeline::gnomonPipeline(void)
{
    d = new gnomonPipelinePrivate;
}

gnomonPipeline::~gnomonPipeline(void)
{
    delete d;
}

void gnomonPipeline::addReader(gnomonAbstractReaderCommand *command)
{
    qDebug() << Q_FUNC_INFO << command->factoryName() << "[" << command->algorithmName() << "] : "<<command->path();
    QMap<QString, gnomonAbstractDynamicForm *> forms = command->outputs();

    gnomonPipelineNodeReader *node = new gnomonPipelineNodeReader(command->factoryName(),command->algorithmName(),command->path(),forms.keys());

    for (const auto& form_name : forms.keys())
    {
        gnomonAbstractDynamicForm *form = forms[form_name];
        if (gnomonAbstractDynamicForm *clone = d->form_clones.key(form,nullptr)) {
            form = clone;
        }
        d->reader_nodes[form] = node;
        d->reader_output[form] = form_name;
    }
}

void gnomonPipeline::addWriter(gnomonAbstractWriterCommand *command)
{
    qDebug() << Q_FUNC_INFO << command->factoryName() << "[" << command->algorithmName() << "] : "<<command->path();
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(),command->algorithmName(),command->path(),input_forms.keys());

    for (const auto& form_name : input_forms.keys())
    {
        gnomonAbstractDynamicForm *input_form = input_forms[form_name];
        if (d->form_clones.contains(input_form)) {
            qDebug()<<Q_FUNC_INFO<<input_form<<"->"<<d->form_clones[input_form];
            input_form = d->form_clones[input_form];
        }

        dtkComposerSceneEdge *edge = nullptr;
        if (d->reader_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(d->reader_nodes[input_form]->output_ports[d->reader_output[input_form]]);
        } else if (d->algorithm_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(d->algorithm_nodes[input_form]->output_ports[d->algorithm_output[input_form]]);
        }
        if (edge) {
            edge->setDestination(node->input_ports[form_name]);
            edge->link(true);
            node->addInputEdge(edge);
        }

        d->writer_nodes[input_form] = node;
        QString node_name = node->algorithm_class;
        if (!d->node_type_count.contains(node->algorithm_class)) {
            d->node_type_count[node->algorithm_class] = 1;
        } else {
            node_name += QString::number(d->node_type_count[node->algorithm_class]);
            d->node_type_count[node->algorithm_class] += 1;
        }
        d->pipeline_node_names.append(node_name);
        d->pipeline_nodes[node_name] = node;
        emit nodeAdded(node);
    }
}

void gnomonPipeline::addAlgorithm(gnomonAbstractAlgorithmCommand *command)
{
    QMap<QString, gnomonCoreParameter *> parameters = command->parameters();

    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();
    QMap<QString, gnomonAbstractDynamicForm *> output_forms = command->outputs();

    QMap<QString, QVariant> parameter_values;
    for (const auto& param : parameters.keys()) {
        if (gnomonCoreParameterInt *parameter = dynamic_cast<gnomonCoreParameterInt *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        } else if (gnomonCoreParameterDouble *parameter = dynamic_cast<gnomonCoreParameterDouble *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        }  else if (gnomonCoreParameterIntRange *parameter = dynamic_cast<gnomonCoreParameterIntRange *>(parameters[param])) {
            QList<QVariant> range;
            for (const auto& val : parameter->value()) {
                range.append(QVariant(val));
            }
            parameter_values[param] = QVariant(range);
        } else if (gnomonCoreParameterDoubleRange *parameter = dynamic_cast<gnomonCoreParameterDoubleRange *>(parameters[param])) {
            QList<QVariant> range;
            for (const auto& val : parameter->value()) {
                range.append(QVariant(val));
            }
            parameter_values[param] = QVariant(range);
        }  else if (gnomonCoreParameterBool *parameter = dynamic_cast<gnomonCoreParameterBool *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        } else if (gnomonCoreParameterString *parameter = dynamic_cast<gnomonCoreParameterString *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        } else if (gnomonCoreParameterStringList *parameter = dynamic_cast<gnomonCoreParameterStringList *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        } else if (gnomonCoreParameterFile *parameter = dynamic_cast<gnomonCoreParameterFile *>(parameters[param])) {
            parameter_values[param] = QVariant(parameter->value());
        }
    }

    gnomonPipelineNodeAlgorithm *node = new gnomonPipelineNodeAlgorithm(command->factoryName(),command->algorithmName(),parameter_values,input_forms.keys(),output_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (const auto& output : output_forms.keys()) {
        d->algorithm_nodes[output_forms[output]] = node;
        d->algorithm_output[output_forms[output]] = output;
    }
}

void gnomonPipeline::addForm(gnomonAbstractDynamicForm *form)
{
    if (d->reader_nodes.contains(form)) {
        gnomonPipelineNodeReader *node = d->reader_nodes[form];
        QString node_name = node->algorithm_class;
        if (!d->node_type_count.contains(node->algorithm_class)) {
            d->node_type_count[node->algorithm_class] = 1;
        } else {
            node_name += QString::number(d->node_type_count[node->algorithm_class]);
            d->node_type_count[node->algorithm_class] += 1;
        }
        d->pipeline_node_names.append(node_name);
        d->pipeline_nodes[node_name] = node;
        emit nodeAdded(node);
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonPipelineNodeAlgorithm *node = d->algorithm_nodes[form];
        QMap<QString, gnomonAbstractDynamicForm *> input_forms = d->node_input_forms[node];
        for (const auto& input : input_forms.keys()) {
            gnomonAbstractDynamicForm *input_form = input_forms[input];
            if (d->form_clones.contains(input_form)) {
                input_form = d->form_clones[input_form];
            }

            dtkComposerSceneEdge *edge = nullptr;
            if (d->reader_nodes.contains(input_form)) {
                edge = new dtkComposerSceneEdge();
                edge->setSource(d->reader_nodes[input_form]->output_ports[d->reader_output[input_form]]);
            } else if (d->algorithm_nodes.contains(input_form)) {
                edge = new dtkComposerSceneEdge();
                edge->setSource(d->algorithm_nodes[input_form]->output_ports[d->algorithm_output[input_form]]);
            }
            if (edge) {
                edge->setDestination(node->input_ports[input]);
                edge->link(true);
                node->addInputEdge(edge);
            }
        }
        QString node_name = node->algorithm_class;
        if (!d->node_type_count.contains(node->algorithm_class)) {
            d->node_type_count[node->algorithm_class] = 1;
        } else {
            node_name += QString::number(d->node_type_count[node->algorithm_class]);
            d->node_type_count[node->algorithm_class] += 1;
        }
        d->pipeline_node_names.append(node_name);
        d->pipeline_nodes[node_name] = node;
        emit nodeAdded(node);
    }
}

void gnomonPipeline::addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone)
{
    qDebug()<<Q_FUNC_INFO<<clone<<"->"<<form;
    d->form_clones[clone] = form;
}

void gnomonPipeline::exportToToml(const QString& path)
{
    Q_ASSERT(path.endsWith(".toml"));

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& node_name : d->pipeline_node_names) {
        out << d->pipeline_nodes[node_name]->toToml(node_name);
    }
    file.close();

    QFileInfo info(path);
    QString script_path = info.path() + "/" + info.baseName() + ".py";

    this->exportToLuigiScript(script_path);
}

void gnomonPipeline::exportToLuigiScript(const QString& path)
{
    qDebug()<<Q_FUNC_INFO<<path;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "import argparse\n";
    out << "\n";
    out << "import luigi\n";
    out << "\n";
    out << "import gnomoncore\n";
    out << "from gnomon_utils import load_plugin_group\n";
    out << "\n";
    out << "from gnomon_luigi.tasks import AlgorithmPluginTask\n";
    out << "\n";
    for (const auto& node_name : d->node_type_count.keys()) {
        out << d->pipeline_nodes[node_name]->toLuigiClass();
    }

    file.close();
}


gnomonPipeline *gnomonPipeline::s_instance = nullptr;

//
// gnomonPipeline.cpp ends here