#include "gnomonWorkspacePointCloudQuantification.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudQuantification>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudQuantificationCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePointCloudQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePointCloudQuantification::gnomonWorkspacePointCloudQuantification(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    d->workspace = "Point Cloud Quantification";
    d->command = new gnomonPointCloudQuantificationCommand;
    d->keys = gnomonCore::pointCloudQuantification::pluginFactory().keys();
    d->algorithmsData = gnomonCore::pointCloudQuantification::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    this->addInputView();

    d->figure = new gnomonMplView(this);
    d->figure->setAcceptForm("gnomonDataFrame",true);
    connect(d->figure, &gnomonMplView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        d->pipeline_manager->addForm(f);
    });
    emit parametersChanged();
    d->updatePool(); //unused here
}

gnomonWorkspacePointCloudQuantification::~gnomonWorkspacePointCloudQuantification(void)
{
    delete d->command;
}

void gnomonWorkspacePointCloudQuantification::setInputs()
{
    d->command->undo(); //clean

    auto *command = dynamic_cast<gnomonPointCloudQuantificationCommand *>(d->command);
    for(gnomonVtkView *f : d->sources->views()) {
        if (f->image()) {
            command->setInputForm("image", f->image());
        }
        if (f->pointCloud()) {
            command->setInputForm("pointCloud", f->pointCloud());
        }
    }
}


void gnomonWorkspacePointCloudQuantification::viewOutputs()
{
    gnomonPointCloudQuantificationCommand * command = dynamic_cast<gnomonPointCloudQuantificationCommand *>(d->command);

    std::shared_ptr<gnomonAbstractDynamicForm> inputForm = d->command->inputs()["pointCloud"];

    if ((command->pointCloud() != nullptr) || (command->dataFrame() != nullptr)) {
        d->registerPipeline();
    }

    if(command->pointCloud()) {
        d->sources->views()[0]->removeForm("gnomonPointCloud");
        d->sources->views()[0]->setForm("gnomonPointCloud", command->pointCloud());
        std::shared_ptr<gnomonPointCloudSeries> out_pointCloud = d->sources->views()[0]->pointCloud();
        int form_count = gnomonFormManager::instance()->formCount(out_pointCloud->formName());
        out_pointCloud->metadata()->set("name", out_pointCloud->formName().remove("gnomon") + QString::number(form_count+1));
        out_pointCloud->metadata()->set("source", d->algorithm);
        //gnomonPipelineManager::instance()->addClonedForm(command->pointCloud(), out_pointCloud);
        gnomonPipelineManager::instance()->addForm(command->pointCloud());
        d->sources->views()[0]->setInputView(false);
    }
    if(command->dataFrame()) {
        d->figure->setForm("gnomonDataFrame", command->dataFrame());
        int form_count = gnomonFormManager::instance()->formCount(command->dataFrame()->formName());
        command->dataFrame()->metadata()->set("name", command->dataFrame()->formName().remove("gnomon") + QString::number(form_count+1));
        command->dataFrame()->metadata()->set("source", d->algorithm);
    }
}

gnomonVtkViewList* gnomonWorkspacePointCloudQuantification::targets(void) const
{
    return d->sources;
}

//
// gnomonWorkspacePointCloudQuantification.cpp ends here
