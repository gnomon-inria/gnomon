#include "gnomonPipelineNodeWriter.h"

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"
#include "gnomonPipelineNodeMorphonet.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeMorphonetPrivate
// /////////////////////////////////////////////////////////////////


class gnomonPipelineNodeMorphonetPrivate {
public:
    QJsonObject morphonet_data;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeMorphonet
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeMorphonet::gnomonPipelineNodeMorphonet(const QString& form_name, QJsonObject morphonet_data): 
                            gnomonPipelineNode(),
                            dd(new gnomonPipelineNodeMorphonetPrivate)
{
    d->color = QColor(135, 206, 235);
    d->type = gnomonPipelineNode::NODE_MORPHONET;
    d->algorithm_class = "morphonetCellImage";
    d->algorithm = form_name;
    dd->morphonet_data = morphonet_data;

    this->addOutputPort(form_name, new gnomonPipelinePort(gnomonPipelinePort::Output, form_name, this));

}

gnomonPipelineNodeMorphonet::~gnomonPipelineNodeMorphonet(void)
{

}

QString gnomonPipelineNodeMorphonet::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    // out << "id = \""<< dd->current_id << "\"\n";
    // out << "voxelsize = \""<< dd->voxelsize << "\"\n";
    // out << "start_time = \""<< dd->start_time << "\"\n";
    // out << "end_time = \""<< dd->end_time << "\"\n";
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeMorphonet::toJson(void)
{
    
    QJsonObject json;
    json.insert("name", d->name);
    json.insert("plugin_group", d->algorithm_class);
    json.insert("plugin_name", d->algorithm);
    json.insert("plugin_version", d->version);
    json.insert("description", d->description);

    QJsonObject in;
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.insert(input_name, QJsonValue::Null);
    }
    json.insert("inputs", in);

    QJsonArray out;
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out.append(output_name);
    }
    json.insert("outputs", out); 

    QJsonObject morphonet_data;
    for(auto it = dd->morphonet_data.begin(); it != dd->morphonet_data.end(); ++it) {
        auto&& md = it.key();
        morphonet_data.insert(md, dd->morphonet_data[md]);
    }
    json.insert("morphonet_data", morphonet_data);

    return json;    
}

QString gnomonPipelineNodeMorphonet::toLuigiClass(void)
{
    return "";
}

QJsonObject gnomonPipelineNodeMorphonet::morphonet_data(void) 
{
    return dd->morphonet_data;
}