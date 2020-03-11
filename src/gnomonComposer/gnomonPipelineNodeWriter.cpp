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

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriter
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeWriter::gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> inputs) : gnomonPipelineNode()
{
    d->color = QColor(255, 255, 0);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    for (const auto& input : inputs) {
        this->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(this->input_ports[input]);
    }
    this->layout();
}

gnomonPipelineNodeWriter::~gnomonPipelineNodeWriter(void)
{

}

QString gnomonPipelineNodeWriter::toToml(const QString& node_name)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << node_name << "]" << "\n";
    out << "plugin_name = \""<< this->algorithm << "\"\n";
    out << "path = \""<< this->path << "\"\n";
    out << "\n";
    return node_string;
}

QString gnomonPipelineNodeWriter::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(this->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    path = luigi.Parameter()\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << this->algorithm_class << "\")\n";
    out<<"        self.writer = gnomoncore." << this->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = [";
    for (int i=0; i<this->input_ports.size(); i++) {
        if (i>0) {
            out<<", ";
        }
        QString input_name = this->input_ports.keys()[i];
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    out<<"        self.file_output_paths[\"savedFile\"] = self.path\n";
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        self.writer.setPath(self.path)\n";
    out<<"        inputs = self.algorithm_inputs()\n";
    for (const auto& input_name : this->input_ports.keys()) {
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