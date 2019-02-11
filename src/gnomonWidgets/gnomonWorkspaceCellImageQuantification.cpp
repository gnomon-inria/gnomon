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

#include <gnomonStyle>

#include <dtkWidgets>
#include <dtkImagingCore>
#include <dtkScript>


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
    gnomonViewForm *view = nullptr;

public:
    dtkInterpreter *terminal;
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

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonCellImageQuantification", &stat);

    d = new gnomonWorkspaceCellImageQuantificationPrivate;

    d->view = new gnomonViewForm(this);
    d->view->setExportColor(gnomonToolBar::cellImageQuantification_color);

    d->terminal = new dtkInterpreter(this);
    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    d->mpl_layout = new QVBoxLayout;
    d->mpl_layout->setContentsMargins(0, 0, 0, 0);
    d->mpl_layout->setSpacing(0);
    d->mpl_layout->addWidget(d->terminal);
    
    d->mpl_view = new QWidget(this);
    d->mpl_view->setLayout(d->mpl_layout);
    d->mpl_view->resize(800,d->mpl_view->height());
    d->mpl_view->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
    
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->view);
    layout->addWidget(d->mpl_view);
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

    this->setObjectName("QuantificationWorkspace");

    QFile file(":gnomon/matplotlib_figure.py");

    if (file.open(QIODevice::ReadOnly)) {
        int stat;
        QString matplotlib_script  = file.readAll();
        file.close();
        d->terminal->output(dtkScriptInterpreterPython::instance()->interpret(matplotlib_script, &stat));
    } else {
        qWarning() << "Can't open matplotlib figure script";
    }
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d;
}

void gnomonWorkspaceCellImageQuantification::addView(QWidget *figure)
{
    if(!d->terminal)
        return;

    d->terminal->hide();
    d->terminal->deleteLater();
    d->terminal = Q_NULLPTR;

    figure->setStyleSheet(gnomonStyleSheet());

    d->mpl_layout->addWidget(figure);
    d->mpl_view->resize(800,d->mpl_view->height());
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
