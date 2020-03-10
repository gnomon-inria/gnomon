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

gnomonPipelineNodeWriter::gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path) : gnomonPipelineNode()
{
    d->color = QColor(255, 255, 0);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->input_port = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
    this->addInputPort(input_port);
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

//
// gnomonPipelineNodeWriter.cpp ends here