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

    this->sources()->addView();

    this->m_target_mpl = new gnomonViewMatplotlib(this);
    this->m_target_mpl->setAcceptForm("gnomonDataFrame",true);
    connect(this->m_target_mpl, &gnomonViewMatplotlib::exportedForm , d->pipeline, &gnomonPipeline::addForm);

    emit parametersChanged();
    d->updateViewFormTypes();
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
    if(command->dataFrame()) {
        m_target_mpl->setForm("gnomonDataFrame",command->dataFrame());
    }
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
