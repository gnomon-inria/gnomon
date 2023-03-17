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

#include "gnomonWorkspacePointCloudQuantification.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudQuantificationCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePointCloudQuantificationPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePointCloudQuantificationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonPointCloudQuantificationCommand>
{
public:
     gnomonWorkspacePointCloudQuantificationPrivate(void);
    ~gnomonWorkspacePointCloudQuantificationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *view = nullptr;
    gnomonViewMatplotlib *mpl_figure = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;

public:
    QVBoxLayout *mpl_layout = nullptr;
    QWidget *mpl_view = nullptr;
};

gnomonWorkspacePointCloudQuantificationPrivate::gnomonWorkspacePointCloudQuantificationPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonPointCloudQuantificationCommand >()
{

}

gnomonWorkspacePointCloudQuantificationPrivate::~gnomonWorkspacePointCloudQuantificationPrivate(void)
{

}

QString gnomonWorkspacePointCloudQuantificationPrivate::workspace(void) const
{
    return "Point Cloud Analysis";
}

QStringList gnomonWorkspacePointCloudQuantificationPrivate::keys(void) const
{
    return gnomonCore::pointCloudQuantification::pluginFactory().keys();
}


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePointCloudQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspacePointCloudQuantification::gnomonWorkspacePointCloudQuantification(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("pointCloudQuantification");

    d = new gnomonWorkspacePointCloudQuantificationPrivate;

    d->view = new gnomonViewForm(this);
    d->view->setNodePortNames({});
    d->view->setExportColor(this->color);
    d->view->setAcceptForm("gnomonPointCloud",true);
    d->view->setAcceptForm("gnomonImage",true);
    d->view->setInputView(true);
    d->view->setEnableLinking(false);

    connect(d->view, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline_manager, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->mpl_figure = new gnomonViewMatplotlib(this);
    d->mpl_figure->setAcceptForm("gnomonDataFrame",true);

    connect(d->mpl_figure, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline_manager, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->mpl_layout = new QVBoxLayout;
    d->mpl_layout->setContentsMargins(0, 0, 0, 0);
    d->mpl_layout->setSpacing(0);
    d->mpl_layout->addWidget(d->mpl_figure);

    d->mpl_view = new QWidget(this);
    d->mpl_view->setLayout(d->mpl_layout);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->mpl_figure);


    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->view);
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
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->view, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->view->pointCloud()) {
            d->command->setPointCloud(d->view->pointCloud());
        }
        if(d->view->image()) {
            d->command->setImage(d->view->image());
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspacePointCloudQuantificationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if(d->view->pointCloud()) {
            d->command->setPointCloud(d->view->pointCloud());
        }
        if(d->view->image()) {
            d->command->setImage(d->view->image());
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

//  connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspacePointCloudQuantification::~gnomonWorkspacePointCloudQuantification(void)
{
    delete d;
}

void gnomonWorkspacePointCloudQuantification::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->view->menu());
//    dtkApp->window()->menubar()->addMenu(d->mpl_figure->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudQuantification::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->view->menu());
//    dtkApp->window()->menubar()->removeMenu(d->mpl_figure->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudQuantification::apply(void)
{
    Q_ASSERT(d->command);

    if(d->view->pointCloud()) {
        d->command->setPointCloud(d->view->pointCloud());
    }
    if(d->view->image()) {
        d->command->setImage(d->view->image());
    }

    d->view->setInputView(true);

    d->command->redo();
    
    if(d->command->pointCloud() != nullptr | d->command->dataFrame() != nullptr) {
        d->registerPipeline();
    }

    if(d->command->pointCloud()) {
        d->view->setPointCloud(d->command->pointCloud());
        //d->pipeline_manager->addClonedForm(d->command->pointCloud(),d->view->pointCloud());
        d->pipeline_manager->addForm(d->command->pointCloud());
        d->view->setInputView(false);
        d->view->setAcceptDrops(true);
    }

    if(d->command->dataFrame()) {
        d->target_stack->setCurrentWidget(d->mpl_figure);
        d->mpl_figure->setForm("gnomonDataFrame",d->command->dataFrame());
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspacePointCloudQuantification::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspacePointCloudQuantification::color = QColor("#d94c64");

bool gnomonWorkspacePointCloudQuantification::isEmpty(void)
{
    return gnomonWorkspacePointCloudQuantificationPrivate::isEmpty();
}

//
// gnomonWorkspacePointCloudQuantification.cpp ends here
