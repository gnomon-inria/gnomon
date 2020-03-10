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

#include "gnomonComposerSceneNode.h"

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
    QMap<QString, gnomonComposerSceneNode *> pipeline_nodes;

    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeReader *> reader_nodes;
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

    for (const auto& form_name : forms.keys())
    {
        gnomonAbstractDynamicForm *form = forms[form_name];
        if (gnomonAbstractDynamicForm *clone = d->form_clones.key(form,nullptr)) {
            form = clone;
        }
        d->reader_nodes[form] = new gnomonPipelineNodeReader(command->factoryName(),command->algorithmName(),command->path());
    }
}

void gnomonPipeline::addWriter(gnomonAbstractWriterCommand *command)
{
    qDebug() << Q_FUNC_INFO << command->factoryName() << "[" << command->algorithmName() << "] : "<<command->path();
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();

    for (const auto& form_name : input_forms.keys())
    {
        gnomonAbstractDynamicForm *input_form = input_forms[form_name];
//        if (gnomonAbstractDynamicForm *clone = d->form_clones.key(input_form,nullptr)) {
//            qDebug()<<Q_FUNC_INFO<<input_form<<"->"<<clone;
//            input_form = clone;
//        }
        if (d->form_clones.contains(input_form)) {
            qDebug()<<Q_FUNC_INFO<<input_form<<"->"<<d->form_clones[input_form];
            input_form = d->form_clones[input_form];
        }
        gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(),command->algorithmName(),command->path());

        dtkComposerSceneEdge *edge = nullptr;
        qDebug()<<Q_FUNC_INFO<<d->reader_nodes.keys();
        qDebug()<<Q_FUNC_INFO<<d->algorithm_nodes.keys();
        if (d->reader_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(d->reader_nodes[input_form]->output_port);
        } else if (d->algorithm_nodes.contains(input_form)) {
            edge = new dtkComposerSceneEdge();
            edge->setSource(d->algorithm_nodes[input_form]->output_ports[d->algorithm_output[input_form]]);
        }
        if (edge) {
            edge->setDestination(node->input_port);
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
                edge->setSource(d->reader_nodes[input_form]->output_port);
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
        gnomonComposerSceneNode *node = d->pipeline_nodes[node_name];
        if (gnomonPipelineNodeReader * reader_node = dynamic_cast<gnomonPipelineNodeReader *>(node)) {
            out << "[" << node_name << "]" << "\n";
            out << "plugin_name = \""<< reader_node->algorithm << "\"\n";
            out << "path = \""<< reader_node->path << "\"\n";
            out << "\n";
        } else if (gnomonPipelineNodeWriter * writer_node = dynamic_cast<gnomonPipelineNodeWriter *>(node)) {
            out << "[" << node_name << "]" << "\n";
            out << "plugin_name = \""<< writer_node->algorithm << "\"\n";
            out << "path = \""<< writer_node->path << "\"\n";
            out << "\n";
        } else if (gnomonPipelineNodeAlgorithm * algorithm_node = dynamic_cast<gnomonPipelineNodeAlgorithm *>(node)) {
            out << "[" << node_name << "]" << "\n";
            out << "plugin_name = \""<< algorithm_node->algorithm << "\"\n";
            out << "    [" << node_name << ".parameters]\n";
            for (const auto& param : algorithm_node->parameters.keys()) {
                QVariant parameter = algorithm_node->parameters[param];
                bool int_status;
                parameter.toInt(&int_status);
                bool double_status;
                parameter.toDouble(&double_status);
                QString parameter_string = "";

                if (int_status | double_status) {
                     parameter_string = parameter.toString();
                } else if (parameter.canConvert<QString>()) {
                     parameter_string = "\"" + parameter.toString() + "\"";
                } else if (parameter.canConvert<QStringList>()) {
                    QStringList list = parameter.toStringList();
                    parameter_string = "[";
                    for (int i=0; i<list.size(); i++) {
                        if (i>0)
                            parameter_string += ", ";
                        parameter_string += "\"" + list[i] + "\"";
                    }
                    parameter_string += "]";
                } else if (parameter.canConvert<QList<QVariant>>()) {
                    QList<QVariant> list = parameter.toList();
                    parameter_string = "[";
                    for (int i=0; i<list.size(); i++) {
                        if (i>0)
                            parameter_string += ", ";
                        parameter_string += list[i].toString();
                    }
                    parameter_string += "]";
                }
                out << "    " << param << " = " << parameter_string << "\n";
            }
            out << "\n";
        }
    }

    file.close();
}

gnomonPipeline *gnomonPipeline::s_instance = nullptr;

//
// gnomonPipeline.cpp ends here