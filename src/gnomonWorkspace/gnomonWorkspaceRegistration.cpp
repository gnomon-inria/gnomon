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
    QList<gnomonImageSeries *> image_stack;
    QList<gnomonDataDictSeries *> data_stack;

    int stack_level = -1;
};


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
        (*d->sources)[1]->setImage(output_image);
        gnomonPipeline::instance()->addForm(output_image);
        gnomonPipeline::instance()->addClonedForm(output_image, (*d->sources)[1]->image());

        dd->image_stack.push_back((*d->sources)[1]->image());
        dd->stack_level++;

        (*d->targets)[0]->clear();
    }
}


//
// gnomonWorkspaceRegistration.cpp ends here
