#include "gnomonWorkspaceSegmentation.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand.h>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceSegmentation
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("cellImageFromImage");
    emit algorithmsLoaded();

    d->workspace = "Segmentation";
    d->command   = new gnomonCellImageFromImageCommand;
    d->keys = gnomonCore::cellImageFromImage::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

    //create the views
    this->sources()->addView();
    this->targets()->addView();

    emit parametersChanged();

    d->updateViewFormTypes();
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    gnomonCellImageFromImageCommand *command = (gnomonCellImageFromImageCommand *)d->command;
    if (command) {
        delete command;
    }
    delete d;
}

void gnomonWorkspaceSegmentation::setInputs()
{
    d->command->undo(); //clean

    gnomonCellImageFromImageCommand *command = static_cast<gnomonCellImageFromImageCommand *>(d->command);
    for(gnomonViewForm *f : d->sources->views()) {
        if (f->image()) {
            command->setInput(f->image());
        }
        if (f->pointCloud()) {
            command->setCellPoints(f->pointCloud());
        }
    }

}

//
// gnomonWorkspaceSegmentation.cpp ends here
