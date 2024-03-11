#include "gnomonPipelineNodeWriter.h"

#include <gnomonProject>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"
#include "gnomonPythonPluginLoader.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriterPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeWriterPrivate {
public:
    QString path;
    QJsonObject metadata;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriter
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeWriter::gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> inputs, QJsonObject metadata) : gnomonPipelineNode(), dd(new gnomonPipelineNodeWriterPrivate)
{
    d->type = gnomonPipelineNode::NODE_WRITER;

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->path = path;

    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }

    dd->metadata = metadata;
    // FIXME: Calling availablePluginsFromGroup from a Python toplevel should not cause a crash
    /*auto plugins = availablePluginsFromGroup(algorithm_class);
    if(plugins.contains(algorithm)) {
        auto localMetadata = pluginMetadata(algorithm_class, algorithm);
        QMap<QString, QString>::key_value_iterator ptr;
        for(ptr = localMetadata.keyValueBegin(); ptr!=localMetadata.keyValueEnd(); ptr++) {
            dd->metadata.insert(ptr->first, ptr->second);
        }
    } else {
        qWarning() << Q_FUNC_INFO << algorithm_class << " doesn't have algorithm " << algorithm << " available algorithms are: " << plugins;
    }*/

}

gnomonPipelineNodeWriter::~gnomonPipelineNodeWriter(void)
{

}

QString gnomonPipelineNodeWriter::path(void) {
    return dd->path;
}

void gnomonPipelineNodeWriter::setPath(const QString& path) {
    dd->path = path;
}

QString gnomonPipelineNodeWriter::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "path = \""<< dd->path << "\"\n";
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeWriter::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();
    json.insert("path", dd->path);

    /*QJsonObject in;
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.insert(input_name, QJsonValue::Null);
    }
    json.insert("inputs", in);*/

    return json;
}

QString gnomonPipelineNodeWriter::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(d->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    path = luigi.Parameter()\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << d->algorithm_class << "\")\n";
    out<<"        self.writer = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = [";
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        if (it != d->input_ports.begin()) {
            out<<", ";
        }
        auto&& input_name = it.key();
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    out<<"        self.file_output_paths[\"savedFile\"] = self.path\n";
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        self.writer.setPath(self.path)\n";
    out<<"        inputs = self.algorithm_inputs()\n";

    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        QString setter_name = "set" + input_name;
        setter_name.replace(3, 1, setter_name[3].toUpper());
        out<<"        self.writer." << setter_name << "(inputs[\"" << input_name << "\"])\n";
    }
    out<<"        self.writer.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeWriter.cpp ends here