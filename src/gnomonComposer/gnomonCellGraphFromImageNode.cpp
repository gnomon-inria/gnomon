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

#include "gnomonCellGraphFromImageNode.h"

#include "gnomonAbstractCellGraphFromImage.h"
#include "gnomonCellGraph.h"
#include <QtCore>

#include <dtkLog>
#include <dtkImage.h>

class gnomonCellGraphFromImageNodePrivate
{
public:
    dtkComposerTransmitterReceiver<dtkImage *> image_in;
    dtkComposerTransmitterReceiver<bool> use_margins;
    dtkComposerTransmitterReceiver<float> min_contact_area;
    dtkComposerTransmitterReceiver<long> background_label;
    dtkComposerTransmitterReceiver<QStringList> property_names;

    dtkComposerTransmitterEmitter<gnomonCellGraph *> graph_out;
};


gnomonCellGraphFromImageNode::gnomonCellGraphFromImageNode(void) : dtkComposerNodeObject<gnomonAbstractCellGraphFromImage>(), d(new gnomonCellGraphFromImageNodePrivate())
{
    this->setFactory(gnomonCore::graphFromImage::pluginFactory());

    this->appendReceiver(&d->image_in);
    this->appendReceiver(&d->use_margins);
    this->appendReceiver(&d->min_contact_area);
    this->appendReceiver(&d->background_label);
    this->appendReceiver(&d->property_names);

    this->appendEmitter (&d->graph_out);
}

gnomonCellGraphFromImageNode::~gnomonCellGraphFromImageNode(void)
{
    delete d;
}


void gnomonCellGraphFromImageNode::run(void)
{
    if (d->image_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input image is not set. Aborting.";
        return;

    } else {

        gnomonAbstractCellGraphFromImage *graphFromImage = this->object();
        if (!graphFromImage) {
            dtkError() << Q_FUNC_INFO << "No Graph from Image found. Aborting.";
            return;
        }

        graphFromImage->setImage(d->image_in.data());

        if(!d->use_margins.isEmpty()) {
            graphFromImage->setUseMargins(d->use_margins.data());
        }
        if(!d->min_contact_area.isEmpty()) {
            graphFromImage->setMinContactArea(d->min_contact_area.data());
        }
        if(!d->background_label.isEmpty()) {
            graphFromImage->setBackgroundLabel(d->background_label.data());
        }
        if(!d->property_names.isEmpty()) {
            graphFromImage->setPredefinedProperties(d->property_names.data());
        }

        graphFromImage->run();

        gnomonCellGraph *cellgraph = graphFromImage->computedGraph();


        d->graph_out.setData(graphFromImage->computedGraph());
    }
}

//
// gnomonCellGraphFromImageNode.cpp ends here
