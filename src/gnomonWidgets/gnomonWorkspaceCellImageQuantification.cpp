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

#include "gnomonWorkspaceCellImageQuantification.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonToolBar.h"

#include "gnomonViewForm.h"

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore/gnomonCellImage>
#include <gnomonCore/gnomonImagesSerie>
#include <gnomonCore/gnomonCellImageQuantificationCommand>

#include <dtkWidgets>
#include <dtkImagingCore>
#include <dtkScript>

class gnomonWorkspaceCellImageQuantificationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageQuantificationCommand>
{
public:
     gnomonWorkspaceCellImageQuantificationPrivate(void);
    ~gnomonWorkspaceCellImageQuantificationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *view = nullptr;

public:
    dtkInterpreter *terminal;
    
};

gnomonWorkspaceCellImageQuantificationPrivate::gnomonWorkspaceCellImageQuantificationPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageQuantificationCommand >()
{

}

gnomonWorkspaceCellImageQuantificationPrivate::~gnomonWorkspaceCellImageQuantificationPrivate(void)
{

}

QString gnomonWorkspaceCellImageQuantificationPrivate::workspace(void) const
{
    return "Cell Quantification";
}

QStringList gnomonWorkspaceCellImageQuantificationPrivate::keys(void) const
{
    return gnomonCore::cellImageQuantification::pluginFactory().keys();
}

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageQuantification", &stat);

    d = new gnomonWorkspaceCellImageQuantificationPrivate;

    d->view = new gnomonViewForm(this);
    d->view->setExportColor(gnomonToolBar::preprocess_color);

    d->terminal = new dtkInterpreter(this);
    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->view);
    layout->addWidget(d->terminal);
    layout->addWidget(d->pane(this));

    connect(d->view, &gnomonViewForm::formAdded, [=] () {
        d->command->setCellImage(d->view->cellImage());
        d->command->setImage(d->view->imagesSerie());
        d->configure(this, d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageQuantificationPrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->setCellImage(d->view->cellImage());
        d->command->setImage(d->view->imagesSerie());
        d->configure(this,algorithm);
    });
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d;
}

void gnomonWorkspaceCellImageQuantification::apply(void)
{
    Q_ASSERT(d->command);

    d->command->setCellImage(d->view->cellImage());
    d->command->setImage(d->view->imagesSerie());

    d->command->redo();

    d->view->setCellImage(d->command->cellImage());
}

void gnomonWorkspaceCellImageQuantification::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
