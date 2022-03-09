#include "gnomonWorkspaceCellImageQuantification.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageQuantificationCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkScript>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageQuantification");
    emit algorithmsLoaded();

    d->workspace = "Cell Analysis";
    d->command = new gnomonCellImageQuantificationCommand;
    d->keys = gnomonCore::cellImageQuantification::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    this->addInputView();

    this->m_target_mpl = new gnomonViewMatplotlib(this);
    this->m_target_mpl->setAcceptForm("gnomonDataFrame",true);
    connect(this->m_target_mpl, &gnomonViewMatplotlib::exportedForm, [=] (gnomonAbstractDynamicForm *f) {
        d->pipeline_manager->addForm(f);
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
    for(gnomonViewForm *f : d->sources->views()) {
        if (f->image()) {
            command->setInputForm("image", f->image());
        }
        if (f->cellImage()) {
            command->setInputForm("cellImage", f->cellImage());
        }
    }
}


void gnomonWorkspaceCellImageQuantification::viewOutputs()
{
    gnomonCellImageQuantificationCommand * command = dynamic_cast<gnomonCellImageQuantificationCommand *>(d->command);

    gnomonAbstractDynamicForm* inputForm = d->command->inputs()["cellImage"];

    if ((command->cellImage() != nullptr) || (command->dataFrame() != nullptr)) {
        d->registerPipeline();
    }

    if(command->cellImage()) {
        d->sources->views()[0]->setForm("gnomonCellImage", command->cellImage()->clone());
        gnomonCellImageSeries *out_cellimage = d->sources->views()[0]->cellImage();
        int form_count = gnomonFormManager::instance()->formCount(out_cellimage->formName());
        out_cellimage->metadata()->set("name", out_cellimage->formName().remove("gnomon") + QString::number(form_count+1));
        out_cellimage->metadata()->set("source", d->algorithm);
        gnomonPipelineManager::instance()->addClonedForm(command->cellImage(), out_cellimage);
        gnomonPipelineManager::instance()->addForm(command->cellImage());
        d->sources->views()[0]->setInputView(false);
    }
    if(command->dataFrame()) {
        this->m_target_mpl->setForm("gnomonDataFrame", command->dataFrame());
        int form_count = gnomonFormManager::instance()->formCount(command->dataFrame()->formName());
        command->dataFrame()->metadata()->set("name", command->dataFrame()->formName().remove("gnomon") + QString::number(form_count+1));
        command->dataFrame()->metadata()->set("source", d->algorithm);
    }

}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
