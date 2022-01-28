#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>


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

QVector<QVector<double> > identityMatrix(void)
{
    QVector<QVector<double> > identity_matrix = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
    return identity_matrix;
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
    emit algorithmsLoaded();

    d->workspace = "Time Registration";
    d->command = new gnomonImageRegistrationCommand;
    d->keys = gnomonCore::imageRegistration::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    emit parametersChanged();

    //create the views
    this->sources()->addView(); // reference
    this->sources()->addView(); // floating
    this->targets()->addView(); // registered

    if(!d->pool)
        d->pool = new gnomonViewFormPool(this);
    d->pool->addView(this->sources()->views()[0]);
    d->pool->addView(this->sources()->views()[1]);
    d->pool->addView(this->targets()->views()[0]);

    d->updateViewFormTypes();
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
                this->targets()->views()[0]->clear();
            }
        }

        emit stackLevelChanged();
    }
}

void gnomonWorkspaceRegistration::setInputs(void)
{
    gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(d->command->inputs()["input"]);
    bool empty_input = (input_image == nullptr);

    gnomonAlgorithmWorkspace::setInputs();

    if (empty_input | !d->command->inputs()["input"]) {
        dd->image_stack.clear();
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
                return transformMatrixString(identityMatrix());
            }
        } else {
            dtkWarn()<<Q_FUNC_INFO<<"Level"<<level<<"has no Transformation info, Identity is returned";
            return transformMatrixString(identityMatrix());
        }
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Invalid level! Image stack only contains"<<dd->image_stack.keys();
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

        gnomonPipeline::instance()->addForm(output_image);
        gnomonPipeline::instance()->addClonedForm(output_image, input_image);
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
