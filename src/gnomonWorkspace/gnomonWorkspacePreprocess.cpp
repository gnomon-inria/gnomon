// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonWorkspacePreprocess.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand.h>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePreprocess
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    loadPluginGroup("imageFilter");
    emit algorithmsLoaded();

    d->workspace = "Preprocess";
    d->command   = new gnomonImageFilterCommand;
    d->keys = gnomonCore::imageFilter::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();
    emit parametersChanged();

    //create the views
    this->sources()->addView();
    this->targets()->addView();

    d->updateViewFormTypes();
    d->updatePool();
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    gnomonImageFilterCommand *command = (gnomonImageFilterCommand *)d->command;
    if (command) {
        delete command;
    }
}

void gnomonWorkspacePreprocess::setInputs()
{
    d->command->undo();

    gnomonImageFilterCommand *command = static_cast<gnomonImageFilterCommand *>(d->command);
    //gnomonBinaryImageFromImageCommand *command = static_cast<gnomonBinaryImageFromImageCommand *>(d->command);
    for(gnomonViewForm *view : d->sources->views()){
        if(auto image = view->image()){
            command->setInput(image);
        }
        if(auto initialization = view->binaryImage()){
            command->setMask(initialization);
        }
    }
}

//
// gnomonWorkspacePreprocess.cpp ends here
