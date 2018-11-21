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

#include "gnomonWorkspaceSimulation.h"
#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate.h"

#include <gnomonImagesSerieFilterCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>

class gnomonWorkspaceSimulationPrivate : public gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >
{
public:
    gnomonWorkspaceSimulationPrivate();
    virtual ~gnomonWorkspaceSimulationPrivate();

public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;
};

gnomonWorkspaceSimulationPrivate::gnomonWorkspaceSimulationPrivate() : gnomonWorkspaceTemplatePrivate< gnomonImagesSerieFilterCommand >()
{
}

gnomonWorkspaceSimulationPrivate::~gnomonWorkspaceSimulationPrivate()
{
}

QString gnomonWorkspaceSimulationPrivate::workspace() const
{ return "Simulation"; }

QStringList gnomonWorkspaceSimulationPrivate::keys() const
{
    return gnomonCore::imagesSerieFilter::pluginFactory().keys();
}

gnomonWorkspaceSimulation::gnomonWorkspaceSimulation(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    //dtkScriptInterpreterPython::instance()->interpret("import gnomonImagesSerieFilter", &stat);

    d = new gnomonWorkspaceSimulationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->pane(this));
}

gnomonWorkspaceSimulation::~gnomonWorkspaceSimulation(void)
{
    delete d;
}

void gnomonWorkspaceSimulation::apply(void)
{

}

void gnomonWorkspaceSimulation::configure(const QString& algorithm)
{
    //d->configure(this, algorithm);
}

//
// gnomonWorkspaceSimulation.cpp ends here
