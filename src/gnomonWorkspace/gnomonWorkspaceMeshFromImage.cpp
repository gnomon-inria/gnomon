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

#include "gnomonWorkspaceMeshFromImage.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshFromImageCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceMeshFromImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonMeshFromImageCommand>
{
public:
     gnomonWorkspaceMeshFromImagePrivate(void);
    ~gnomonWorkspaceMeshFromImagePrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    dtkWidgetsMenu *menu_;    
};

gnomonWorkspaceMeshFromImagePrivate::gnomonWorkspaceMeshFromImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonMeshFromImageCommand >()
{

}

gnomonWorkspaceMeshFromImagePrivate::~gnomonWorkspaceMeshFromImagePrivate(void)
{

}

QString gnomonWorkspaceMeshFromImagePrivate::workspace(void) const
{
    return "Surface Meshing";
}

QStringList gnomonWorkspaceMeshFromImagePrivate::keys(void) const
{
    return gnomonCore::meshFromImage::pluginFactory().keys();
}

gnomonWorkspaceMeshFromImage::gnomonWorkspaceMeshFromImage(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonMeshFromImage", &stat);

    d = new gnomonWorkspaceMeshFromImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::meshFromImage_color);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::meshFromImage_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

    d->menu_ = d->menu(this);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////
    
    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->command->input() != d->source->image())
            d->command->setInput(d->source->image());
        else
            qDebug() << "Not changed";
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceMeshFromImagePrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->setInput(d->source->image());
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceMeshFromImage::~gnomonWorkspaceMeshFromImage(void)
{
    delete d;
}

void gnomonWorkspaceMeshFromImage::enter(void)
{
    dtkApp->window()->menubar()->addMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceMeshFromImage::leave(void)
{
    dtkApp->window()->menubar()->removeMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceMeshFromImage::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != d->source->image())
        d->command->setInput(d->source->image());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setMesh(d->command->output());
}

void gnomonWorkspaceMeshFromImage::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceMeshFromImage.cpp ends here
