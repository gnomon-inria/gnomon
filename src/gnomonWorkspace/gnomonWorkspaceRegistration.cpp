#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageRegistration>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonView/gnomonViewFormPool>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

QString transformMatrixString(QVector<QVector<double> > transform_matrix)
{
    QString matrix_string;

    matrix_string += "[";
    for (int row=0; row<transform_matrix.size(); row++) {
        if (row > 0) matrix_string += "\n ";
        matrix_string += " [";
        for (int col=0; col<transform_matrix[row].size(); col++) {
            if (col > 0) matrix_string += ",";
            if (transform_matrix[row][col]>=0) matrix_string += " ";
            matrix_string += " " + QString::number(transform_matrix[row][col], 'f', 3);
        }
        matrix_string += "]";
    }
    matrix_string += " ]";

    return matrix_string;
}

QVector<QVector<double> > identity_matrix = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceRegistrationPrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceRegistrationPrivate
{

public:
     gnomonWorkspaceRegistrationPrivate(void);
    ~gnomonWorkspaceRegistrationPrivate(void);

public:
    QHash<int, gnomonImageSeries *> image_stack;
    QHash<int, gnomonDataDictSeries *> transformation_stack;

    int stack_level = -1;
};

gnomonWorkspaceRegistrationPrivate::gnomonWorkspaceRegistrationPrivate(void)
{
}

gnomonWorkspaceRegistrationPrivate::~gnomonWorkspaceRegistrationPrivate(void)
{
    if (!this->image_stack.isEmpty()) {
        for (const auto& level : this->image_stack.keys()) {
            delete this->image_stack[level];
        }
        this->image_stack.clear();
    }
    if (!this->transformation_stack.isEmpty()) {
        for (const auto& level : this->transformation_stack.keys()) {
            delete this->transformation_stack[level];
        }
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
    emit algorithmsLoaded();

    d->workspace = "Time Registration";
    d->command = new gnomonImageRegistrationCommand;
    d->keys = gnomonCore::imageRegistration::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    emit parametersChanged();

    //create the views
    this->addInputView(); // reference
    this->addInputView(); // floating
    this->addOutputView(); // registered

    this->m_target_dict = new gnomonViewData(this);
    this->m_target_dict->setAcceptForm("gnomonDataDict", true);
    
    if(!d->pool)
        d->pool = new gnomonViewFormPool(this);
    d->pool->addView(this->sources()->views()[0]);
    d->pool->addView(this->sources()->views()[1]);
    d->pool->addView(this->targets()->views()[0]);

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));

    connect(this->targets()->views()[0], &gnomonViewForm::syncedChanged, [=]() {
        this->targets()->views()[0]->disconnectTime();
        this->sources()->views()[0]->disconnectTime();
        this->sources()->views()[1]->disconnectTime();
    });
    for(int i=0; i<2; i++)
    {
        connect(this->sources()->views()[i], &gnomonViewForm::syncedChanged, [=]() {
            this->targets()->views()[0]->disconnectTime();
            this->sources()->views()[0]->disconnectTime();
            this->sources()->views()[1]->disconnectTime();
        });
    }
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
            gnomonImageSeries *input_image = dd->image_stack[dd->stack_level];
            if (input_image != this->sources()->views()[1]->image()) {
                this->sources()->views()[1]->setImage(input_image);

                if (dd->image_stack.contains(dd->stack_level+1)) {
                    gnomonImageSeries *output_image = dd->image_stack[dd->stack_level+1];
                    this->targets()->views()[0]->setImage(output_image);
                } else {
                    this->targets()->views()[0]->clear();
                }
            }
        }

        emit stackLevelChanged();
    }
}

void gnomonWorkspaceRegistration::setInputs(void)
{
    gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(d->command->inputs()["input"]);
    bool empty_input = (input_image == nullptr);

    //gnomonAlgorithmWorkspace::setInputs();
    d->command->setInputForm("input", d->sources->views()[1]->image());

    if (empty_input || !d->command->inputs()["input"]) {
        for (const auto& level : dd->image_stack.keys()) {
            delete dd->image_stack[level];
        }
        dd->image_stack.clear();
        for (const auto& level : dd->transformation_stack.keys()) {
            delete dd->transformation_stack[level];
        }
        dd->transformation_stack.clear();
        emit stackSizeChanged();
        this->setStackLevel(-1);

        if (d->command->inputs()["input"]) {
            input_image = dynamic_cast<gnomonImageSeries *>(d->command->inputs()["input"]);
            dd->image_stack.insert(0, input_image);
            emit stackSizeChanged();
            this->setStackLevel(0);
        }
    }
}

QString gnomonWorkspaceRegistration::transformStringAt(int level) const
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
}

void gnomonWorkspaceRegistration::iterate(void)
{
    gnomonImageSeries *output_image = dynamic_cast<gnomonImageSeries *>(d->command->outputs()["output"]);
    if (output_image) {
        gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(output_image->clone());
        dd->image_stack.insert(dd->stack_level+1, input_image);
        gnomonDataDictSeries *transformation = dynamic_cast<gnomonDataDictSeries *>(d->command->outputs()["transformation"]->clone());
        dd->transformation_stack.insert(dd->stack_level+1, transformation);
        emit stackSizeChanged();

        this->setStackLevel(dd->stack_level+1);

        gnomonPipelineManager::instance()->addForm(output_image);
        gnomonPipelineManager::instance()->addClonedForm(output_image, input_image);
    }

}

void gnomonWorkspaceRegistration::viewOutputs()
{
    gnomonImageRegistrationCommand * command = dynamic_cast<gnomonImageRegistrationCommand *>(d->command);
    if(command->outputs()["transformation"]) {
        this->m_target_dict->setForm("gnomonDataDict", command->outputs()["transformation"]);
        int form_count = gnomonFormManager::instance()->formCount(command->outputs()["transformation"]->formName());
        command->outputs()["transformation"]->metadata()->set("name", command->outputs()["transformation"]->formName().remove("gnomon") + QString::number(form_count+1));
        command->outputs()["transformation"]->metadata()->set("source", d->algorithm);
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
