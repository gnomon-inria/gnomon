#include "gnomonImageRegistrationCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageRegistrationCommandPrivate
{
public:
    gnomonAbstractCommand::orderedMap input_types = {{"input", "gnomonImage"}};
    gnomonAbstractCommand::orderedMap output_types = {{"output", "gnomonImage"}, {"transformation", "gnomonDataDict"}};

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs = {{"input", nullptr}};
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs = {{"output", nullptr}, {"transformation", nullptr}};
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageRegistrationCommand::gnomonImageRegistrationCommand() : d(new gnomonImageRegistrationCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageRegistration::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageRegistration::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageRegistrationCommand::~gnomonImageRegistrationCommand()
{
    delete d;
}

void gnomonImageRegistrationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::imageRegistration::pluginFactory().create(algo_name);
}

void gnomonImageRegistrationCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonImageRegistrationCommand::postdo(void)
{
    std::shared_ptr<gnomonImageSeries> image = ((gnomonAbstractImageRegistration *) this->action)->output();

    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->outputs["output"] = nullptr;
    } else {
        d->outputs["output"] = image;
    }

    std::shared_ptr<gnomonDataDictSeries> transformation = ((gnomonAbstractImageRegistration *) this->action)->outputTransformation();
    if ((!transformation)||(transformation->times().empty())||(transformation->current()->keys().empty())) {
        d->outputs["transformation"] = nullptr;
    } else {
        d->outputs["transformation"] = transformation;
    }
}

void gnomonImageRegistrationCommand::undo()
{
    this->setImage(nullptr);
}

void gnomonImageRegistrationCommand::setImage(std::shared_ptr<gnomonImageSeries> image_series)
{
    if ((!image_series)||(image_series->times().empty())||(image_series->current()->channels().empty())) {
        d->inputs["input"] = nullptr;
    } else {
        d->inputs["input"] = image_series;
    }
    ((gnomonAbstractImageRegistration *) this->action)->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(d->inputs["input"]));
}

std::shared_ptr<gnomonImageSeries> gnomonImageRegistrationCommand::image()
{
    return std::dynamic_pointer_cast<gnomonImageSeries>(d->inputs["input"]);
}

std::shared_ptr<gnomonImageSeries> gnomonImageRegistrationCommand::output()
{
    return std::dynamic_pointer_cast<gnomonImageSeries>(d->outputs["output"]);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageRegistrationCommand::inputs()
{
    return d->inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::inputTypes()
{
    return d->input_types;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageRegistrationCommand::outputs()
{
    return d->outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::outputTypes()
{
    return d->output_types;
}

bool gnomonImageRegistrationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonImageRegistrationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonImageRegistrationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    // TODO: come back later to check if correct
    if(name == "input") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else {
        dtkWarn() << Q_FUNC_INFO << "unknown input " << name;
        return;
    }
}

void gnomonImageRegistrationCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->outputs["output"]) {
        d->outputs["output"] = std::make_shared<gnomonBinaryImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->outputs["output"]->deserialize(tmp);
    if(!d->outputs["transformation"]) {
        d->outputs["transformation"] = std::make_shared<gnomonBinaryImageSeries>();
    }
    auto tmp2 = serialization["transformation"].toObject();
    d->outputs["transformation"]->deserialize(tmp2);
}

QJsonObject gnomonImageRegistrationCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = std::dynamic_pointer_cast<gnomonImageSeries>(d->outputs["output"])->serialize();
    out["transformation"] = std::dynamic_pointer_cast<gnomonDataDictSeries>(d->outputs["transformation"])->serialize();
    return out;
}

//
// gnomonImageRegistrationCommand.cpp ends here
