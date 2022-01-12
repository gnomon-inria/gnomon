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
    

    //create the views
    this->sources()->addView();
    this->targets()->addView();

    emit parametersChanged();

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

    for(gnomonViewForm *view : d->sources->views()){
        if(auto image = view->image()){
            command->setInput(image);
        }
        if(auto mask = view->binaryImage()){
            command->setMask(mask);
        }
    }
}

QStringList gnomonWorkspacePreprocess::availablePlugins() {
    return QStringList();
}

//
// gnomonWorkspacePreprocess.cpp ends here
