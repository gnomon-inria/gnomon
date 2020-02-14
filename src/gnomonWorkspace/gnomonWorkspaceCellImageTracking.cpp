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

#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageTrackingPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspaceCellImageTrackingPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageTrackingCommand>
{
public:
     gnomonWorkspaceCellImageTrackingPrivate(void);
    ~gnomonWorkspaceCellImageTrackingPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *prev_view = nullptr;
    gnomonViewForm *next_view = nullptr;

    QHBoxLayout *view_layout = nullptr;
    QWidget *view_widget = nullptr;
    
    gnomonViewMatplotlib *mpl_figure = nullptr;

public:
    QStackedWidget *next_stack = nullptr;
    gnomonMessageBoard *next_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;

public:
    QVBoxLayout *mpl_layout = nullptr;
    QWidget *mpl_view = nullptr;
};

gnomonWorkspaceCellImageTrackingPrivate::gnomonWorkspaceCellImageTrackingPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageTrackingCommand >()
{

}

gnomonWorkspaceCellImageTrackingPrivate::~gnomonWorkspaceCellImageTrackingPrivate(void)
{

}

QString gnomonWorkspaceCellImageTrackingPrivate::workspace(void) const
{
    return "Cell Image Tracking";
}

QStringList gnomonWorkspaceCellImageTrackingPrivate::keys(void) const
{
    return gnomonCore::cellImageTracking::pluginFactory().keys();
}


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageTracking
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageTracking::gnomonWorkspaceCellImageTracking(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellImageTracking");

    d = new gnomonWorkspaceCellImageTrackingPrivate;

    d->prev_view = new gnomonViewForm(this);
    d->prev_view->setExportColor(this->color);
    d->prev_view->setInputView(true);
    d->prev_view->setEnableLinking(false);
    
    d->next_view = new gnomonViewForm(this);
    d->next_view->setExportColor(this->color);
    d->next_view->setInputView(false);
    d->next_view->setEnableLinking(false);

    d->mpl_figure = new gnomonViewMatplotlib(this);

    d->mpl_layout = new QVBoxLayout;
    d->mpl_layout->setContentsMargins(0, 0, 0, 0);
    d->mpl_layout->setSpacing(0);
    d->mpl_layout->addWidget(d->mpl_figure);

    d->mpl_view = new QWidget(this);
    d->mpl_view->setLayout(d->mpl_layout);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked next view
// /////////////////////////////////////////////////////////////////////////////

    d->next_message = new gnomonMessageBoard(this);
    d->next_message->setMessage("Result will be displayed here");

    d->next_stack = new QStackedWidget(this);
    d->next_stack->addWidget(d->next_message);
    d->next_stack->addWidget(d->next_view);

    d->view_layout = new QHBoxLayout;
    d->view_layout->setContentsMargins(0, 0, 0, 0);
    d->view_layout->setSpacing(0);
    d->view_layout->addWidget(d->prev_view);
    d->view_layout->addWidget(d->next_stack);

    d->view_widget = new QWidget(this);
    d->view_widget->setLayout(d->view_layout);

    d->splitter = new QSplitter(this);
    d->splitter->setOrientation(Qt::Vertical);
    d->splitter->addWidget(d->view_widget);
    d->splitter->addWidget(d->mpl_view);
    d->splitter->setSizes(QList<int>({800, 200}));

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

    connect(d->prev_view, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->prev_view->cellImage()) {
            d->command->setCellImage(d->prev_view->cellImage());
        }
        if(d->prev_view->image()) {
            d->command->setImage(d->prev_view->image());
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageTrackingPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if(d->prev_view->cellImage()) {
            d->command->setCellImage(d->prev_view->cellImage());
        }
        if(d->prev_view->image()) {
            d->command->setImage(d->prev_view->image());
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceCellImageTracking::~gnomonWorkspaceCellImageTracking(void)
{
    delete d;
}

void gnomonWorkspaceCellImageTracking::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageTracking::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageTracking::apply(void)
{
    Q_ASSERT(d->command);

    if(d->prev_view->cellImage()) {
        d->command->setCellImage(d->prev_view->cellImage());
    }
    if(d->prev_view->image()) {
        d->command->setImage(d->prev_view->image());
    }

    d->command->redo();

    qDebug()<<Q_FUNC_INFO<<d->command->cellImage();
    if(d->command->cellImage()) {
        d->next_view->setCellImage(dynamic_cast<gnomonCellImageSeries *>(d->command->cellImage()->clone()));
        d->next_view->setInputView(false);
        d->next_stack->setCurrentWidget(d->next_view);
    } else {
        d->next_stack->setCurrentWidget(d->next_message);
    }

    if(d->command->tree()) {
        d->mpl_figure->setForm("gnomonTree",d->command->tree());
    }
}

void gnomonWorkspaceCellImageTracking::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceCellImageTracking::color = QColor("#742ce1");

//
// gnomonWorkspaceCellImageTracking.cpp ends here
