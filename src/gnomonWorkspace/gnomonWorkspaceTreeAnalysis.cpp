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

#include "gnomonWorkspaceTreeAnalysis.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeTransformCommand.h>
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

class gnomonWorkspaceTreeAnalysisPrivate : public gnomonWorkspaceTemplatePrivate<gnomonTreeTransformCommand>
{
public:
     gnomonWorkspaceTreeAnalysisPrivate(void);
    ~gnomonWorkspaceTreeAnalysisPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewMatplotlib *source = nullptr;
    gnomonViewMatplotlib *target = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;


public:
    QMetaObject::Connection c_o;
    QMetaObject::Connection c_s;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceTreeAnalysisPrivate::gnomonWorkspaceTreeAnalysisPrivate(void) : gnomonWorkspaceTemplatePrivate<gnomonTreeTransformCommand>()
{

}

gnomonWorkspaceTreeAnalysisPrivate::~gnomonWorkspaceTreeAnalysisPrivate(void)
{

}

QString gnomonWorkspaceTreeAnalysisPrivate::workspace(void) const
{
    return "TreeAnalysis";
}

QStringList gnomonWorkspaceTreeAnalysisPrivate::keys(void) const
{
    return gnomonCore::treeTransform::pluginFactory().keys();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceTreeAnalysis::gnomonWorkspaceTreeAnalysis(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("treeTransform");

    d = new gnomonWorkspaceTreeAnalysisPrivate;

    d->source = new gnomonViewMatplotlib(this);
    d->target = new gnomonViewMatplotlib(this);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->source);
    d->splitter->addWidget(d->target_stack);

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
//    layout->addWidget(d->source);
//    layout->addWidget(d->target);
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewMatplotlib::formAdded, [=] ()
    {

        if (d->command->input() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
                d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
            }
        } else {
            qDebug() << "Not changed";
        }
        d->configure(d->algorithm);
    });

//    connect(d, &gnomonWorkspaceTreeAnalysisPrivate::algorithmChanged, [=] (const QString& algorithm)
//    {
//        d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
//        d->configure(algorithm);
//    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceTreeAnalysis::~gnomonWorkspaceTreeAnalysis(void)
{
    delete d;
}

void gnomonWorkspaceTreeAnalysis::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeAnalysis::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeAnalysis::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

void gnomonWorkspaceTreeAnalysis::apply(void)
{
    Q_ASSERT(d->command);


    if (d->command->input() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
        if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonTree",d->command->output());
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

const QColor gnomonWorkspaceTreeAnalysis::color = QColor("#734906");

//
// gnomonWorkspaceTreeAnalysis.cpp ends here
