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

#include "gnomonPipelineNodeEvolutionModel.h"

#include <dtkCore>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeEvolutionModelPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeEvolutionModelPrivate {
public:
    QJsonObject parameters;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeEvolutionModel
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeEvolutionModel::gnomonPipelineNodeEvolutionModel(const QString& algorithm_class, const QString& algorithm, QJsonObject parameters, QList<QString> outputs) : gnomonPipelineNode(), dd(new gnomonPipelineNodeEvolutionModelPrivate)
{
    d->type = gnomonPipelineNode::NODE_EVOLUTION_MODEL;

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->parameters = parameters;
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    // this->layout()();
}

gnomonPipelineNodeEvolutionModel::~gnomonPipelineNodeEvolutionModel(void)
{

}

QJsonObject gnomonPipelineNodeEvolutionModel::parameters(void)
{
    return dd->parameters;
}

QString gnomonPipelineNodeEvolutionModel::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "    [" << d->name << ".parameters]\n";
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        dtkCoreParameter *parameter = dtkCoreParameter::create(dd->parameters[param].toObject().toVariantHash());
        QString parameter_string = d->variantParameterString(parameter->variant());
        out << "    " << param << " = " << parameter_string << "\n";
    }
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeEvolutionModel::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();

    QJsonObject parameters;
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        QVariantHash parameter = dd->parameters[param].toObject().toVariantHash();
        parameters.insert(param, QJsonObject::fromVariantHash(parameter));
    }
    json.insert("parameters", parameters);

    return json;
}

QString gnomonPipelineNodeEvolutionModel::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(d->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    parameters = luigi.DictParameter()\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << d->algorithm_class << "\")\n";
    out<<"        self.model = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = []\n";
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.model." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        for name, value in self.parameters.items():\n";
    out<<"            if isinstance(value, tuple):\n";
    out<<"                value = list(value)\n";
    out<<"            self.model.setParameter(name,value)\n";
    out<<"        self.model.run()\n";
    out<<"\n";

    return luigi_string;
}

QStringList gnomonPipelineNodeEvolutionModel::parametersName(void) {
    return dd->parameters.keys();
}

void gnomonPipelineNodeEvolutionModel::configureParameter(const QString &name, dtkCoreParameter *param) {
    param->setValue(QVariant(dd->parameters[name].toObject().toVariantHash()));
}

//
// gnomonPipelineNodeEvolutionModel.cpp ends here