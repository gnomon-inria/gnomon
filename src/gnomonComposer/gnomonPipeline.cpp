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
    QMap< QPair<QString, QString>, QPair<QString, QString> > pipeline_edges;

    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeReader *> reader_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> reader_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeWriter *> writer_nodes;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;

    QMap<gnomonPipelineNode *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;

    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;

public:
    void linkNodeInputs(gnomonPipelineNode *node);
};


void gnomonPipelinePrivate::linkNodeInputs(gnomonPipelineNode *node)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = this->node_input_forms[node];
    for (const auto& input : input_forms.keys()) {
        gnomonAbstractDynamicForm *input_form = input_forms[input];
        if (this->form_clones.contains(input_form)) {
            input_form = this->form_clones[input_form];
        }
        dtkComposerSceneEdge *edge = nullptr;
        QPair<QString, QString> edge_source;
        QPair<QString, QString> edge_target;
        if (this->reader_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(this->reader_nodes[input_form]->output_ports[this->reader_output[input_form]]);
            edge_source.first = this->pipeline_nodes.key(this->reader_nodes[input_form]);
            edge_source.second = this->reader_output[input_form];
        } else if (this->algorithm_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(this->algorithm_nodes[input_form]->output_ports[this->algorithm_output[input_form]]);
            edge_source.first = this->pipeline_nodes.key(this->algorithm_nodes[input_form]);
            edge_source.second = this->algorithm_output[input_form];
        }
        if (edge) {
            if (gnomonPipelineNodeWriter *writer_node = dynamic_cast<gnomonPipelineNodeWriter *>(node)) {
                edge->setDestination(writer_node->input_ports[input]);
            } else if (gnomonPipelineNodeAlgorithm *algorithm_node = dynamic_cast<gnomonPipelineNodeAlgorithm *>(node)) {
                edge->setDestination(algorithm_node->input_ports[input]);
            } 
            edge->link(true);
            node->addInputEdge(edge);
            edge_target.first = this->pipeline_nodes.key(node);
            edge_target.second = input;
            this->pipeline_edges[edge_target] = edge_source;
        }
    }
}

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
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(),command->algorithmName(),command->path(),input_forms.keys());

    QString node_name = node->algorithm_class;
    if (!d->node_type_count.contains(node->algorithm_class)) {
        d->node_type_count[node->algorithm_class] = 1;
    } else {
        node_name += QString::number(d->node_type_count[node->algorithm_class]);
        d->node_type_count[node->algorithm_class] += 1;
    }
    d->pipeline_node_names.append(node_name);
    d->pipeline_nodes[node_name] = node;

    d->node_input_forms[node] = input_forms;

    d->linkNodeInputs(node);

    emit nodeAdded(node);
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

        QString node_name = node->algorithm_class;
        if (!d->node_type_count.contains(node->algorithm_class)) {
            d->node_type_count[node->algorithm_class] = 1;
        } else {
            node_name += QString::number(d->node_type_count[node->algorithm_class]);
            d->node_type_count[node->algorithm_class] += 1;
        }
        d->pipeline_node_names.append(node_name);
        d->pipeline_nodes[node_name] = node;

        d->linkNodeInputs(node);

        emit nodeAdded(node);
    }
}

void gnomonPipeline::addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone)
{
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
}

void gnomonPipeline::exportToLuigiScript(const QString& path)
{
    QFileInfo info(path);
    QString config_path = info.path() + "/" + info.baseName() + ".toml";

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
    for (const auto& node_name : d->pipeline_node_names) {
        if (d->node_type_count.contains(node_name)) {
            out << d->pipeline_nodes[node_name]->toLuigiClass();
        }
    }
    out << "\n";
    out << "def main():\n";
    out << "    parser = argparse.ArgumentParser()\n";
    out << "    parser.add_argument(\"-c\", \"--config-filename\", ";
    out << "help=\"Path to the TOML file containing pipeline config\", ";
    out << "default=\"" << info.baseName() << ".toml\")\n";
    out << "    args = parser.parse_args()\n";
    out << "\n";
    out << "    config = luigi.configuration.LuigiTomlParser().read(config_paths=[args.config_filename])\n";
    out << "\n";
    out << "    tasks = {}\n";

    for (const auto& node_name: d->pipeline_node_names) {
        QString class_name = QString(node_name).remove(QRegExp("[0-9]")) + "Task";
        class_name.replace(0, 1, class_name[0].toUpper());
        out << "    tasks[\"" << node_name <<"\"] = " << class_name << "(**config[\"" << node_name <<"\"])\n";

        for (const auto& edge_target : d->pipeline_edges.keys()) {
            if (edge_target.first == node_name) {
                out << "    tasks[\"" << node_name <<"\"].connect_input(tasks[\"" << d->pipeline_edges[edge_target].first << "\"], ";
                out << "output_name=\"" << d->pipeline_edges[edge_target].second << "\", ";
                out << "input_name=\"" << edge_target.second << "\")\n";
            }
        }
        out << "\n";
    }

    out << "    sink_tasks = []\n";

    QMap<QString, bool> node_sink;
    for (const auto& node_name: d->pipeline_node_names) {
        node_sink[node_name] = true;
    }
    for (const auto& edge_source : d->pipeline_edges.values()) {
        node_sink[edge_source.first] = false;
    }
    for (const auto& node_name: d->pipeline_node_names) {
        if (node_sink[node_name]) {
            out << "    sink_tasks.append(tasks[\"" << node_name << "\"])\n";
        }
    }

    out << "\n";
    out << "    luigi.build(sink_tasks, local_scheduler=True)\n";
    out << "\n";
    out << "\n";
    out << "if __name__ == '__main__':\n";
    out << "    main()\n";
    out << "\n";

    file.close();

    this->exportToToml(config_path);
}


gnomonPipeline *gnomonPipeline::s_instance = nullptr;

//
// gnomonPipeline.cpp ends here