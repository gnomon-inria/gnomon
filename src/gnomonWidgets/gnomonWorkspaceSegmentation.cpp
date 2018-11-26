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

#include "gnomonViewVolumic.h"
#include "gnomonViewVolumicPool.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonVisualization/gnomonPolyDataCellImage.h>
#include <gnomonVisualization/gnomonActor2DCellImage.h>
#include <gnomonVisualization/gnomonActorMeshCellImage.h>

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
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;

public:
    gnomonViewVolumicPool *pool = nullptr;

public:
    gnomonCellImage *cellimage = nullptr;

public:
    gnomonPolyDataCellImage *polydata = nullptr;
    gnomonActorMeshCellImage *actor = nullptr;
    gnomonActor2DCellImage *actor2D = nullptr;

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

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    d->pool = new gnomonViewVolumicPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    QPushButton *cell_button = new QPushButton("Compute cells", this);

    gnomonOverlayPaneItem *visu_item = new gnomonOverlayPaneItem(this);
    visu_item->setTitle("Segmentation");
    visu_item->addWidget(cell_button);
    visu_item->toggle();

    gnomonOverlayPane *pane = d->pane(this);
    pane->addWidget(visu_item);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(pane);

    connect(cell_button, &QPushButton::clicked, [=]() {
                                                    this->setCursor(Qt::BusyCursor);
                                                    this->computeCells();
                                                    this->setCursor(Qt::BusyCursor);});
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

    if (d->actor) {
        d->target->renderer3D()->RemoveActor(d->actor);
        d->actor->Delete();
        d->actor = nullptr;
    }

    if (d->actor2D) {
        d->target->disconnect(d->c_o);
        d->target->disconnect(d->c_s);
        d->target->renderer2D()->RemoveActor(d->actor2D);
        d->actor2D->Delete();
        d->actor2D = nullptr;
    }

    if (d->polydata) {
        d->polydata->Delete();
        d->polydata = nullptr;
    }

    d->target->render();

    d->command->setImage(d->source->image().data());
    d->command->redo();

    d->target->setImage(dtkImagePtr(new dtkImage(*d->command->computedImage()->image())));
}

void gnomonWorkspaceSegmentation::computeCells(void)
{
    if(!d->polydata)
        d->polydata = gnomonPolyDataCellImage::New();

    d->polydata->setCellImage((gnomonCellImage *)d->command->computedImage()->clone());

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    if(!converter)
        return;

    dtkImage *image = d->command->computedImage()->image();
    converter->setInput(image);

    if(!converter->convert())
        return;

    vtkImageData *volume = static_cast<vtkImageData *>(converter->output());

    if(!d->actor)
        d->actor = gnomonActorMeshCellImage::New();
    d->actor->setPolyData(d->polydata);

    d->target->renderer3D()->AddActor(d->actor);

    if(!d->actor2D)
        d->actor2D = gnomonActor2DCellImage::New();
    d->actor2D->setInteractor(d->target->interactor());
    d->actor2D->setDimensions(volume->GetDimensions());
    d->actor2D->setSpacing(volume->GetSpacing());
    d->actor2D->setPolyData(d->polydata);

    d->target->renderer2D()->AddActor(d->actor2D);

    d->c_o = connect(d->target, &gnomonViewVolumic::sliceOrientationChanged, [=] (int value) {
        d->actor2D->setSliceOrientation(value);
    });

    d->c_s = connect(d->target, &gnomonViewVolumic::sliceChanged, [=] (int value) {
        d->actor2D->setSlice(value);
    });

    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
