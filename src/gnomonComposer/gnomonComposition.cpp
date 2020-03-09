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

#include "gnomonComposition.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <dtkCore>
#include <dtkComposer>


// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeReader
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonComposerNodeReader : public dtkComposerSceneNodeComposite
{
public:
     gnomonComposerNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path);
    ~gnomonComposerNodeReader(void);

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    dtkComposerScenePort *output_port;

};

gnomonComposerNodeReader::gnomonComposerNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path) : dtkComposerSceneNodeComposite()
{
    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->output_port = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
    this->addOutputPort(output_port);
    this->layout();
}

gnomonComposerNodeReader::~gnomonComposerNodeReader(void)
{

}

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeAlgorithm
// /////////////////////////////////////////////////////////////////

class GNOMONCOMPOSER_EXPORT gnomonComposerNodeAlgorithm : public dtkComposerSceneNodeComposite
{
public:
     gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> inputs, QList<QString> outputs);
    ~gnomonComposerNodeAlgorithm(void);

public:
    QString algorithm_class;
    QString algorithm;
    QMap<QString, QVariant> parameters;

    QMap<QString, dtkComposerScenePort *> input_ports;
    QMap<QString, dtkComposerScenePort *> output_ports;
};

gnomonComposerNodeAlgorithm::gnomonComposerNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> inputs,  QList<QString> outputs) : dtkComposerSceneNodeComposite()
{
    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->parameters = parameters;

    this->setTitle(this->algorithm_class);

    for (const auto& input : inputs) {
        this->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(this->input_ports[input]);
    }
    for (const auto& output : outputs) {
        this->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(this->output_ports[output]);
    }
    this->layout();
}

gnomonComposerNodeAlgorithm::~gnomonComposerNodeAlgorithm(void)
{

}


// /////////////////////////////////////////////////////////////////
// gnomonCompositionPrivate
// /////////////////////////////////////////////////////////////////

class gnomonCompositionPrivate
{
public:
    QStringList pipeline_node_names;
    QMap<QString, int> node_type_count;
    QMap<QString, dtkComposerSceneNodeComposite *> pipeline_nodes;

    QMap<gnomonAbstractDynamicForm *, gnomonComposerNodeReader *> reader_nodes;
    QMap<gnomonAbstractDynamicForm *, gnomonComposerNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;

    QMap<gnomonComposerNodeAlgorithm *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;

    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;
};



// /////////////////////////////////////////////////////////////////
// gnomonComposition
// /////////////////////////////////////////////////////////////////

gnomonComposition *gnomonComposition::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonComposition;

    return s_instance;
}


gnomonComposition::gnomonComposition(void)
{
    d = new gnomonCompositionPrivate;
}

gnomonComposition::~gnomonComposition(void)
{
    delete d;
}

void gnomonComposition::addReader(gnomonAbstractReaderCommand *command)
{
    qDebug() << Q_FUNC_INFO << command->factoryName() << "[" << command->algorithmName() << "] : "<<command->path();
    QMap<QString, gnomonAbstractDynamicForm *> forms = command->outputs();

    for (const auto& form_name : forms.keys())
    {
        gnomonAbstractDynamicForm *form = forms[form_name];
        if (gnomonAbstractDynamicForm *clone = d->form_clones.key(form,nullptr)) {
            form = clone;
        }
        d->reader_nodes[form] = new gnomonComposerNodeReader(command->factoryName(),command->algorithmName(),command->path());
    }
}

void gnomonComposition::addAlgorithm(QMap<QString, gnomonAbstractDynamicForm *> input_forms, QMap<QString, gnomonAbstractDynamicForm *> output_forms, const QString& algorithm_class, const QString& algorithm, QMap<QString, gnomonCoreParameter *> parameters)
{
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

    gnomonComposerNodeAlgorithm *node = new gnomonComposerNodeAlgorithm(algorithm_class,algorithm,parameter_values,input_forms.keys(),output_forms.keys());

    d->node_input_forms[node] = input_forms;

    for (const auto& output : output_forms.keys()) {
        d->algorithm_nodes[output_forms[output]] = node;
        d->algorithm_output[output_forms[output]] = output;
    }
}

void gnomonComposition::addForm(gnomonAbstractDynamicForm *form)
{
    if (d->reader_nodes.contains(form)) {
        gnomonComposerNodeReader *node = d->reader_nodes[form];
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
        gnomonComposerNodeAlgorithm *node = d->algorithm_nodes[form];
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
                node->addEdge(edge);
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

void gnomonComposition::addClonedForm(gnomonAbstractDynamicForm *form, gnomonAbstractDynamicForm *clone)
{
    qDebug()<<Q_FUNC_INFO<<clone<<"->"<<form;
    d->form_clones[clone] = form;
}

void gnomonComposition::exportToToml(const QString& path)
{
    Q_ASSERT(path.endsWith(".toml"));

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (const auto& node_name : d->pipeline_node_names) {
        dtkComposerSceneNodeComposite *node = d->pipeline_nodes[node_name];
        if (gnomonComposerNodeReader * reader_node = dynamic_cast<gnomonComposerNodeReader *>(node)) {
            out << "[" << node_name << "]" << "\n";
            out << "plugin_name = \""<< reader_node->algorithm << "\"\n";
            out << "path = \""<< reader_node->path << "\"\n";
            out << "\n";
        } else if (gnomonComposerNodeAlgorithm * algorithm_node = dynamic_cast<gnomonComposerNodeAlgorithm *>(node)) {
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

gnomonComposition *gnomonComposition::s_instance = nullptr;

//
// gnomonComposition.cpp ends here