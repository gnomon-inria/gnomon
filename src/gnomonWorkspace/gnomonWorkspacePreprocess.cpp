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
    std::cout<<"**************************"<<std::endl;
    std::cout<< "Before call1" << std::endl;
    std::cout<<"**************************"<<std::endl;

    d->command->undo();
    std::cout<<"**************************"<<std::endl;
    std::cout<< "Before call2" << std::endl;
    std::cout<<"**************************"<<std::endl;

    gnomonImageFilterCommand *command = static_cast<gnomonImageFilterCommand *>(d->command);
    std::cout<<"**************************"<<std::endl;
    std::cout<< command << std::endl;
    std::cout<<"**************************"<<std::endl;
    for(gnomonViewForm *view : d->sources->views()){
        if(auto image = view->image()){
            command->setInput(image);
            std::cout<<"first cond..."<<std::endl;
        }
        std::cout<<"**************************"<<std::endl;
        std::cout<<view->binaryImage()<<endl;
        std::cout<<"**************************"<<std::endl;
        if(auto mask = view->binaryImage()){
            command->setMask(mask);
            std::cout<<"last cond..."<<std::endl;
        }
    }
}

//
// gnomonWorkspacePreprocess.cpp ends here
