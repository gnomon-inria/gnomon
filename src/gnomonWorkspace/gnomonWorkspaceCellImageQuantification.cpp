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
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageQuantificationCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantificationPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspaceCellImageQuantificationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageQuantificationCommand>
{
public:
     gnomonWorkspaceCellImageQuantificationPrivate(void);
    ~gnomonWorkspaceCellImageQuantificationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    // gnomonViewForm *view = nullptr;
    // gnomonViewMatplotlib *mpl_figure = nullptr;

public:
    QVBoxLayout *mpl_layout = nullptr;
    QWidget *mpl_view = nullptr;
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


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageQuantification", &stat);

    d = new gnomonWorkspaceCellImageQuantificationPrivate;

    // d->view = new gnomonViewForm(this);
    // d->view->setExportColor(gnomonToolBar::cellImageQuantification_color);

    // d->mpl_figure = new gnomonViewMatplotlib(this);

    // d->mpl_layout = new QVBoxLayout;
    // d->mpl_layout->setContentsMargins(0, 0, 0, 0);
    // d->mpl_layout->setSpacing(0);
    // d->mpl_layout->addWidget(d->mpl_figure);

    // d->mpl_view = new QWidget(this);
    // d->mpl_view->setLayout(d->mpl_layout);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // layout->addWidget(d->view);
    // layout->addWidget(d->mpl_view);
    // layout->addWidget(d->pane(this));

    // connect(d->view, &gnomonViewForm::formAdded, [=] () {
    //     d->command->setCellImage(d->view->cellImage());
    //     d->command->setImage(d->view->image());
    //     d->configure(this, d->algorithm);
    // });

    // connect(d, &gnomonWorkspaceCellImageQuantificationPrivate::algorithmChanged, [=] (const QString& algorithm) {
    //     d->command->setCellImage(d->view->cellImage());
    //     d->command->setImage(d->view->image());
    //     d->configure(this,algorithm);
    // });
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d;
}

void gnomonWorkspaceCellImageQuantification::apply(void)
{
    Q_ASSERT(d->command);

    // d->command->setCellImage(d->view->cellImage());
    // d->command->setImage(d->view->image());

    // d->command->redo();

    // d->view->setCellImage(d->command->cellImage());

    // d->mpl_figure->setForm("gnomonDataFrame",d->command->dataFrame());
}

void gnomonWorkspaceCellImageQuantification::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
