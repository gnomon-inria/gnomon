#include "gnomonWorkspacePreprocess.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonProject"


#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFilter.h>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePreprocess
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("imageFilter");

    d->workspace = "Preprocess";
    d->command   = new gnomonImageFilterCommand;
    d->keys = gnomonCore::imageFilter::pluginFactory().keys();
    d->algorithmsData = gnomonCore::imageFilter::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    QJsonObject workspace_info = GNOMON_PROJECT->workspaceAlgoInfo("Preprocess");
    if(!workspace_info.isEmpty())
        this->m_uuid = workspace_info.keys()[0];
    if(this->m_uuid.isEmpty()) {
        this->m_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        QJsonObject data_json;
        data_json.insert("workspace_name", "Preprocess");
        workspace_info.insert(this->m_uuid, data_json);
        GNOMON_PROJECT->addToManifest(workspace_info);
    }

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    gnomonImageFilterCommand *command = (gnomonImageFilterCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspacePreprocess.cpp ends here
