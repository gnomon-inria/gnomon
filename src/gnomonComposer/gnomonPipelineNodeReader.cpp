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
// gnomonPipelineNodeReader
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeReader::gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> outputs) : gnomonPipelineNode()
{
    d->color = QColor(170, 255, 0);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    for (const auto& output : outputs) {
        this->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(this->output_ports[output]);
    }
    this->layout();
}

gnomonPipelineNodeReader::~gnomonPipelineNodeReader(void)
{

}

QString gnomonPipelineNodeReader::toToml(const QString& node_name)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << node_name << "]" << "\n";
    out << "task_name = \""<< node_name << "\"\n";
    out << "plugin_name = \""<< this->algorithm << "\"\n";
    out << "path = \""<< this->path << "\"\n";
    out << "\n";
    return node_string;
}

QString gnomonPipelineNodeReader::toLuigiClass(void)
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
    out<<"        self.reader = gnomoncore." << this->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = []\n";
    for (const auto& output_name : this->output_ports.keys()) {
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