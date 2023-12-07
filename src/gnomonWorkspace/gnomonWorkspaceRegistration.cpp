#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonProject>
#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonView/gnomonVtkViewPool>
#include <gnomonVisualization/gnomonView/gnomonQmlView>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

std::shared_ptr<gnomonDataDictSeries> identityDataDict(void)
{
    std::shared_ptr<gnomonDataDictSeries> dict = nullptr;
    QStringList data_dict_plugins = gnomonCore::dataDictData::pluginFactory().keys();
    if (data_dict_plugins.size() > 0) {
        gnomonAbstractDataDictData *data_dict_data = gnomonCore::dataDictData::pluginFactory().create(
                data_dict_plugins[0]);
        std::shared_ptr<gnomonDataDict> data_dict = std::make_shared<gnomonDataDict>();
        data_dict->setData(data_dict_data);

        QVector<QVector<double>> eye4 = {{1, 0, 0, 0},
                                         {0, 1, 0, 0},
                                         {0, 0, 1, 0},
                                         {0, 0, 0, 1}};
        QString eye4_str = transformMatrixString(eye4);
        data_dict->set("transform", eye4_str);

        dict = std::make_shared<gnomonDataDictSeries>();
        dict->insert(0, data_dict);
    }
    return dict;
}


// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceRegistrationPrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceRegistrationPrivate
{

public:
     gnomonWorkspaceRegistrationPrivate(void);
    ~gnomonWorkspaceRegistrationPrivate(void);

public:
    QHash<int, std::shared_ptr<gnomonImageSeries> > image_stack;
    QHash<int, std::shared_ptr<gnomonDataDictSeries> > transformation_stack;

    int stack_level = -1;
};

gnomonWorkspaceRegistrationPrivate::gnomonWorkspaceRegistrationPrivate(void)
{
}

gnomonWorkspaceRegistrationPrivate::~gnomonWorkspaceRegistrationPrivate(void)
{
    if (!this->image_stack.isEmpty()) {
        this->image_stack.clear();
    }
    if (!this->transformation_stack.isEmpty()) {
        this->transformation_stack.clear();
    }
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceRegistration
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    dd = new gnomonWorkspaceRegistrationPrivate;

    d->workspace = "Time Registration";
    d->command = new gnomonImageRegistrationCommand;
    d->keys = gnomonCore::imageRegistration::pluginFactory().keys();
    d->algorithmsData = gnomonCore::imageRegistration::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();
    emit parametersChanged();

    //create the views
    this->addInputView({}, {"reference", "image"}); // reference
    this->addInputView({}, {"floating", "image"}); // floating
    this->addOutputView(); // registered

    d->text_view = new gnomonQmlView(this);
    d->text_view->setAcceptForm("gnomonDataDict", true);

    std::shared_ptr<gnomonDataDictSeries> input_dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(d->text_view->form("gnomonDataDict"));
    if (!input_dict) {
        d->text_view->setForm("gnomonDataDict", identityDataDict());
    } else {
        d->text_view->setForm("gnomonDataDict", input_dict);
    }
    dd->transformation_stack.insert(0, std::dynamic_pointer_cast<gnomonDataDictSeries>(d->text_view->form("gnomonDataDict")));
    emit stackSizeChanged();

    if(!d->pool)
        d->pool = new gnomonVtkViewPool(this);
    d->pool->addView(this->sources()->views()[0]);
    d->pool->addView(this->sources()->views()[1]);
    d->pool->addView(this->targets()->views()[0]);

    connect(this->targets()->views()[0], &gnomonVtkView::syncedChanged, [=]() {
        this->targets()->views()[0]->disconnectTime();
        this->sources()->views()[0]->disconnectTime();
        this->sources()->views()[1]->disconnectTime();
    });
    for(int i=0; i<2; i++)
    {
        connect(this->sources()->views()[i], &gnomonVtkView::syncedChanged, [=]() {
            this->targets()->views()[0]->disconnectTime();
            this->sources()->views()[0]->disconnectTime();
            this->sources()->views()[1]->disconnectTime();
        });
    }

    connect(d->text_view, &gnomonAbstractView::exportedForm, [=](auto form) {
        gnomonPipelineManager::instance()->addForm(form->uuid());
    });

    connect(d->text_view, &gnomonQmlView::formsChanged, [=]() {
        this->setInputs();
    });
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    gnomonImageRegistrationCommand *command = (gnomonImageRegistrationCommand *)d->command;
    if (command) {
        delete command;
    }
}

int gnomonWorkspaceRegistration::stackSize(void) const
{
    return dd->image_stack.size();
}

int gnomonWorkspaceRegistration::stackLevel(void) const
{
    return dd->stack_level;
}

void gnomonWorkspaceRegistration::setStackLevel(int level)
{
    if (level != dd->stack_level) {
        dd->stack_level = level;

        if (dd->image_stack.contains(dd->stack_level)) {
            if (dd->transformation_stack.contains(dd->stack_level) && dd->transformation_stack[dd->stack_level]) {
                std::shared_ptr<gnomonDataDictSeries> data_dict = dd->transformation_stack[dd->stack_level];
                if (data_dict) {
                    // Force display of floating transformation
                    data_dict->at(0);
                }
                d->text_view->setForm("gnomonDataDict", data_dict);
            } else {
                d->text_view->setForm("gnomonDataDict", identityDataDict());
            }

            std::shared_ptr<gnomonImageSeries> input_image = dd->image_stack[0];
            if (dd->image_stack.contains(dd->stack_level) && level>=1) {
                std::shared_ptr<gnomonImageSeries> output_image = dd->image_stack[dd->stack_level];
                this->targets()->views()[0]->setForm("gnomonImage", output_image);
            } else {
                this->targets()->views()[0]->clear();
            }
        }
        emit stackLevelChanged();
    }
}

void gnomonWorkspaceRegistration::setInputs(void)
{
    std::shared_ptr<gnomonImageSeries> input_image = std::dynamic_pointer_cast<gnomonImageSeries>(d->command->inputs()["image"]);
    bool empty_input = (input_image == nullptr);

    //gnomonAlgorithmWorkspace::setInputs();
    d->command->setInputForm("image", d->sources->views()[1]->image());
    if(d->text_view->contains("gnomonDataDict")) {
        d->command->setInputForm("initialTransformation", d->text_view->form("gnomonDataDict"));
    }

    if (empty_input || !d->command->inputs()["image"]) {
        dd->image_stack.clear();
        dd->transformation_stack.clear();
        emit stackSizeChanged();
        this->setStackLevel(-1);

        if (d->command->inputs()["image"]) {
            input_image = std::dynamic_pointer_cast<gnomonImageSeries>(d->command->inputs()["image"]);
            dd->image_stack.insert(0, input_image);
            emit stackSizeChanged();
            this->setStackLevel(0);
        }
    }
}

void gnomonWorkspaceRegistration::iterate(void)
{
    std::shared_ptr<gnomonImageSeries> output_image = std::dynamic_pointer_cast<gnomonImageSeries>(d->command->outputs()["output"]);
    if (output_image) {
        dd->image_stack.insert(dd->stack_level+1, output_image);
        std::shared_ptr<gnomonDataDictSeries> transformation = std::dynamic_pointer_cast<gnomonDataDictSeries>(d->command->outputs()["outputTransformation"]);
        dd->transformation_stack.insert(dd->stack_level+1, transformation);
        emit stackSizeChanged();

        this->setStackLevel(dd->stack_level+1);

        GNOMON_SESSION->addForm(output_image);
        gnomonPipelineManager::instance()->addForm(output_image->uuid());
        GNOMON_SESSION->addForm(transformation);
        gnomonPipelineManager::instance()->addForm(transformation->uuid());
    }
}

void gnomonWorkspaceRegistration::viewOutputs()
{
    gnomonAlgorithmWorkspace::viewOutputs();
    auto * command = dynamic_cast<gnomonImageRegistrationCommand *>(d->command);
    if(command->outputs()["outputTransformation"]) {
        std::shared_ptr<gnomonAbstractDynamicForm> data_dict = command->outputs()["outputTransformation"];
        GNOMON_SESSION->registerForm(data_dict);
        int form_count = gnomonFormManager::instance()->formCount(data_dict->formName());
        data_dict->metadata()->set("name", data_dict->formName().remove("gnomon") + QString::number(form_count+1));
        data_dict->metadata()->set("source", d->algorithm);
        iterate();
        d->text_view->setForm("gnomonDataDict", data_dict);
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
