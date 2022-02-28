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

#include "gnomonPipelineNodeWriter.h"

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriterPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeWriterPrivate {
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriter
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeWriter::gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> inputs) : gnomonPipelineNode(), dd(new gnomonPipelineNodeWriterPrivate)
{
    d->color = QColor(153, 153, 69);

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->path = path;

    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    // this->layout()();
}

gnomonPipelineNodeWriter::~gnomonPipelineNodeWriter(void)
{

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

    QJsonArray in;
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.append(input_name);
    }
    json.insert("input", in);

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