#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

gnomonWorkspaceCellImageTracking::gnomonWorkspaceCellImageTracking(QObject *parent): gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageTracking");
    emit algorithmsLoaded();

    d->workspace = "Cellular Tracking";
    d->command   = new gnomonCellImageTrackingCommand;
    d->keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

    connect(d->command, &gnomonAbstractCommand::finished, [this]() { 
        this->viewOutputs();
        this->finished();
    });

    connect(this->target(), &gnomonViewForm::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
    connect(this->source(), &gnomonViewForm::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
}

gnomonWorkspaceCellImageTracking::~gnomonWorkspaceCellImageTracking(void)
{

}
