#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>

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
    QHash<int, gnomonDataDictSeries *> data_stack;

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
    if (!this->data_stack.isEmpty()) {
        this->data_stack.clear();
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
        emit stackLevelChanged();
    }
}

void gnomonWorkspaceRegistration::setInputs(void)
{
    gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(d->command->inputs()["input"]);
    bool empty_input = (input_image == nullptr);

    gnomonAlgorithmWorkspace::setInputs();

    if (empty_input) {
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

void gnomonWorkspaceRegistration::iterate(void)
{
    gnomonImageSeries *output_image = dynamic_cast<gnomonImageSeries *>(d->command->outputs()["output"]);
    if (output_image) {
        gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(output_image->clone());

        dd->image_stack.insert(dd->stack_level+1, input_image);
        emit stackSizeChanged();

        this->setStackLevel(dd->stack_level+1);
        this->sources()->views()[1]->setImage(input_image);

        gnomonPipeline::instance()->addForm(output_image);
        gnomonPipeline::instance()->addClonedForm(output_image, input_image);

        this->targets()->views()[0]->clear();
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
