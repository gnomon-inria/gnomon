#include "gnomonPipelineNodeAlgorithm.h"

#include <dtkCore>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"
#include "gnomonPythonPluginLoader"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithmPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeAlgorithmPrivate {
public:
    QJsonObject parameters;
    QJsonObject metadata;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithm
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAlgorithm::gnomonPipelineNodeAlgorithm(const QString &algorithm_class, const QString &algorithm,
                                                         QJsonObject &parameters,
                                                         QList<QString> inputs, QList<QString> outputs,
                                                         QJsonObject metadata) :
                                                         gnomonPipelineNode(),dd(new gnomonPipelineNodeAlgorithmPrivate)
{
    if (algorithm_class.contains("From")) {
        d->type = gnomonPipelineNode::NODE_CONVERTER;
    } else {
        d->type = gnomonPipelineNode::NODE_FILTER;
    }
    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->parameters = parameters;
    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    // this->layout()();

    qDebug() << "A-a";
    dd->metadata = metadata;
    qDebug() << "A-b";
    auto plugins = availablePluginsFromGroup(algorithm_class);
    qDebug() << "A-c";
    if(plugins.contains(algorithm)) {
        qDebug() << "B-a";
        auto localMetadata = pluginMetadata(algorithm_class, algorithm);
        qDebug() << "B-b";
        QMap<QString, QString>::key_value_iterator ptr;
        for(ptr = localMetadata.keyValueBegin(); ptr!=localMetadata.keyValueEnd(); ptr++) {
            qDebug() << "C-a";
            dd->metadata.insert(ptr->first, ptr->second);
            qDebug() << "C-b";
        }
        qDebug() << "B-c";
    }
    qDebug() << "A-d";
}

gnomonPipelineNodeAlgorithm::~gnomonPipelineNodeAlgorithm(void)
{

}

QJsonObject gnomonPipelineNodeAlgorithm::parameters(void)
{
    return dd->parameters;
}

QString gnomonPipelineNodeAlgorithm::toToml(void)
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

const QJsonObject gnomonPipelineNodeAlgorithm::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();

    QJsonObject parameters;
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        if (param == "python_code") {
            parameters.insert(param, dd->parameters[param]);
        } else {
            QVariantHash parameter = dd->parameters[param].toObject().toVariantHash();
            parameters.insert(param, QJsonObject::fromVariantHash(parameter));
        }
    }
    json.insert("parameters", parameters);
    json.insert("metadata", dd->metadata);

    return json;
}

QString gnomonPipelineNodeAlgorithm::toLuigiClass(void)
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
    out<<"        self.algorithm = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = [";
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        if (it != d->input_ports.begin()) {
            out<<", ";
        }
        auto&& input_name = it.key();
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.algorithm." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        inputs = self.algorithm_inputs()\n";
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        QRegularExpression numbered_input("[A-z]+[0-9]+");
        if (numbered_input.match(input_name).hasMatch()) {
            QString input_type_name = QString(input_name);
            input_type_name = input_type_name.remove(QRegularExpression("[0-9]+"));
            QString adder_name = "add" + input_type_name;
            adder_name.replace(3, 1, adder_name[3].toUpper());
            out<<"        self.algorithm." << adder_name << "(inputs[\"" << input_name << "\"])\n";
        } else {
            QString setter_name = "set" + input_name;
            setter_name.replace(3, 1, setter_name[3].toUpper());
            out<<"        self.algorithm." << setter_name << "(inputs[\"" << input_name << "\"])\n";
        }
    }
    out<<"        for name, value in self.parameters.items():\n";
    out<<"            if isinstance(value, tuple):\n";
    out<<"                value = list(value)\n";
    out<<"            self.algorithm.setParameter(name,value)\n";
    out<<"        self.algorithm.run()\n";
    out<<"\n";

    return luigi_string;
}

QStringList gnomonPipelineNodeAlgorithm::parametersName(void) {
    return dd->parameters.keys();
}

void gnomonPipelineNodeAlgorithm::configureParameter(const QString &name, dtkCoreParameter *param) {
    param->setValue(QVariant(dd->parameters[name].toObject().toVariantHash()));
}

QString gnomonPipelineNodeAlgorithm::getParameterAsString(const QString &name) {
    return dd->parameters[name].toString();
}

QMap<QString, QString> gnomonPipelineNodeAlgorithm::getPluginMetadata(void) {
    QMap<QString, QString> out;
    for(const auto &key: dd->metadata.keys()) {
        out.insert(key, dd->metadata[key].toString());
    }
    return out;
}

//
// gnomonPipelineNodeAlgorithm.cpp ends here