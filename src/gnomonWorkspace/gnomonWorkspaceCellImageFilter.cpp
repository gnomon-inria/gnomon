#include "gnomonWorkspaceCellImageFilter.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonProject"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFilter.h>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFilterCommand.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageFilter
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageFilter::gnomonWorkspaceCellImageFilter(QObject *parent):gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageFilter");

    d->workspace = "CellImage Filter";
    d->command = new gnomonCellImageFilterCommand;
    d->keys = gnomonCore::cellImageFilter::pluginFactory().keys();
    d->algorithmsData = gnomonCore::cellImageFilter::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();
}

gnomonWorkspaceCellImageFilter::~gnomonWorkspaceCellImageFilter(void)
{
    gnomonCellImageFilterCommand *command = (gnomonCellImageFilterCommand *)d->command;
    if (command) {
        delete command;
    }
}


//
// gnomonWorkspaceCellImageFilter.cpp ends here
