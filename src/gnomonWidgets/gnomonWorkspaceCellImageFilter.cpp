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

#include "gnomonWorkspaceCellImageFilter.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonToolBar.h"

//#include "gnomonViewVolumic.h"
//#include "gnomonViewVolumicPool.h"

#include "gnomonViewForm.h"
#include "gnomonViewFormPool.h"

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore/gnomonCellImage>
#include <gnomonCore/gnomonCellImageFilterCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceCellImageFilterPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageFilterCommand>
{
public:
     gnomonWorkspaceCellImageFilterPrivate(void);
    ~gnomonWorkspaceCellImageFilterPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;
//    gnomonViewVolumic *source = nullptr;
//    gnomonViewVolumic *target = nullptr;
//
public:
    gnomonViewFormPool *pool = nullptr;
};

gnomonWorkspaceCellImageFilterPrivate::gnomonWorkspaceCellImageFilterPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageFilterCommand >()
{

}

gnomonWorkspaceCellImageFilterPrivate::~gnomonWorkspaceCellImageFilterPrivate(void)
{

}

QString gnomonWorkspaceCellImageFilterPrivate::workspace(void) const
{
    return "Label Postprocess";
}

QStringList gnomonWorkspaceCellImageFilterPrivate::keys(void) const
{
    return gnomonCore::cellImageFilter::pluginFactory().keys();
}

gnomonWorkspaceCellImageFilter::gnomonWorkspaceCellImageFilter(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageFilter", &stat);

    d = new gnomonWorkspaceCellImageFilterPrivate;

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
    layout->addWidget(d->pane(this));

    connect(d->source, &gnomonViewForm::formAdded, [=] () {
        if(d->command->input() != d->source->cellImage())
            d->command->setInput(d->source->cellImage());
        else
            qDebug() << "Not changed";
        d->configure(this, d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageFilterPrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setInput(d->source->cellImage());
        d->configure(this,algorithm);
    });
}

gnomonWorkspaceCellImageFilter::~gnomonWorkspaceCellImageFilter(void)
{
    delete d;
}

void gnomonWorkspaceCellImageFilter::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->cellImage())
        d->command->setInput(d->source->cellImage());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setCellImage(d->command->output());
}

void gnomonWorkspaceCellImageFilter::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceCellImageFilter.cpp ends here
