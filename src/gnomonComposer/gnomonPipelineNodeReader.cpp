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

#include "gnomonPipelineNodeReader.h"

#include "gnomonPipelineNode_p.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeReaderPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeReaderPrivate {
public:
    QString path;
    QMap<QString, dtkComposerScenePort *> output_ports;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeReader
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeReader::gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> outputs) : gnomonPipelineNode(), dd(new gnomonPipelineNodeReaderPrivate)
{
    d->color = QColor(111, 153, 69);

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->path = path;


    for (const auto& output : outputs) {
        dd->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(dd->output_ports[output]);
    }
    this->layout();
}

gnomonPipelineNodeReader::~gnomonPipelineNodeReader(void)
{

}

const QMap<QString, dtkComposerScenePort *>& gnomonPipelineNodeReader::outputPorts(void)
{
    return dd->output_ports;
}

QString gnomonPipelineNodeReader::toToml(const QString& node_name)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << node_name << "]" << "\n";
    out << "task_name = \""<< node_name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "path = \""<< dd->path << "\"\n";
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeReader::toJson(const QString& node_name)
{
    QJsonObject json;
    json.insert("name", node_name);
    json.insert("plugin_name", d->algorithm);
    json.insert("path", dd->path);

    QJsonArray out;
    for (auto it = dd->output_ports.begin(); it != dd->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out.append(output_name);
    }
    json.insert("outputs", out);

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
    for (auto it = dd->output_ports.begin(); it != dd->output_ports.end(); ++it) {
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