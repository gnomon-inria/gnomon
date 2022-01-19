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
#include <gnomonVisualization>

#include <dtkCore>
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
    gnomonViewMatplotlib *mpl_figure = nullptr;

//public:
//    QStackedWidget *target_stack = nullptr;
//    gnomonMessageBoard *target_message = nullptr;
//
//    QSplitter *splitter = nullptr;

//public:
//    dtkWidgetsMenu *menu_;
//
//public:
//    dtkWidgetsMenuBarContainer *dashboard;
//
//public:
//    QVBoxLayout *mpl_layout = nullptr;
//    QWidget *mpl_view = nullptr;
};

gnomonWorkspaceCellImageQuantificationPrivate::gnomonWorkspaceCellImageQuantificationPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageQuantificationCommand >()
{
    this->command   = new gnomonCellImageQuantificationCommand;
    this->algorithm = this->command->algorithmName();
}

gnomonWorkspaceCellImageQuantificationPrivate::~gnomonWorkspaceCellImageQuantificationPrivate(void)
{

}

QString gnomonWorkspaceCellImageQuantificationPrivate::workspace(void) const
{
    return "Cell Analysis";
}

QStringList gnomonWorkspaceCellImageQuantificationPrivate::keys(void) const
{
    return gnomonCore::cellImageQuantification::pluginFactory().keys();
}


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QObject *parent) : QObject(parent)
{
    loadPluginGroup("cellImageQuantification");

    d = new gnomonWorkspaceCellImageQuantificationPrivate;

    d->view = new gnomonViewForm(this);
//    d->view->setExportColor(this->color);
    d->view->setInputView(true);
    d->view->setAcceptForm("gnomonCellImage",true);
    d->view->setAcceptForm("gnomonImage",true);

    connect(d->view, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->mpl_figure = new gnomonViewMatplotlib(this);
    d->mpl_figure->setAcceptForm("gnomonDataFrame",true);

    connect(d->mpl_figure, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

//    d->mpl_layout = new QVBoxLayout;
//    d->mpl_layout->setContentsMargins(0, 0, 0, 0);
//    d->mpl_layout->setSpacing(0);
//    d->mpl_layout->addWidget(d->mpl_figure);

//    d->mpl_view = new QWidget(this);
//    d->mpl_view->setLayout(d->mpl_layout);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

//    d->target_message = new gnomonMessageBoard(this);
//    d->target_message->setMessage("Result will be displayed here");
//
//    d->target_stack = new QStackedWidget(this);
//    d->target_stack->addWidget(d->target_message);
//    d->target_stack->addWidget(d->mpl_figure);
//
//
//    d->splitter = new QSplitter(this);
//    d->splitter->addWidget(d->view);
//    d->splitter->addWidget(d->target_stack);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

//    d->dashboard = new dtkWidgetsMenuBarContainer(this);
//    d->dashboard->navigator->deleteLater();
//    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
//    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////

//    QHBoxLayout *layout = new QHBoxLayout(this);
//    layout->setContentsMargins(0, 0, 0, 0);
//    layout->setSpacing(0);
//    layout->addWidget(d->splitter);
//    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->view, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->view->cellImage()) {
            d->command->setCellImage(d->view->cellImage());
        }
        if(d->view->image()) {
            d->command->setImage(d->view->image());
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageQuantificationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if(d->view->cellImage()) {
            d->command->setCellImage(d->view->cellImage());
        }
        if(d->view->image()) {
            d->command->setImage(d->view->image());
        }
        d->configure(algorithm);
    });

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

//    this->enter();
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d;
}

QString gnomonWorkspaceCellImageQuantification::algoName(void) const
{
    return d->algorithm;
}

QStringList gnomonWorkspaceCellImageQuantification::algorithms(void) const
{
    return d->keys();
}

void gnomonWorkspaceCellImageQuantification::setAlgoName(const QString& algorithm)
{
    if (algorithm != d->algorithm)
    {
        d->configure(algorithm);
        emit parametersChanged();
    }
}

QJSValue gnomonWorkspaceCellImageQuantification::parameters(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewForm* gnomonWorkspaceCellImageQuantification::source(void) const
{
    return d->view;
}

gnomonViewMatplotlib* gnomonWorkspaceCellImageQuantification::target(void) const
{
    return d->mpl_figure;
}

void gnomonWorkspaceCellImageQuantification::run(void)
{
    Q_ASSERT(d->command);

    if(d->view->cellImage()) {
        d->command->setCellImage(d->view->cellImage());
    }
    if(d->view->image()) {
        d->command->setImage(d->view->image());
    }

    d->view->setInputView(true);

    d->command->redo();

    if(d->command->cellImage() != nullptr | d->command->dataFrame() != nullptr) {
        d->registerPipeline();
    }

    if(d->command->cellImage()) {
        d->view->setCellImage(dynamic_cast<gnomonCellImageSeries *>(d->command->cellImage()->clone()));
        d->pipeline->addClonedForm(d->command->cellImage(),d->view->cellImage());
        d->pipeline->addForm(d->command->cellImage());
        d->view->setInputView(false);
    }

    if(d->command->dataFrame()) {
//        d->target_stack->setCurrentWidget(d->mpl_figure);
        d->mpl_figure->setForm("gnomonDataFrame",d->command->dataFrame());
    } else {
//        d->target_stack->setCurrentWidget(d->target_message);
    }
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
