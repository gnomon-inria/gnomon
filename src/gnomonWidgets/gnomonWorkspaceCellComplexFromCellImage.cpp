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

#include "gnomonWorkspaceCellComplexFromCellImage.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonToolBar.h"

//#include "gnomonViewVolumic.h"
//#include "gnomonViewVolumicPool.h"

#include "gnomonViewForm.h"
#include "gnomonViewFormPool.h"

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexFromCellImageCommand>

#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceCellComplexFromCellImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellComplexFromCellImageCommand>
{
public:
     gnomonWorkspaceCellComplexFromCellImagePrivate(void);
    ~gnomonWorkspaceCellComplexFromCellImagePrivate(void);

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

gnomonWorkspaceCellComplexFromCellImagePrivate::gnomonWorkspaceCellComplexFromCellImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellComplexFromCellImageCommand >()
{

}

gnomonWorkspaceCellComplexFromCellImagePrivate::~gnomonWorkspaceCellComplexFromCellImagePrivate(void)
{

}

QString gnomonWorkspaceCellComplexFromCellImagePrivate::workspace(void) const
{
    return "Cell Reconstruction";
}

QStringList gnomonWorkspaceCellComplexFromCellImagePrivate::keys(void) const
{
    return gnomonCore::cellComplexFromCellImage::pluginFactory().keys();
}

gnomonWorkspaceCellComplexFromCellImage::gnomonWorkspaceCellComplexFromCellImage(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellComplexFromCellImage", &stat);

    d = new gnomonWorkspaceCellComplexFromCellImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);
    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);

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

    connect(d, &gnomonWorkspaceCellComplexFromCellImagePrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setInput(d->source->cellImage());
        d->configure(this,algorithm);
    });
}

gnomonWorkspaceCellComplexFromCellImage::~gnomonWorkspaceCellComplexFromCellImage(void)
{
    delete d;
}

void gnomonWorkspaceCellComplexFromCellImage::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->cellImage())
        d->command->setInput(d->source->cellImage());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setCellComplex(d->command->output());
}

void gnomonWorkspaceCellComplexFromCellImage::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceCellComplexFromCellImage.cpp ends here
