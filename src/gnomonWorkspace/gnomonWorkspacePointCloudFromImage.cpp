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

#include "gnomonWorkspacePointCloudFromImage.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudFromImageCommand>
#include <gnomonWidgets>

#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspacePointCloudFromImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonPointCloudFromImageCommand>
{
public:
     gnomonWorkspacePointCloudFromImagePrivate(void);
    ~gnomonWorkspacePointCloudFromImagePrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspacePointCloudFromImagePrivate::gnomonWorkspacePointCloudFromImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonPointCloudFromImageCommand >()
{

}

gnomonWorkspacePointCloudFromImagePrivate::~gnomonWorkspacePointCloudFromImagePrivate(void)
{

}

QString gnomonWorkspacePointCloudFromImagePrivate::workspace(void) const
{
    return "Cell Detection";
}

QStringList gnomonWorkspacePointCloudFromImagePrivate::keys(void) const
{
    return gnomonCore::pointCloudFromImage::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePointCloudFromImage::gnomonWorkspacePointCloudFromImage(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("pointCloudFromImage");

    d = new gnomonWorkspacePointCloudFromImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(this->color);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);
    d->pool->linkAll();

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
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if (d->command->input() != d->source->image()) {
            if (d->source->image()) {
                d->command->setInput(d->source->image());
            }
        } else {
            qDebug() << "Not changed";
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspacePointCloudFromImagePrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (d->source->image()) {
            d->command->setInput(d->source->image());
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspacePointCloudFromImage::~gnomonWorkspacePointCloudFromImage(void)
{
    delete d;
}

void gnomonWorkspacePointCloudFromImage::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudFromImage::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudFromImage::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != d->source->image()) {
        if (d->source->image()) {
            d->command->setInput(d->source->image());
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonPointCloud",d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
        d->source->setEnableLinking(false);
        d->target->setEnableLinking(false);
    }
}

void gnomonWorkspacePointCloudFromImage::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspacePointCloudFromImage::color = QColor("#209820");

//
// gnomonWorkspacePointCloudFromImage.cpp ends here
