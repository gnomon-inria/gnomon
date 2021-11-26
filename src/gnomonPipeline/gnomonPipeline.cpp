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

#include "gnomon"
#include "gnomonPipelineNode.h"
#include "gnomonPipelineEdge.h"

#include "gnomonPipelineNodeAdapter.h"
#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeConstructor.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeWriter.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <dtkCore>
// #include <dtkComposer>

#include <cmath>


// /////////////////////////////////////////////////////////////////
// gnomonPipelinePrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelinePrivate
{
public:
    gnomonPipeline *q;

public:
    QStringList pipeline_node_names;
    QMap<QString, int> node_type_count;
    QMap<QString, gnomonPipelineNode *> pipeline_nodes;
    QMap< QPair<QString, QString>, QPair<QString, QString> > pipeline_edges;

    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeReader *> reader_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> reader_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeWriter *> writer_nodes;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAdapter *> adapter_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> adapter_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeAlgorithm *> algorithm_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> algorithm_output;
    QMap<gnomonAbstractDynamicForm *, gnomonPipelineNodeConstructor *> constructor_nodes;
    QMap<gnomonAbstractDynamicForm *, QString> constructor_output;

    QMap<gnomonPipelineNode *, QMap<QString, gnomonAbstractDynamicForm *> > node_input_forms;

    QMap<gnomonAbstractDynamicForm *, gnomonAbstractDynamicForm *> form_clones;

    QMap<QString, QPointF> node_layout;

public:
    void linkNodeInputs(gnomonPipelineNode *node);
    QVariantMap parameterVariantValues(const dtkCoreParameters&parameters);

public:
    QStringList sourceNodeNames(void);
    QStringList sinkNodeNames(void);

public:
    bool hasNode(gnomonPipelineNode *);

public:
    QList<QList<double> > nodeDistances(QList<QPointF> node_positions);
    QList<QList<QVector2D> > nodeVectors(QList<QPointF> node_positions);
    void forceDrivenLayout(void);
};


void gnomonPipelinePrivate::linkNodeInputs(gnomonPipelineNode *node)
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
            QPair<QString, QString> edge_source;
            QPair<QString, QString> edge_target;
            if (this->reader_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->reader_nodes[input_form]->outputPorts()[this->reader_output[input_form]]);
                edge_source.first = this->pipeline_nodes.key(this->reader_nodes[input_form]);
                edge_source.second = this->reader_output[input_form];
            } else if (this->constructor_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->constructor_nodes[input_form]->outputPorts()[this->constructor_output[input_form]]);
                edge_source.first = this->pipeline_nodes.key(this->constructor_nodes[input_form]);
                edge_source.second = this->constructor_output[input_form];
            } else if (this->adapter_nodes.contains(input_form)) {
                q->addAdaptedForm(input_form);
                edge = new gnomonPipelineEdge();
                edge->setSource(this->adapter_nodes[input_form]->outputPorts()[this->adapter_output[input_form]]);
                edge_source.first = this->pipeline_nodes.key(this->adapter_nodes[input_form]);
                edge_source.second = this->adapter_output[input_form];
            } else if (this->algorithm_nodes.contains(input_form)) {
                edge = new gnomonPipelineEdge();
                edge->setSource(this->algorithm_nodes[input_form]->outputPorts()[this->algorithm_output[input_form]]);
                edge_source.first = this->pipeline_nodes.key(this->algorithm_nodes[input_form]);
                edge_source.second = this->algorithm_output[input_form];
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
                //node->addInputEdge(edge);
                edge_target.first = this->pipeline_nodes.key(node);
                edge_target.second = input;
                this->pipeline_edges[edge_target] = edge_source;
            }
        }
    }
}

QVariantMap gnomonPipelinePrivate::parameterVariantValues(const dtkCoreParameters&parameters)
{
    QVariantMap parameter_values;
    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        auto&& parameter_name = it.key();
        auto&& param = it.value();
        parameter_values[parameter_name] = param->variant();
    }

    return parameter_values;
}

QStringList gnomonPipelinePrivate::sourceNodeNames(void)
{
    QMap<QString, bool> node_source;
    for (const auto& node_name: this->pipeline_node_names) {
        node_source[node_name] = true;
    }
    for (auto it = this->pipeline_edges.begin(); it != this->pipeline_edges.end(); ++it) {
        auto&& edge_target = it.key();
        node_source[edge_target.first] = false;
    }
    QStringList source_nodes;
    for (const auto& node_name: this->pipeline_node_names) {
        if (node_source[node_name]) {
            source_nodes.append(node_name);
        }
    }
    return source_nodes;
}

QStringList gnomonPipelinePrivate::sinkNodeNames(void)
{
    QMap<QString, bool> node_sink;
    for (const auto& node_name: this->pipeline_node_names) {
        node_sink[node_name] = true;
    }
    for (auto it = this->pipeline_edges.begin(); it != this->pipeline_edges.end(); ++it) {
        auto&& edge_source = it.value();
        node_sink[edge_source.first] = false;
    }
    QStringList sink_nodes;
    for (const auto& node_name: this->pipeline_node_names) {
        if (node_sink[node_name]) {
            sink_nodes.append(node_name);
        }
    }
    return sink_nodes;
}

bool gnomonPipelinePrivate::hasNode(gnomonPipelineNode *node)
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

void gnomonPipelinePrivate::forceDrivenLayout(void)
{
    QList<QPointF> node_positions;
    for (const auto& node_name : this->pipeline_node_names) {
        node_positions.append(this->pipeline_nodes[node_name]->position()/10);
    }

    QList<QPair<int, int> > node_edge_indices;

    for (auto it = this->pipeline_edges.begin(); it != this->pipeline_edges.end(); ++it) {
        auto&& edge_target = it.key();
        int target_index = this->pipeline_node_names.indexOf(edge_target.first);
        int source_index = this->pipeline_node_names.indexOf(this->pipeline_edges[edge_target].first);
        node_edge_indices.append(QPair<int,int>(source_index,target_index));
    }

    int iterations = 1000;
    double target_distance = 200;
    double max_deformation = 5;

    QMap<QString, double> force_weights;
    force_weights["node_repulsion"] = 1.;
    force_weights["edge_attraction"] = 1.;
    force_weights["source_left_drift"] = 0.5;
    force_weights["sink_right_drift"] = 0.5;
    force_weights["edge_horizontality"] = 1.;
    force_weights["vertical_centering"] = 1.;

    QList<QList<double> > node_distances;
    QList<QList<QVector2D> > node_vectors;

    double target_radius = target_distance*pow(node_positions.size()-1,0.5)+1e-7;

    for (int n=0; n<node_positions.size(); n++) {
        node_positions[n] += QPointF(rand()/(RAND_MAX+1.), rand()/(RAND_MAX+1.));
    }

    for (int iteration=0; iteration<iterations; iteration++)
    {
        node_distances = this->nodeDistances(node_positions);
        node_vectors = this->nodeVectors(node_positions);

        QMap<QString, QList<QVector2D> > node_forces;
        node_forces["node_repulsion"] = QList<QVector2D>();
        for (int n1=0; n1<node_positions.size(); n1++) {
            QVector2D node_force = QVector2D(0,0);
            for (int n2=0; n2<node_positions.size(); n2++) {
                node_force += pow(target_distance,2)*node_vectors[n1][n2]/(pow(node_distances[n1][n2],2)+1e-7);
            }
            node_forces["node_repulsion"].append(node_force);
        }

        node_forces["edge_attraction"] = QList<QVector2D>();
        for (int n=0; n<node_positions.size(); n++) {
            QVector2D node_force = QVector2D(0,0);
            node_forces["edge_attraction"].append(node_force);
        }
        for (const auto& edge_indices : node_edge_indices)
        {
            int n1 = edge_indices.first;
            int n2 = edge_indices.second;
            node_forces["edge_attraction"][n1] += (target_distance-node_distances[n1][n2])*node_vectors[n1][n2]/target_distance;
            node_forces["edge_attraction"][n2] += (target_distance-node_distances[n2][n1])*node_vectors[n2][n1]/target_distance;
        }

        node_forces["source_left_drift"] = QList<QVector2D>();
        for (int n=0; n<node_positions.size(); n++) {
            QVector2D node_force = QVector2D(0,0);
            node_forces["source_left_drift"].append(node_force);
        }
        for (const auto& node_name : this->sourceNodeNames())
        {
            int n = this->pipeline_node_names.indexOf(node_name);
            double left_x = -target_radius;
            double x_drift = left_x - node_positions[n].x();
            if (x_drift < 0)
            {
                node_forces["source_left_drift"][n] = QVector2D(x_drift*abs(x_drift)/pow(target_distance,2),0);
            }
        }

        node_forces["sink_right_drift"] = QList<QVector2D>();
        for (int n=0; n<node_positions.size(); n++) {
            QVector2D node_force = QVector2D(0,0);
            node_forces["sink_right_drift"].append(node_force);
        }
        for (const auto& node_name : this->sinkNodeNames())
        {
            int n = this->pipeline_node_names.indexOf(node_name);
            double right_x = target_radius;
            double x_drift = right_x - node_positions[n].x();
            if (x_drift > 0)
            {
                node_forces["sink_right_drift"][n] = QVector2D(x_drift*abs(x_drift)/pow(target_distance,2),0);
            }
        }

        node_forces["vertical_centering"] = QList<QVector2D>();
        for (int n=0; n<node_positions.size(); n++) {
            double node_y = node_positions[n].y();
            QVector2D node_force = QVector2D(0,-node_y*abs(node_y)/pow(target_radius,2));
            node_forces["vertical_centering"].append(node_force);
        }

        node_forces["edge_horizontality"] = QList<QVector2D>();
        for (int n=0; n<node_positions.size(); n++) {
            QVector2D node_force = QVector2D(0,0);
            node_forces["edge_horizontality"].append(node_force);
        }
        for (const auto& edge_indices : node_edge_indices)
        {
            int n1 = edge_indices.first;
            int n2 = edge_indices.second;
            double edge_sinus = node_vectors[n1][n2].y()/node_vectors[n1][n2].length();
            node_forces["edge_horizontality"][n1] += QVector2D(-abs(edge_sinus),-edge_sinus);
            node_forces["edge_horizontality"][n2] += QVector2D(abs(edge_sinus),edge_sinus);
        }

        QList<QVector2D> node_force;
        for (int n=0; n<node_positions.size(); n++) {
            QVector2D force = QVector2D(0,0);
            for (auto it = force_weights.begin(); it != force_weights.end(); ++it) {
                auto&& force_name = it.key();
                force += force_weights[force_name]*node_forces[force_name][n];
            }
            if (force.length() > max_deformation) {
                force *= max_deformation/force.length();
            }
            node_force.append(force);
        }

        for (int n=0; n<node_positions.size(); n++) {
            node_positions[n] += node_force[n].toPointF();
        }
    }

    QPointF center = QPointF(0,0);
    for (int n=0; n<node_positions.size(); n++) {
        center += node_positions[n];
    }
    center /= node_positions.size();

    for (int n=0; n<node_positions.size(); n++) {
        gnomonPipelineNode *node = this->pipeline_nodes[this->pipeline_node_names[n]];
        node->setPosition(node_positions[n]-center);
    }

    /*for (int n=0; n<node_positions.size(); n++) {
        gnomonPipelineNode *node = this->pipeline_nodes[this->pipeline_node_names[n]];
        for (const auto& edge : node->inputEdges()) {
            edge->adjust();
        }
    }*/
}

QList<QList<double> > gnomonPipelinePrivate::nodeDistances(QList<QPointF> node_positions)
{
    QList<QList<double> > node_distances;
    for (int n1=0; n1<node_positions.size(); n1++) {
        QList<double> n1_distances;
        node_distances.append(n1_distances);
        for (int n2=0; n2<node_positions.size(); n2++) {
            if (n2>n1) {
                node_distances[n1].append(QVector2D(node_positions[n1] - node_positions[n2]).length());
            } else if (n2==n1) {
                node_distances[n1].append(0);
            } else {
                node_distances[n1].append(node_distances[n2][n1]);
            }
        }
    }
    return node_distances;
}

QList<QList<QVector2D> > gnomonPipelinePrivate::nodeVectors(QList<QPointF> node_positions)
{
    QList<QList<QVector2D> > node_vectors;
    for (int n1=0; n1<node_positions.size(); n1++) {
        QList<QVector2D> n1_vectors;
        node_vectors.append(n1_vectors);
        for (int n2=0; n2<node_positions.size(); n2++) {
            if (n2>n1) {
                node_vectors[n1].append(QVector2D(node_positions[n1] - node_positions[n2]));
                node_vectors[n1][n2] = node_vectors[n1][n2]/(node_vectors[n1][n2].length() + 1e-7);
            } else if (n2==n1) {
                node_vectors[n1].append(QVector2D(0,0));
            } else {
                node_vectors[n1].append(-node_vectors[n2][n1]);
            }
        }
    }
    return node_vectors;
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
    d->q = this;
}

gnomonPipeline::~gnomonPipeline(void)
{
    delete d;
}

const QStringList& gnomonPipeline::nodeNames(void)
{
    return d->pipeline_node_names;
}

gnomonPipelineNode *gnomonPipeline::node(const QString& node_name)
{
    if (d->pipeline_node_names.contains(node_name)) {
        return d->pipeline_nodes[node_name];
    } else {
        return nullptr;
    }
}

void gnomonPipeline::addReader(gnomonAbstractReaderCommand *command)
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

void gnomonPipeline::addWriter(gnomonAbstractWriterCommand *command)
{
    QMap<QString, gnomonAbstractDynamicForm *> input_forms = command->inputs();

    gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(command->factoryName(),command->algorithmName(),command->path(),input_forms.keys());

    QString node_name = node->algorithmClass();
    if (!d->node_type_count.contains(node->algorithmClass())) {
        d->node_type_count[node->algorithmClass()] = 1;
    } else {
        node_name += QString::number(d->node_type_count[node->algorithmClass()]);
        d->node_type_count[node->algorithmClass()] += 1;
    }
    d->pipeline_node_names.append(node_name);
    node->setName(node_name);
    d->pipeline_nodes[node_name] = node;

    d->node_input_forms[node] = input_forms;

    d->linkNodeInputs(node);

    this->updateLayout();

    emit nodeAdded(node);
}

void gnomonPipeline::addAdapter(gnomonAbstractAdapterCommand *command)
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

void gnomonPipeline::addAlgorithm(gnomonAbstractCommand *command)
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


void gnomonPipeline::addConstructor(gnomonAbstractConstructorCommand *command)
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

void gnomonPipeline::addAdaptedForm(gnomonAbstractDynamicForm *form)
{
    if (d->adapter_nodes.contains(form))
    {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];
        if (!d->pipeline_nodes.values().contains(node)) {
            QString node_name = node->algorithmClass();
            if (!d->node_type_count.contains(node->algorithmClass())) {
                d->node_type_count[node->algorithmClass()] = 1;
            } else {
                node_name += QString::number(d->node_type_count[node->algorithmClass()]);
                d->node_type_count[node->algorithmClass()] += 1;
            }
            d->pipeline_node_names.append(node_name);
            node->setName(node_name);
            d->pipeline_nodes[node_name] = node;
            d->linkNodeInputs(node);
            this->updateLayout();
            emit nodeAdded(node);
        }
    }
}

void gnomonPipeline::addForm(gnomonAbstractDynamicForm *form)
{
    qDebug()<<Q_FUNC_INFO<<form<<d->algorithm_nodes.contains(form);
    if (d->reader_nodes.contains(form)) {
        gnomonPipelineNodeReader *node = d->reader_nodes[form];

        if (!d->hasNode(node))
        {
            QString node_name = node->algorithmClass();
            if (!d->node_type_count.contains(node->algorithmClass())) {
                d->node_type_count[node->algorithmClass()] = 1;
            } else {
                node_name += QString::number(d->node_type_count[node->algorithmClass()]);
                d->node_type_count[node->algorithmClass()] += 1;
            }
            d->pipeline_node_names.append(node_name);
            node->setName(node_name);
            qDebug()<<Q_FUNC_INFO<<node->name();
            d->pipeline_nodes[node_name] = node;

            this->updateLayout();

            emit nodeAdded(node);
        }
    } else if (d->adapter_nodes.contains(form)) {
        gnomonPipelineNodeAdapter *node = d->adapter_nodes[form];

        if (!d->hasNode(node))
        {
            QString node_name = node->algorithmClass();
            if (!d->node_type_count.contains(node->algorithmClass())) {
                d->node_type_count[node->algorithmClass()] = 1;
            } else {
                node_name += QString::number(d->node_type_count[node->algorithmClass()]);
                d->node_type_count[node->algorithmClass()] += 1;
            }
            d->pipeline_node_names.append(node_name);
            node->setName(node_name);
            d->pipeline_nodes[node_name] = node;

            d->linkNodeInputs(node);
            this->updateLayout();

            emit nodeAdded(node);
        }
    } else if (d->constructor_nodes.contains(form)) {
        gnomonPipelineNodeConstructor *node = d->constructor_nodes[form];

        if (!d->hasNode(node))
        {
            QString node_name = node->algorithmClass();
            if (!d->node_type_count.contains(node->algorithmClass())) {
                d->node_type_count[node->algorithmClass()] = 1;
            } else {
                node_name += QString::number(d->node_type_count[node->algorithmClass()]);
                d->node_type_count[node->algorithmClass()] += 1;
            }
            d->pipeline_node_names.append(node_name);
            node->setName(node_name);
            d->pipeline_nodes[node_name] = node;

            this->updateLayout();

            emit nodeAdded(node);
        }
    } else if (d->algorithm_nodes.contains(form)) {
        gnomonPipelineNodeAlgorithm *node = d->algorithm_nodes[form];

        if (!d->hasNode(node))
        {
            QString node_name = node->algorithmClass();
            if (!d->node_type_count.contains(node->algorithmClass())) {
                d->node_type_count[node->algorithmClass()] = 1;
            } else {
                node_name += QString::number(d->node_type_count[node->algorithmClass()]);
                d->node_type_count[node->algorithmClass()] += 1;
            }
            d->pipeline_node_names.append(node_name);
            node->setName(node_name);
            d->pipeline_nodes[node_name] = node;

            d->linkNodeInputs(node);
            this->updateLayout();

            emit nodeAdded(node);
        }
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
        out << d->pipeline_nodes[node_name]->toToml();
    }
    file.close();
}

void gnomonPipeline::exportToJson(const QString& url)
{
    QUrl q_url(url);
    QString path = q_url.toLocalFile();

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "can't open file " << path;
        return;
    }

    // 1 pipeline document
    QJsonObject pipeline_json;
    QString pipeline_name = QFileInfo(path).baseName();
    pipeline_json.insert("type", "pipeline");
    pipeline_json.insert("gnomonVersion", GNOMON_VERSION);
    pipeline_json.insert("fileFormatVersion", "0.0.1");
    pipeline_json.insert("name", pipeline_name);
    pipeline_json.insert("description", "TODO");

    QJsonArray inputs_json;  // input_name, node_name -> method
    QJsonArray outputs_json; // output_name, node_name -> method

    QJsonArray inputs_json_run; // "input": {"toto" : {"monnom": "/home/trcabel/Dev/naviscope/gnomon/gnomon-data/p58-t0_imgFus_down_interp_2x.inr.gz"}},


    for (const auto& node_name : d->pipeline_node_names) {
        auto node_json = d->pipeline_nodes[node_name]->toJson();
        for (auto it = d->pipeline_edges.begin(); it != d->pipeline_edges.end(); ++it) {
            auto&& edge_target = it.key();
            if (edge_target.first == node_name) {
                QString from = d->pipeline_edges[edge_target].first + " -> " + d->pipeline_edges[edge_target].second;
                node_json.insert(edge_target.second, from);
            }
        }
        pipeline_json.insert(node_name, node_json);

        auto *node_reader = dynamic_cast<gnomonPipelineNodeReader *>(d->pipeline_nodes[node_name]);
        if (node_reader) {
            //this is a nodeReader add to inputs
            QJsonObject input;
            QString input_name = "my_input_" + QString::number(inputs_json.count());
            input.insert(input_name, node_name + " -> path");
            inputs_json.append(input);

            QJsonObject input_run;
            input_run.insert(input_name, node_json["path"]);
            QJsonObject input_run_with_pipeline;
            input_run_with_pipeline.insert(pipeline_name, input_run);
            inputs_json_run.append(input_run_with_pipeline);
        }

        // TODO 1 pipeline for each output????
        auto *node_writer = dynamic_cast<gnomonPipelineNodeWriter *>(d->pipeline_nodes[node_name]);
        if (node_writer) {
            //this is a nodeReader add to inputs
            QJsonObject output;
            output.insert("my_output" ,node_name + " -> path");
            outputs_json.append(output); // "output": {"anOutput": "cellImageQuantification -> cellImage"},
        }
    }

    pipeline_json.insert("input", inputs_json);
    pipeline_json.insert("output", outputs_json);

    QJsonDocument pipeline_doc(pipeline_json);
    file.write(pipeline_doc.toJson());
    file.close();

    //QJsonArray pipeline_ids = { gnomonDataDriver::instance()->insert(pipeline_doc.toJson()) };
    QJsonArray pipeline_ids = { path };

    // 2 run document
    QString path_run = path;
    path_run.remove(".json");
    path_run += "_run.json";

    QFile file_run(path_run);
    if (!file_run.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QJsonObject run_json;
    run_json.insert("pipelines", pipeline_ids);
    run_json.insert("parameters", "TODO"); // "parameters": { "toto": {"cellImageFromImage" : {"h_min": 3}}},

    run_json.insert("intermediateResults", "TODO"); // "intermediateResults":  {"toto" : {"cellImageFromImage -> output": "/asdasdasd/dsadad/"} } ,


    run_json.insert("input", inputs_json_run); // "input": {"toto" : {"monnom": "/home/trcabel/Dev/naviscope/gnomon/gnomon-data/p58-t0_imgFus_down_interp_2x.inr.gz"}},

    run_json.insert("output", "TODO"); // "output": ["/home/trcabel/aaa"]
    run_json.insert("type", "run");
    run_json.insert("gnomonVersion", GNOMON_VERSION);
    run_json.insert("fileFormatVersion", "0.0.1");
    run_json.insert("name", QFileInfo(path).baseName() + "_run");

    QJsonDocument run_doc(run_json);
    file_run.write(run_doc.toJson());
    file_run.close();

    //commented for now. will put it back when we do databases
    //gnomonDataDriver::instance()->insert(run_doc.toJson());
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
    out << "import os\n";
    out << "\n";
    out << "import luigi\n";
    out << "\n";
    out << "import gnomoncore\n";
    out << "from gnomon_utils import load_plugin_group\n";
    out << "\n";
    out << "from gnomon_utils.gnomonLuigi import AlgorithmPluginTask\n";
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
    out << "    parser.add_argument(\"-f\", \"--force\", ";
    out << "help=\"Force pipeline to overwrite existing output files\", ";
    out << "action=\"store_true\", default=False)\n";
    out << "    args = parser.parse_args()\n";
    out << "\n";
    out << "    config = luigi.configuration.LuigiTomlParser().read(config_paths=[args.config_filename])\n";
    out << "\n";
    out << "    tasks = {}\n";

    for (const auto& node_name: d->pipeline_node_names) {
        QString class_name = QString(node_name).remove(QRegExp("[0-9]")) + "Task";
        class_name.replace(0, 1, class_name[0].toUpper());
        out << "    tasks[\"" << node_name <<"\"] = " << class_name << "(**config[\"" << node_name <<"\"])\n";
    }

    for (const auto& node_name: d->pipeline_node_names) {
        for (auto it = d->pipeline_edges.begin(); it != d->pipeline_edges.end(); ++it) {
            auto&& edge_target = it.key();
            if (edge_target.first == node_name) {
                out << "    tasks[\"" << node_name <<"\"].connect_input(tasks[\"" << d->pipeline_edges[edge_target].first << "\"], ";
                out << "output_name=\"" << d->pipeline_edges[edge_target].second << "\", ";
                out << "input_name=\"" << edge_target.second << "\")\n";
            }
        }
        out << "\n";
    }

    out << "    sink_tasks = []\n";

    for (const auto& node_name: d->sinkNodeNames()) {
        out << "    sink_tasks.append(tasks[\"" << node_name << "\"])\n";
    }
    out << "\n";

    out << "    if args.force:\n";
    out << "        for task in sink_tasks:\n";
    out << "            for output_name in task.file_output_paths.keys():\n";
    out << "                if os.path.exists(task.file_output_paths[output_name]):\n";
    out << "                    os.remove(task.file_output_paths[output_name])\n";
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

void gnomonPipeline::updateLayout(void)
{
    d->forceDrivenLayout();
}


gnomonPipeline *gnomonPipeline::s_instance = nullptr;

//
// gnomonPipeline.cpp ends here