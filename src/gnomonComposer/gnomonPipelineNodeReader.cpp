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

gnomonPipelineNodeReader::gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path) : gnomonPipelineNode()
{
    d->color = QColor(170, 255, 0);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->output_port = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
    this->addOutputPort(output_port);
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
    out << "plugin_name = \""<< this->algorithm << "\"\n";
    out << "path = \""<< this->path << "\"\n";
    out << "\n";
    return node_string;
}

//
// gnomonPipelineNodeReader.cpp ends here