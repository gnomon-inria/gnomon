#include "gnomonWorkspaceLSystemModel.h"

#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand>
#include <gnomonCore/gnomonPythonPluginLoader>

#include <gnomonVisualization/gnomonView/gnomonViewForm>

QString vonKochLSystem(void)
{
    QString l_sys;
    l_sys += "Axiom: -(90)_(0.1)F(50.)\n";
    l_sys += "production:\n";
    l_sys += "F(x) : produce F(x/3.)+(60)F(x/3.)-(120)F(x/3.)+(60)F(x/3.)\n";
    l_sys += "endlsystem\n";
    l_sys += "###### INITIALISATION ######\n";
    l_sys += "\n";
    l_sys += "def __initialiseContext__(context):\n";
    l_sys += "    from openalea.plantgl.all import Material,Color3\n";
    l_sys += "    context.options.setSelection(\"Selection Required\",0)\n";
    l_sys += "    context.options.setSelection(\"Module declaration\",0)\n";
    return l_sys;
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceLSystemModelPrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceLSystemModelPrivate
{
public:
    gnomonWorkspaceLSystemModelPrivate(void);
    ~gnomonWorkspaceLSystemModelPrivate(void);

public:
    bool setModel(const QString& model);

public:
    QString text;
    int derivationLength = 1;

public:
    QString workspace;
    QStringList keys;
    QString model;
    int currentIndex = 0;

    QTemporaryFile *model_file = nullptr;

public:
    gnomonLStringEvolutionModelCommand *command = nullptr;

public:
    gnomonViewForm *view = nullptr;
};

gnomonWorkspaceLSystemModelPrivate::gnomonWorkspaceLSystemModelPrivate(void)
{

}

gnomonWorkspaceLSystemModelPrivate::~gnomonWorkspaceLSystemModelPrivate(void)
{

}


// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceLSystemModel
// /////////////////////////////////////////////////////////////////////////////


gnomonWorkspaceLSystemModel::gnomonWorkspaceLSystemModel(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemModelPrivate;

    loadPluginGroup("lStringEvolutionModel");
    emit modelsLoaded();

    d->workspace = "LStringEvolutionModel";
    d->command = new gnomonLStringEvolutionModelCommand;
    d->keys = gnomonCore::lStringEvolutionModel::pluginFactory().keys();
    d->model = d->command->modelName();

    d->view = new gnomonViewForm(parent);
    d->view->setAcceptForm("gnomonLString", true);

    connect(d->view, &gnomonViewForm::formsChanged, [=] ()
    {
        this->setInitialState();
    });

    d->model_file = new QTemporaryFile();
    if (d->model_file->open()) {
        d->command->setLSystem(d->model_file->fileName());
    }

    this->setText(vonKochLSystem());
    emit parametersChanged();
}

gnomonWorkspaceLSystemModel::~gnomonWorkspaceLSystemModel(void)
{
    gnomonLStringEvolutionModelCommand *command = (gnomonLStringEvolutionModelCommand *) d->command;
    if (command) {
        delete command;
    }
    delete d;
}

QString gnomonWorkspaceLSystemModel::text(void)
{
    return d->text;
}

void gnomonWorkspaceLSystemModel::setText(const QString& text)
{
    if (text != d->text) {
        d->text = text;
        QFile *model_file = new QFile(d->model_file->fileName());
        if (model_file->open(QIODevice::WriteOnly))
        {
            QTextStream model_stream(model_file);
            model_stream<<d->text;
            model_stream.flush();
        }
        model_file->close();
        d->command->setLSystem(d->model_file->fileName());
        emit textChanged(d->text);
    }
}

int gnomonWorkspaceLSystemModel::derivationLength(void)
{
    return d->derivationLength;
}

void gnomonWorkspaceLSystemModel::setDerivationLength(int l)
{
    if (l != d->derivationLength) {
        d->derivationLength = l;
        emit derivationLengthChanged(d->derivationLength);
    }
}

void gnomonWorkspaceLSystemModel::run()
{
    Q_ASSERT(d->command);

    // TODO: redo=step or redo=run?
    emit started();
    this->setInitialState();
    d->command->undo();
    for (int t=0; t<d->derivationLength; t++) {
        d->command->redo();
    }
    this->viewState();
    emit finished();
}

void gnomonWorkspaceLSystemModel::step()
{
    Q_ASSERT(d->command);

    // TODO: make the commannd async
    emit started();
    d->command->redo();
    this->viewState();
    emit finished();
}

void gnomonWorkspaceLSystemModel::reset()
{
    Q_ASSERT(d->command);

    emit started();
    this->setInitialState();
    d->command->undo();
    this->viewState();
    emit finished();
}

void gnomonWorkspaceLSystemModel::setInitialState()
{
    // TODO: drop axiom into a different view?
    // d->command->setInitialState(d->view->lString());
    d->command->setInitialState(nullptr);
}

void gnomonWorkspaceLSystemModel::viewState()
{
    // TODO: pass lsystem to visu plugin
    auto lString = d->command->state();
    if (lString) {
        d->view->setLString(lString);
        d->view->render();
    }
}

void gnomonWorkspaceLSystemModel::export_outputs(void)
{
    d->view->transmit();
}

QString gnomonWorkspaceLSystemModel::modelName(void) const
{
    return d->model;
}

QStringList gnomonWorkspaceLSystemModel::models(void) const
{
    return d->keys;
}

void gnomonWorkspaceLSystemModel::setModelName(const QString &model)
{
    if (model != d->model) {
        d->command->setModelName(model);
        d->model = model;
        emit modelChanged(model);
        d->command->undo();
        this->setInitialState();
        emit parametersChanged();
    }
}

int gnomonWorkspaceLSystemModel::currentIndex(void) const
{
    return d->currentIndex;
}

void gnomonWorkspaceLSystemModel::setCurrentIndex(int i)
{
    if ((d->currentIndex != i) & (i < d->keys.size())) {
        d->currentIndex = i;
        emit currentIndexChanged();
        this->setModelName(d->keys[i]);
    }
}

gnomonViewForm *gnomonWorkspaceLSystemModel::view(void) const
{
    return d->view;
}

QJSValue gnomonWorkspaceLSystemModel::parameters(void)
{
    QJSValue parameters = dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
    QMap<QString, QString> parameter_groups = d->command->parameterGroups();

    QJSValueIterator it(parameters);
    while (it.hasNext()) {
        it.next();
        QString group = parameter_groups.contains(it.name()) ? parameter_groups[it.name()] : "";
        it.value().setProperty("group", group != "" ? group : nullptr);
    }
    return parameters;
}
