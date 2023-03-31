#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonView/gnomonVtkViewPool>
#include <gnomonVisualization/gnomonView/gnomonQmlView>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>


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

    loadPluginGroup("imageRegistration");

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

    this->m_target_dict = new gnomonQmlView(this);
    this->m_target_dict->setAcceptForm("gnomonDataDict", true);

    // TODO: actually create a dataDict and add it to the transformation stack
    QVector<QVector<double>> eye4 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    m_target_dict->setDataDict(transformMatrixString(eye4));
    
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

    connect(this->m_target_dict, &gnomonAbstractView::exportedForm, [=](auto form) {
        gnomonPipelineManager::instance()->addForm(form);
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
            if (level>=1) {
                std::shared_ptr<gnomonDataDictSeries> data_dict = dd->transformation_stack[dd->stack_level];
                if (data_dict) {
                    // Force display of floating transformation
                    data_dict->at(0);
                }
                m_target_dict->setForm("gnomonDataDict", data_dict);
            } else {
                QVector<QVector<double>> eye4 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
                m_target_dict->setDataDict(transformMatrixString(eye4));
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
    if(m_target_dict->contains("gnomonDataDict")) {
        d->command->setInputForm("initialTransformation", m_target_dict->form("gnomonDataDict"));
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

/* QString gnomonWorkspaceRegistration::transformStringAt(int level) const
{
    if (dd->image_stack.contains(level)) {
        if (dd->transformation_stack.contains(level)) {
            gnomonDataDictSeries *transformation = dd->transformation_stack[level];
            if (transformation->current()->keys().contains("transform")) {
                QVariant transform = transformation->current()->get("transform");
                QVector<QVector< double>> transform_matrix = transform.value<QVector<QVector< double> > >();
                return transformMatrixString(transform_matrix);
            } else {
                dtkWarn()<<Q_FUNC_INFO<<"Transformation info has no transform matrix, Identity is returned";
                return transformMatrixString(identity_matrix);
            }
        } else {
            dtkWarn()<<Q_FUNC_INFO<<"Level"<<level<<"has no Transformation info, Identity is returned";
            return transformMatrixString(identity_matrix);
        }
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Level"<<level<<"is invalid! Image stack only contains"<<dd->image_stack.keys();
        return "";
    }
} */

void gnomonWorkspaceRegistration::iterate(void)
{
    std::shared_ptr<gnomonImageSeries> output_image = std::dynamic_pointer_cast<gnomonImageSeries>(d->command->outputs()["output"]);
    if (output_image) {
        dd->image_stack.insert(dd->stack_level+1, output_image);
        std::shared_ptr<gnomonDataDictSeries> transformation = std::dynamic_pointer_cast<gnomonDataDictSeries>(d->command->outputs()["outputTransformation"]);
        dd->transformation_stack.insert(dd->stack_level+1, transformation);
        emit stackSizeChanged();

        this->setStackLevel(dd->stack_level+1);

        gnomonPipelineManager::instance()->addForm(output_image);
        gnomonPipelineManager::instance()->addForm(transformation);
    }
}

void gnomonWorkspaceRegistration::viewOutputs()
{   
    auto * command = dynamic_cast<gnomonImageRegistrationCommand *>(d->command);
    if(command->outputs()["outputTransformation"]) {
        std::shared_ptr<gnomonAbstractDynamicForm> data_dict = command->outputs()["outputTransformation"];
        int form_count = gnomonFormManager::instance()->formCount(data_dict->formName());
        data_dict->metadata()->set("name", data_dict->formName().remove("gnomon") + QString::number(form_count+1));
        data_dict->metadata()->set("source", d->algorithm);
        iterate();
        this->m_target_dict->setForm("gnomonDataDict", data_dict);
    }
    gnomonAlgorithmWorkspace::viewOutputs();
}


//
// gnomonWorkspaceRegistration.cpp ends here
