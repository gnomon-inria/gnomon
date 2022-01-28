#include "gnomonWorkspaceRegistration.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>

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
    qDebug()<<Q_FUNC_INFO<<dd->stack_level;

    loadPluginGroup("imageRegistration");
    emit algorithmsLoaded();

    d->workspace = "Time Registration";
    d->command = new gnomonImageRegistrationCommand;
    d->keys = gnomonCore::imageRegistration::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    emit parametersChanged();

    //create the views
    this->sources()->addView();
    this->sources()->addView();
    this->targets()->addView();

    d->updateViewFormTypes();
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    gnomonImageRegistrationCommand *command = (gnomonImageRegistrationCommand *)d->command;
    if (command) {
        delete command;
    }
}

void gnomonWorkspaceRegistration::iterate(void)
{
    gnomonImageSeries *output_image = dynamic_cast<gnomonImageSeries *>(d->command->outputs()["output"]);
    if (output_image) {
        gnomonImageSeries *input_image = dynamic_cast<gnomonImageSeries *>(output_image->clone());

        int level = dd->stack_level++;
        dd->image_stack.insert(level, input_image);
        this->sources()->views()[1]->setImage(input_image);

        gnomonPipeline::instance()->addForm(output_image);
        gnomonPipeline::instance()->addClonedForm(output_image, input_image);

        this->targets()->views()[0]->clear();
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
