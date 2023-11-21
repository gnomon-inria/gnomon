#include "gnomonWorkspaceCellImageQuantification.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageQuantification>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageQuantificationCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonProject>
#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    d->workspace = "Cell Analysis";
    d->command = new gnomonCellImageQuantificationCommand;
    d->keys = gnomonCore::cellImageQuantification::pluginFactory().keys();
    d->algorithmsData = gnomonCore::cellImageQuantification::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    this->addInputView();

    d->figure = new gnomonMplView(this);
    d->figure->setAcceptForm("gnomonDataFrame",true);
    connect(d->figure, &gnomonMplView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        d->pipeline_manager->addForm(f->uuid());
    });
    emit parametersChanged();
    d->updatePool(); //unused here
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d->command;
}

void gnomonWorkspaceCellImageQuantification::setInputs()
{
    d->command->undo(); //clean

    auto *command = dynamic_cast<gnomonCellImageQuantificationCommand *>(d->command);
    for(gnomonVtkView *f : d->sources->views()) {
        if (f->image()) {
            command->setInputForm("image", f->image());
        }
        if (f->mesh()) {
            command->setInputForm("mesh", f->mesh());
        }
        if (f->cellImage()) {
            command->setInputForm("cellImage", f->cellImage());
        }
    }
}


void gnomonWorkspaceCellImageQuantification::viewOutputs()
{
    gnomonCellImageQuantificationCommand * command = dynamic_cast<gnomonCellImageQuantificationCommand *>(d->command);

    std::shared_ptr<gnomonAbstractDynamicForm> inputForm = d->command->inputs()["cellImage"];

    if ((command->cellImage() != nullptr) || (command->dataFrame() != nullptr)) {
        d->registerPipeline();
    }

    if(command->cellImage()) {
        d->sources->views()[0]->removeForm("gnomonCellImage");
        d->sources->views()[0]->setForm("gnomonCellImage", command->cellImage());
        std::shared_ptr<gnomonCellImageSeries> out_cellimage = d->sources->views()[0]->cellImage();
        int form_count = gnomonFormManager::instance()->formCount(out_cellimage->formName());
        out_cellimage->metadata()->set("name", out_cellimage->formName().remove("gnomon") + QString::number(form_count+1));
        out_cellimage->metadata()->set("source", d->algorithm);
        //gnomonPipelineManager::instance()->addClonedForm(command->cellImage(), out_cellimage);
        GNOMON_SESSION->addForm(command->cellImage());
        gnomonPipelineManager::instance()->addForm(command->cellImage()->uuid());
        d->sources->views()[0]->setInputView(false);
    }
    if(command->dataFrame()) {
        d->figure->setForm("gnomonDataFrame", command->dataFrame());
        int form_count = gnomonFormManager::instance()->formCount(command->dataFrame()->formName());
        command->dataFrame()->metadata()->set("name", command->dataFrame()->formName().remove("gnomon") + QString::number(form_count+1));
        command->dataFrame()->metadata()->set("source", d->algorithm);
    }
}

gnomonVtkViewList* gnomonWorkspaceCellImageQuantification::targets(void) const
{
    return d->sources;
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
