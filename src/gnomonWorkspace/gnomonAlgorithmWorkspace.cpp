#include "gnomonAlgorithmWorkspace.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspacePrivate
// /////////////////////////////////////////////////////////////////////////////


gnomonAlgorithmWorkspacePrivate::gnomonAlgorithmWorkspacePrivate(void)
{
    this->pipeline = gnomonPipeline::instance();
}

gnomonAlgorithmWorkspacePrivate::~gnomonAlgorithmWorkspacePrivate(void)
{
}

bool gnomonAlgorithmWorkspacePrivate::setAlgorithm(const QString& algorithm)
{
    if (algorithm != this->algorithm) {
        this->command->setAlgorithmName(algorithm);
        this->algorithm = algorithm;
        return true;
    }
    return false;
}

void gnomonAlgorithmWorkspacePrivate::registerPipeline(void)
{
    if (this->command) {
        this->pipeline->addAlgorithm(this->command);
    }
}

void gnomonAlgorithmWorkspacePrivate::updateViewFormTypes(void)
{
    if(!this->command) {
        dtkError() << Q_FUNC_INFO << "command should be created before calling this function";
    }

    for(auto [input_name, input_type] : this->command->inputTypes()) {
        for(auto* source: this->sources->views()) {
            source->setAcceptForm(input_type, true);
        }
    }

    auto&& output_types = this->command->outputTypes();
    for (auto [output_name, output_type] : this->command->outputTypes()) {
        for(auto * target: this->targets->views()) {
            target->setAcceptForm(output_type, true);
        }
    }
}

void gnomonAlgorithmWorkspacePrivate::updatePool(void)
{
    if(!this->pool)
        this->pool = new gnomonViewFormPool(this);

    foreach(gnomonViewForm *view, this->sources->views())
        this->pool->addView(view);

    foreach(gnomonViewForm *view, this->targets->views())
        this->pool->addView(view);
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspace
// /////////////////////////////////////////////////////////////////////////////

gnomonAlgorithmWorkspace::gnomonAlgorithmWorkspace(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonAlgorithmWorkspacePrivate;

    d->sources = new gnomonViewFormList(this);
    connect(d->sources, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        v->setInputView(true);
    });

    d->targets = new gnomonViewFormList(this);
    connect(d->targets, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        connect(v, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));
    });

    connect(d->sources, &gnomonViewFormList::formsChanged, [=] ()
    {
        this->setInputs();
        emit parametersChanged();
    });

}

gnomonAlgorithmWorkspace::~gnomonAlgorithmWorkspace(void)
{
    delete d;
}

QString gnomonAlgorithmWorkspace::algoName(void) const
{
    return d->algorithm;
}

QStringList gnomonAlgorithmWorkspace::algorithms(void) const
{
    return d->keys;
}

void gnomonAlgorithmWorkspace::setAlgoName(const QString& algorithm)
{
    if (d->setAlgorithm(algorithm)) {
        emit algorithmChanged(algorithm);
        this->setInputs();
        emit parametersChanged();
    }
}

QJSValue gnomonAlgorithmWorkspace::parameters(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewFormList* gnomonAlgorithmWorkspace::sources(void) const
{
    return d->sources;
}

gnomonViewFormList* gnomonAlgorithmWorkspace::targets(void) const
{
    return d->targets;
}

void gnomonAlgorithmWorkspace::run(void)
{
    Q_ASSERT(d->command);

    emit started();

    this->setInputs();

    d->command->redo();

    this->viewOutputs();
}

void gnomonAlgorithmWorkspace::setInputs()
{
    //you need to overwrite this function if you don't have an exact mapping between
    // the number of views (sources) and the number of input types for your command.
    // example: workspaceSegmentation

    if (d->command->inputs().size() == d->sources->views().size()) {
        d->command->undo(); //clean
        int i=0;
        for(auto [name, input_type] : d->command->inputTypes()) {
            d->command->setInputForm(name, (*d->sources)[i]->form(input_type));
            ++i;
        }
    } else if (d->sources->views().size() == 1) {
        d->command->undo(); //clean
        for(auto [name, input_type] : d->command->inputTypes()) {
            d->command->setInputForm(name, (*d->sources)[0]->form(input_type));
        }
    } else {
        dtkWarn() << Q_FUNC_INFO << "inputs size " <<d->command->inputs().size() << " but nb input views " << d->sources->views().size();
        return;
    }
}

void gnomonAlgorithmWorkspace::viewOutputs(void)
{
    Q_ASSERT(d->command);

    if(d->command->outputs().size() != d->targets->views().size()) {
        dtkWarn() << Q_FUNC_INFO << "outputs size " <<d->command->outputs().size() << " but nb output views " << d->targets->views().size();
        return;
    }

    bool empty_output = true;

    int i=0;
    for(auto [name, output_type] : d->command->outputTypes()) {
        if(d->command->outputs()[name]) {
            (*d->targets)[i]->setForm(output_type, d->command->outputs()[name]);
            (*d->targets)[i]->render();
            empty_output = false;
        }
        ++i;
    }

    if (!empty_output) {
        d->registerPipeline();
    }
}

//
// gnomonAlgorithmWorkspace.cpp ends here
