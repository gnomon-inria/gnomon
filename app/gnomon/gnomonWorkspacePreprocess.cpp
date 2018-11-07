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
#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include <gnomonImagesSerieFilterCommand.h>

#include <QtWidgets>

class gnomonWorkspacePreprocessPrivate
{
public:
    gnomonViewVolumic *source;
    gnomonViewVolumic *target;
};

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : QWidget(parent)
{
    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QPushButton *button = new QPushButton("Apply", this);

    gnomonOverlayPaneItem *item = new gnomonOverlayPaneItem(this);
    item->setTitle("Normalization");
    item->addWidget(button);
    item->toggle();

    gnomonOverlayPane *pane = new gnomonOverlayPane(this);
    pane->addWidget(item);
    pane->toggle();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(button, SIGNAL(clicked()), this, SLOT(apply()));
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::apply(void)
{
    gnomonImagesSerieFilterCommand *image_filter_command = new gnomonImagesSerieFilterCommand("gnomonImagesSerieFilter");

    Q_ASSERT(image_filter_command);

    image_filter_command->setImage(d->source->image());
    image_filter_command->redo();

    dtkImage *img = image_reader_command->next();

    if (!img) {
        qDebug() << Q_FUNC_INFO << "Resulting image is void.";
        event->ignore();
        return;
    }

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(img);
    converter->convert();
    d->target->setImage(static_cast<vtkImageData *>(converter->output()));

    delete converter;

    qDebug() << Q_FUNC_INFO;
}

//
// gnomonWorkspacePreprocess.cpp ends here
