#include "gnomonPipelineNodeReader.h"

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"
#include "gnomonPythonPluginLoader"
#include "gnomonPythonPluginLoader.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeReaderPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeReaderPrivate {
public:
    QString path;
    QJsonObject metadata;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeReader
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeReader::gnomonPipelineNodeReader(const QString &algorithm_class, const QString &algorithm,
                                                   const QString &path, QList<QString> outputs,
                                                   QJsonObject metadata) : gnomonPipelineNode(), dd(new gnomonPipelineNodeReaderPrivate)
{
    d->type = gnomonPipelineNode::NODE_READER;

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;
    dd->path = path;

    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    dd->metadata = metadata;
    auto plugins = availablePluginsFromGroup(algorithm_class);
    if(plugins.contains(algorithm)) {
        auto localMetadata = pluginMetadata(algorithm_class, algorithm);
        QMap<QString, QString>::key_value_iterator ptr;
        for(ptr = localMetadata.keyValueBegin(); ptr!=localMetadata.keyValueEnd(); ptr++) {
            dd->metadata.insert(ptr->first, ptr->second);
        }
    } else {
        qWarning() << Q_FUNC_INFO << algorithm_class << " doesn't have algorithm " << algorithm << " available algorithms are: " << plugins;
    }
}

gnomonPipelineNodeReader::~gnomonPipelineNodeReader(void)
{

}

QString gnomonPipelineNodeReader::path(void) {
    return dd->path;
}

QString gnomonPipelineNodeReader::toToml(void)
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

const QJsonObject gnomonPipelineNodeReader::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();
    json.insert("path", dd->path);

    return json;
}

QString gnomonPipelineNodeReader::toLuigiClass(void)
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
    out<<"        self.reader = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = []\n";
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.reader." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        self.reader.setPath(self.path)\n";
    out<<"        self.reader.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeReader.cpp ends here