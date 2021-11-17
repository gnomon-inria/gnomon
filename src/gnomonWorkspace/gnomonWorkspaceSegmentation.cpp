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

#include "gnomonWorkspaceSegmentation.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFromImageCommand.h>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceSegmentationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageFromImageCommand>
{
public:
     gnomonWorkspaceSegmentationPrivate(void);
    ~gnomonWorkspaceSegmentationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

// public:
//     QStackedWidget *target_stack = nullptr;
//     gnomonMessageBoard *target_message = nullptr;

//     QSplitter *splitter = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    gnomonCellImage *cellimage = nullptr;

public:
    QMetaObject::Connection c_o;
    QMetaObject::Connection c_s;

// public:
//     dtkWidgetsMenu *menu_;

// public:
//     dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceSegmentationPrivate::gnomonWorkspaceSegmentationPrivate(void) : gnomonWorkspaceTemplatePrivate<gnomonCellImageFromImageCommand>()
{
    this->command   = new gnomonCellImageFromImageCommand;
    this->algorithm = this->command->algorithmName();
}

gnomonWorkspaceSegmentationPrivate::~gnomonWorkspaceSegmentationPrivate(void)
{

}

QString gnomonWorkspaceSegmentationPrivate::workspace(void) const
{
    return "Segmentation";
}

QStringList gnomonWorkspaceSegmentationPrivate::keys(void) const
{
    return gnomonCore::cellImageFromImage::pluginFactory().keys();
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceSegmentation::gnomonWorkspaceSegmentation(QObject *parent) : QObject(parent)
{
    loadPluginGroup("cellImageFromImage");

    d = new gnomonWorkspaceSegmentationPrivate;

    d->source = new gnomonViewForm(this);
//    d->source->setExportColor(this->color);
    d->source->setAcceptForm("gnomonImage",true);
    d->source->setAcceptForm("gnomonPointCloud",true);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
//    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonCellImage",true);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    // TODO: Fix the view pool
    // d->pool = new gnomonViewFormPool(this);
    // d->pool->addView(d->source);
    // d->pool->addView(d->target);
    // d->pool->linkAll();

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

//     d->target_message = new gnomonMessageBoard(this);
// //    d->target_message->setMessage("Load a Form from the top bar to set the input");
//     d->target_message->setMessage("Result will be displayed here");

//     d->target_stack = new QStackedWidget(this);
//     d->target_stack->addWidget(d->target_message);
//     d->target_stack->addWidget(d->target);

// // /////////////////////////////////////////////////////////////////////////////
// // NOTE: Dashboard inception
// // /////////////////////////////////////////////////////////////////////////////

//     d->dashboard = new dtkWidgetsMenuBarContainer(this);
//     d->dashboard->navigator->deleteLater();
//     d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
//     d->dashboard->setFixedWidth(300);

// // /////////////////////////////////////////////////////////////////////////////

//     d->splitter = new QSplitter(this);
//     d->splitter->addWidget(d->source);
//     d->splitter->addWidget(d->target_stack);

//     QHBoxLayout *layout = new QHBoxLayout(this);
//     layout->setContentsMargins(0, 0, 0, 0);
//     layout->setSpacing(0);
//     layout->addWidget(d->splitter);
//     layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if (d->command->input() != d->source->image()) {
            if(d->source->image()) {
                d->command->setInput(d->source->image());
//                d->target_message->setMessage("Press Apply to display the result of the algorithm");
//                d->target_message->setMessage("Result will be displayed here");
            }
        } else {
            qDebug() << "Not changed";
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceSegmentationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if(d->source->image())
            d->command->setInput(d->source->image());
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

//    this->enter();
}

gnomonWorkspaceSegmentation::~gnomonWorkspaceSegmentation(void)
{
    delete d;
}

QString gnomonWorkspaceSegmentation::algoName(void) const
{
    return d->algorithm;
}

void gnomonWorkspaceSegmentation::setAlgoName(const QString& algorithm)
{
    d->configure(algorithm);
}

QJSValue gnomonWorkspaceSegmentation::collection(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewForm* gnomonWorkspaceSegmentation::source(void) const
{
    return d->source;
}

gnomonViewForm* gnomonWorkspaceSegmentation::target(void) const
{
    return d->target;
}

void gnomonWorkspaceSegmentation::run(void)
{
    Q_ASSERT(d->command);

    if ((d->command->input() != d->source->image()) || (d->command->cellPoints() != d->source->pointCloud())) {
        if (d->source->image()) {
            d->command->setInput(d->source->image());
        }
        if (d->source->pointCloud()) {
            d->command->setCellPoints(d->source->pointCloud());
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output())
    {
        d->target->setForm("gnomonCellImage",d->command->output());
        d->target->render();
//        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);

        d->registerPipeline();
    } else {
//        d->target_stack->setCurrentWidget(d->target_message);
        d->source->setEnableLinking(false);
        d->target->setEnableLinking(false);
    }
}

//
// gnomonWorkspaceSegmentation.cpp ends here
