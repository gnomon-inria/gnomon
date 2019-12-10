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
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonSegmentationCommand.h>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

#include <QtWidgets>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

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

public:
    gnomonViewFormPool *pool = nullptr;

public:
    // gnomonCellImage *cellimage = nullptr;

public:
    QMetaObject::Connection c_o;
    QMetaObject::Connection c_s;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
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

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellImageFromImage");

    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::segmentation_color);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::segmentation_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if (d->command->input() != d->source->image())
            d->command->setInput(d->source->image());
        else
            qDebug() << "Not changed";
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceSegmentationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->setInput(d->source->image());
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    delete d;
}

void gnomonWorkspaceSegmentation::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    d->source->menubar()->touch();
    d->target->menubar()->touch();
}

void gnomonWorkspaceSegmentation::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    d->source->menubar()->touch();
    d->target->menubar()->touch();
}

void gnomonWorkspaceSegmentation::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

void gnomonWorkspaceSegmentation::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != d->source->image())
        d->command->setInput(d->source->image());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setForm("gnomonCellImage",d->command->output());
    d->target->render();
}

//
// gnomonWorkspaceSegmentation.cpp ends here
