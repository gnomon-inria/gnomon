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

#include "gnomonWorkspaceSegmentation.h"

//#include "gnomonViewVolumic.h"
//#include "gnomonViewVolumicPool.h"

#include "gnomonViewForm.h"
#include "gnomonViewFormPool.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore/gnomonImagesSerie.h>
#include <gnomonCore/gnomonCellImage.h>
#include <gnomonCore/gnomonSegmentationCommand.h>

#include <dtkImagingCore>
#include <dtkScript>

#include <QtWidgets>

#include <vtkImageData.h>
#include <vtkRenderer.h>

class gnomonWorkspaceSegmentationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonSegmentationCommand>
{

public:
     gnomonWorkspaceSegmentationPrivate(void);
    ~gnomonWorkspaceSegmentationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;
//    gnomonViewVolumic *source = nullptr;
//    gnomonViewVolumic *target = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    gnomonCellImage *cellimage = nullptr;

public:
    QMetaObject::Connection c_o;
    QMetaObject::Connection c_s;
};

gnomonWorkspaceSegmentationPrivate::gnomonWorkspaceSegmentationPrivate(void) : gnomonWorkspaceTemplatePrivate<gnomonSegmentationCommand>()
{

}

gnomonWorkspaceSegmentationPrivate::~gnomonWorkspaceSegmentationPrivate(void)
{

}

QString gnomonWorkspaceSegmentationPrivate::workspace(void) const
{
    return "Segmentation";
}

QStringList gnomonWorkspaceSegmentationPrivate::keys(void) const
{
    return gnomonCore::cellImageFromImage::pluginFactory().keys();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageFromImage", &stat);

    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewForm(this);
    d->target = new gnomonViewForm(this);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    gnomonOverlayPane *pane = d->pane(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(d->source, &gnomonViewForm::formAdded, [=] () {
        qDebug()<<Q_FUNC_INFO<<d->source->imagesSerie()->channels();
        if(d->command->input() != d->source->imagesSerie())
            d->command->setInput(d->source->imagesSerie());
        else
            qDebug() << "Not changed";
        d->configure(this, d->algorithm);
    });
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    delete d;
}

void gnomonWorkspaceSegmentation::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

void gnomonWorkspaceSegmentation::apply(void)
{
    Q_ASSERT(d->command);


    d->target->render();

    if(d->command->input() != d->source->imagesSerie())
        d->command->setInput(d->source->imagesSerie());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setForm("Segmented Image",d->command->output());
}
//
// gnomonWorkspaceSegmentation.cpp ends here
