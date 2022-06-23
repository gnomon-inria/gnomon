#include "gnomonPipeline.h"

#include "gnomon"
#include "gnomonPipelineEdge.h"
#include "gnomonPipelineNode.h"
#include "gnomonPipelinePort.h"

#include "gnomonPipelineNodeAlgorithm.h"
#include "gnomonPipelineNodeReader.h"
#include "gnomonPipelineNodeTask.h"
#include "gnomonPipelineNodeMorphonet.h"
#include "gnomonPipelineNodeWriter.h"

#include <cmath>


// /////////////////////////////////////////////////////////////////
// gnomonPipelinePrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelinePrivate
{

public:
    QString name;
    QString description;

public:
    QStringList pipeline_node_names;
    QMap<QString, int> node_type_count;
    QMap<QString, gnomonPipelineNode *> pipeline_nodes;

    QMap<QString, QPointF> node_layout;

public:
    void clear(void);

public:
    QStringList sourceNodeNames(void);
    QStringList sinkNodeNames(void);
    QList<QStringList> scheduledNodeNameGroups(void);

public:
    QList<QList<double> > nodeDistances(QList<QPointF> node_positions);
    QList<QList<QVector2D> > nodeVectors(QList<QPointF> node_positions);
    void forceDrivenLayout(void);
};

void gnomonPipelinePrivate::clear(void)
{
    this->pipeline_node_names.clear();
    this->pipeline_nodes.clear();

    this->node_type_count.clear();

    this->node_layout.clear();
}

QStringList gnomonPipelinePrivate::sourceNodeNames(void)
{
    QStringList source_nodes;
    for (const auto& node_name: this->pipeline_node_names) {
        gnomonPipelineNode *node = this->pipeline_nodes[node_name];
        if (node->inputEdgeCount() == 0) {
            source_nodes.append(node_name);
        }
    }
    return source_nodes;
}

QStringList gnomonPipelinePrivate::sinkNodeNames(void)
{
    QStringList sink_nodes;
    for (const auto& node_name: this->pipeline_node_names) {
        gnomonPipelineNode *node = this->pipeline_nodes[node_name];
        if (node->outputEdgeCount() == 0) {
            sink_nodes.append(node_name);
        }
    }
    return sink_nodes;
}

QList<QStringList> gnomonPipelinePrivate::scheduledNodeNameGroups(void)
{
    QStringList source_nodes = this->sourceNodeNames();
    QList<QStringList> scheduled_node_groups;
    scheduled_node_groups.append(source_nodes);

    QStringList unscheduled_nodes = QStringList(this->pipeline_node_names);
    for (const auto& node_name : source_nodes) {
        unscheduled_nodes.removeAll(node_name);
    }

    while (unscheduled_nodes.size() > 0) {
        QStringList next_group_nodes;
        for (const auto& node_name : unscheduled_nodes) {
            bool node_next_group = true;
            gnomonPipelineNode *node = this->pipeline_nodes[node_name];
            for (auto edge : node->inputEdges()) {
                gnomonPipelineNode *linked_node = edge->source()->node();
                QString linked_node_name = this->pipeline_nodes.key(linked_node);
                if (unscheduled_nodes.contains(linked_node_name)) {
                    node_next_group = false;
                }
            }
            if (node_next_group) {
                next_group_nodes.append(node_name);
            }
        }
        scheduled_node_groups.append(next_group_nodes);
        for (const auto& node_name : next_group_nodes) {
            unscheduled_nodes.removeAll(node_name);
        }
    }

    return scheduled_node_groups;
}

void gnomonPipelinePrivate::forceDrivenLayout(void)
{
    QList<QPointF> node_positions;
    QList<QPair<int, int> > node_edge_indices;
    for (const auto& node_name : this->pipeline_node_names) {
        node_positions.append(this->pipeline_nodes[node_name]->position()/10);

        gnomonPipelineNode *node = this->pipeline_nodes[node_name];
        for (auto edge : node->inputEdges()) {
            int target_index = this->pipeline_node_names.indexOf(node_name);
            gnomonPipelineNode *source_node = edge->source()->node();
            QString source_node_name = this->pipeline_nodes.key(source_node);
            int source_index = this->pipeline_node_names.indexOf(source_node_name);
            node_edge_indices.append(QPair<int,int>(source_index,target_index));
        }
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


gnomonPipeline::gnomonPipeline(QObject *parent) : QObject(parent)
{
    d = new gnomonPipelinePrivate;
}

gnomonPipeline::~gnomonPipeline(void)
{
    delete d;
}

const QString& gnomonPipeline::name(void)
{
    return d->name;
}

const QString& gnomonPipeline::description(void)
{
    return d->description;
}

void gnomonPipeline::setName(const QString& name)
{
    if (name != d->name) {
        d->name = name;
        emit nameChanged();
    }
}

void gnomonPipeline::setDescription(const QString& desc)
{
    if (desc != d->description) {
        d->description = desc;
        emit descriptionChanged();
    }
}

QStringList gnomonPipeline::nodeNames(void)
{
    return d->pipeline_node_names;
}

void gnomonPipeline::clear(void)
{
    d->clear();
    emit pipelineCleared();
}

gnomonPipelineNode *gnomonPipeline::node(const QString& node_name)
{
    if (d->pipeline_node_names.contains(node_name)) {
        return d->pipeline_nodes[node_name];
    } else {
        return nullptr;
    }
}

void gnomonPipeline::addNode(gnomonPipelineNode *node)
{
    if (!d->node_type_count.contains(node->algorithmClass())) {
        d->node_type_count[node->algorithmClass()] = 1;
    } else {
        d->node_type_count[node->algorithmClass()] += 1;
    }

    QString node_name;
    if (!node->name().isEmpty() && !d->pipeline_node_names.contains(node->name())) {
        node_name = node->name();
    } else {
        node_name = node->algorithmClass() + QString::number(d->node_type_count[node->algorithmClass()]);
        node->setName(node_name);
    }
    d->pipeline_node_names.append(node_name);
    d->pipeline_nodes[node_name] = node;

    this->updateLayout();

    emit nodeAdded(node);
}

void gnomonPipeline::removeLastNode() 
{
    d->node_type_count[d->pipeline_nodes[d->pipeline_node_names.last()]->algorithmClass()] -= 1;
    d->pipeline_node_names.removeLast();
    d->pipeline_nodes.remove(d->pipeline_nodes.lastKey());
    emit lastNodeRemoved();
}


QStringList gnomonPipeline::scheduledNodeNames(bool recompute_form_indices)
{
    int form_index = 0;
    //int group_id = 0;
    QStringList scheduled_node_names;
    QList<QStringList> node_groups = d->scheduledNodeNameGroups();
    for (const auto& group_node_names : node_groups) {
        //qInfo() << "group: " << group_id++;
        for (const auto& node_name : group_node_names) {
            //qInfo() << "-- node name: " << node_name;
            if (recompute_form_indices) {
                auto node = d->pipeline_nodes[node_name];
                for (auto output : node->outputPorts()) {
                    for (auto edge : node->outputEdges()) {
                        if (edge->source() == output) {
                            edge->setFormIndex(form_index);
                        }
                    }
                    form_index++;
                }
            }
            scheduled_node_names.append(node_name);
        }
    }

    return scheduled_node_names;
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
    d->scheduledNodeNameGroups();

    QUrl q_url(url);
    QString path = q_url.toLocalFile();

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "can't open file " << path;
        return;
    }

    // 1 pipeline document
    QJsonObject pipeline_json;
    QString pipeline_name = d->name; //QFileInfo(path).baseName();
    pipeline_json.insert("type", "pipeline");
    pipeline_json.insert("gnomonVersion", GNOMON_VERSION);
    pipeline_json.insert("fileFormatVersion", "0.0.1");
    pipeline_json.insert("name", pipeline_name);
    pipeline_json.insert("description", d->description);

    QJsonArray inputs_json;  // input_name, node_name -> method
    QJsonArray outputs_json; // output_name, node_name -> method

    QJsonArray inputs_json_run; // "input": {"toto" : {"monnom": "/home/trcabel/Dev/naviscope/gnomon/gnomon-data/p58-t0_imgFus_down_interp_2x.inr.gz"}},

    for (const auto& node_name : d->pipeline_node_names) {
        auto node = d->pipeline_nodes[node_name];
        QJsonObject node_json = node->toJson();
        QJsonObject in = node_json["inputs"].toObject();
        for (auto edge : node->inputEdges()) {
            QString source_name = edge->source()->node()->name();
            QString from = source_name + " -> " + edge->source()->name();
            in.insert(edge->target()->name(), from);
        }
        node_json.insert("inputs", in);
        pipeline_json.insert(node->name(), node_json);

        auto *node_reader = dynamic_cast<gnomonPipelineNodeReader *>(d->pipeline_nodes[node_name]);
        if (node_reader) {
            //this is a nodeReader add to inputs
            QJsonObject input;
            // TODO : use form name = port label instead of node name?
            // QString reader_output_name = node_reader->outputPort(node_reader->outputPortsNames()[0])->label();
            // QString input_name = reader_output_name + "_path";
            QString input_name = node_reader->name() + "_path";
            input.insert(input_name, node_reader->name() + " -> path");
            inputs_json.append(input);

            QJsonObject input_run;
            input_run.insert(input_name, node_json["path"]);
            QJsonObject input_run_with_pipeline;
            input_run_with_pipeline.insert(pipeline_name, input_run);
            inputs_json_run.append(input_run_with_pipeline);
        }

        auto *node_writer = dynamic_cast<gnomonPipelineNodeWriter *>(d->pipeline_nodes[node_name]);
        if (node_writer) {
            //this is a nodeReader add to inputs
            QJsonObject output;
            // TODO : use form name = port label instead of node name?
            // QString writer_input_name = node_writer->inputPort(node_writer->inputPortsNames()[0])->label();
            // QString input_name = writer_input_name + "_path";
            QString output_name = node_writer->name() + "_path";
            output.insert(output_name ,node_name + " -> path");
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
    out << "import gnomon.core\n";
    out << "from gnomon.utils import load_plugin_group\n";
    out << "\n";
    out << "from gnomon.gnomonLuigi import AlgorithmPluginTask\n";
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
        QString class_name = QString(node_name).remove(QRegularExpression("[0-9]")) + "Task";
        class_name.replace(0, 1, class_name[0].toUpper());
        out << "    tasks[\"" << node_name <<"\"] = " << class_name << "(**config[\"" << node_name <<"\"])\n";
    }

    for (const auto& node_name: d->pipeline_node_names) {

        auto node = d->pipeline_nodes[node_name];
        auto node_json = node->toJson();
        for (auto edge : node->inputEdges()) {
            auto source_node = edge->source()->node();
            auto source_node_name = d->pipeline_nodes.key(source_node);
            out << "    tasks[\"" << node_name <<"\"].connect_input(tasks[\"" <<  source_node_name << "\"], ";
            out << "output_name=\"" << edge->source()->name() << "\", ";
            out << "input_name=\"" << edge->target()->name() << "\")\n";
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


void gnomonPipeline::readFromJson(const QString& url)
{
    QString path;
    const QUrl q_url(url);
    if (q_url.isLocalFile()) {
        path = QDir::toNativeSeparators(q_url.toLocalFile());
    } else {
        path = url;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "can't open file " << path;
        return;
    }

    QByteArray pipeline_json = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(pipeline_json);
    QJsonObject rootObj = doc.object();

    QMap<QPair<QString, QString>, QPair<QString, QString> > edges;

    d->name = rootObj.value("name").toString();
    d->description = rootObj.value("description").toString();

    for(auto k:rootObj.keys()) {
        QJsonObject node_json = rootObj.value(k).toObject();

        if (node_json.keys().contains("plugin_group")) {
            QString name = node_json.value("name").toString();
            QString algorithm_class = node_json.value("plugin_group").toString();
            QString algorithm_plugin = node_json.value("plugin_name").toString();
            if (algorithm_class.contains("Reader")) {
                QString path = node_json.value("path").toString();
                QStringList outputs;
                for (auto output_variant: node_json.value("outputs").toArray().toVariantList()) {
                    outputs.append(output_variant.toString());
                }
                gnomonPipelineNodeReader *node = new gnomonPipelineNodeReader(algorithm_class, algorithm_plugin, path, outputs);
                node->setName(name);
                this->addNode(node);
            } else if(algorithm_class.contains("morphonetCellImage")) {
                QJsonObject morphonet_data = node_json.value("morphonet_data").toObject();
                QStringList outputs;
                for (auto output_variant: node_json.value("outputs").toArray().toVariantList()) {
                    outputs.append(output_variant.toString());
                }
                gnomonPipelineNodeMorphonet *node =  new gnomonPipelineNodeMorphonet(outputs[0], morphonet_data);
                node->setName(name);
                this->addNode(node);
            } else if (algorithm_class.contains("Writer")) {
                QString path = node_json.value("path").toString();
                QStringList inputs = node_json.value("inputs").toObject().keys();
                gnomonPipelineNodeWriter *node = new gnomonPipelineNodeWriter(algorithm_class, algorithm_plugin, path, inputs);
                node->setName(name);
                this->addNode(node);
            } else if (algorithm_class == "task") {
                QStringList inputs = node_json.value("inputs").toObject().keys();
                QStringList outputs;
                for (auto output_variant: node_json.value("outputs").toArray().toVariantList()) {
                    outputs.append(output_variant.toString());
                }
                auto *node = new gnomonPipelineNodeTask(algorithm_plugin, inputs, outputs);
                node->setName(name);
                this->addNode(node);
            } else {
                QStringList inputs = node_json.value("inputs").toObject().keys();
                QStringList outputs;
                for (auto output_variant: node_json.value("outputs").toArray().toVariantList()) {
                    outputs.append(output_variant.toString());
                }
                QJsonObject parameters = node_json.value("parameters").toObject();
                gnomonPipelineNodeAlgorithm *node = new gnomonPipelineNodeAlgorithm(algorithm_class, algorithm_plugin, parameters, inputs, outputs);
                node->setName(name);
                this->addNode(node);
            }

            if (node_json.contains("inputs")) {
                QJsonObject inputs_json = node_json.value("inputs").toObject();
                for (const QString &input_name: inputs_json.keys()) {
                    QJsonValue in = inputs_json.value(input_name);
                    if (in != QJsonValue::Null) {
                        QStringList input_source = in.toString().split(" -> ");
                        edges[QPair<QString, QString>(name, input_name)] = QPair<QString, QString>(input_source[0], input_source[1]);
                    }
                }
            }
        }
    }

    for (QPair<QString, QString> target : edges.keys()) {
        QPair<QString, QString> source = edges[target];


        gnomonPipelineEdge *edge = new gnomonPipelineEdge();
        edge->setSource(d->pipeline_nodes[source.first]->outputPort(source.second));
        edge->setTarget(d->pipeline_nodes[target.first]->inputPort(target.second));
        edge->link();
    }
}

QList<QStringList> gnomonPipeline::scheduleGroups(void) {
    return d->scheduledNodeNameGroups();
}


//
// gnomonPipeline.cpp ends here
