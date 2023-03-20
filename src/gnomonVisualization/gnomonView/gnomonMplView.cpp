#include "gnomonMplView.h"
#include "gnomonAbstractView_p.h"

#include <dtkScript>

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringAdapter>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeAdapter>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeAdapterCommand>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include "gnomonManager/gnomonFormManager.h"

#include <gnomonCommand/gnomonAbstractMplVisualizationCommand>
#include <gnomonCommand/gnomonDataFrame/gnomonDataFrameMplVisualizationCommand>
#include <gnomonCommand/gnomonLString/gnomonLStringMplVisualizationCommand>
#include <gnomonCommand/gnomonTree/gnomonTreeMplVisualizationCommand>

#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"
#include "gnomonVisualizations/gnomonDataFrame/gnomonAbstractDataFrameMplVisualization.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractLStringMplVisualization.h"
#include "gnomonVisualizations/gnomonTree/gnomonAbstractTreeMplVisualization.h"


QVariantMap visuMatplotlibParameters(std::shared_ptr<gnomonAbstractMplVisualization> visu)
{
    QVariantMap parameters;

    dtkCoreParameters dtkParameters = visu->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        QVariant param_value = dtkParameters[param_name]->variant();
        parameters.insert(param_name, param_value);
    }
    return parameters;
};

void setVisuMatplotlibParameters(std::shared_ptr<gnomonAbstractMplVisualization> visu, QVariantMap parameters)
{
    for(const auto& param_name: parameters.keys()) {
        QVariant param = parameters[param_name];
        visu->setParameter(param_name, param);
    }
};


// ///////////////////////////////////////////////////////////////////
// gnomonMplViewPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonMplViewPrivate : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        QString currentFormType;
        int currentFormIndex = -1;
        QMap<QString, QString> visuSelected;  // formType --> visu_name
        QMap<QString, QVariantMap> parameters; // visu_name --> parameters

    } MplViewParameters;

public:
     gnomonMplViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonMplViewPrivate(void);

public:
    gnomonMplView *q = nullptr;

public slots:
    void saveFigure(const QString& path);

    void clear(void);

public:
    QMap<QString, QString> formVisualizationNames;
    QMap<QString, std::shared_ptr<gnomonAbstractMplVisualization> > formVisualization;

    QMap<QString, bool > formVisibility;
    MplViewParameters viewParameters;

public:
    QMap<QString, bool> formModified;

public:
    int figureNumber;

public slots:
    void setFormVisualization(const QString& name, const QString& visu, const QVariantMap &parameters = {});
    void updateFormVisualization(const QString& name);
    
    void render(void);

public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    std::shared_ptr<gnomonAbstractDynamicForm> form_to_adapt = nullptr;

public slots:
    void adaptForm(const QString& adapter_plugin);
};

gnomonMplViewPrivate::gnomonMplViewPrivate(QObject *parent) : QObject(parent)
{
    this->figureNumber = -1;
    viewParameters.currentFormIndex = -1;
}

gnomonMplViewPrivate::~gnomonMplViewPrivate(void)
{
}

/*void gnomonMplViewPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->formVisualization[key]);
        q->emit exportedForm(this->forms[key]);
    }
}*/

void gnomonMplViewPrivate::saveFigure(const QString& path)
{
    /*QSettings settings("inria", "gnomon");
    settings.beginGroup("General");
    QString path = settings.value("last_saved_file", QDir::homePath()).toString();
    settings.endGroup();

    QString export_file_path;
    export_file_path = QFileDialog::getSaveFileName(this, tr("Save figure"), path, tr("Figures (*.png *.eps *.pdf *.svg)"));*/

    QString figure_number = QString::number(this->figureNumber);
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import matplotlib.pyplot as plt", &stat);
    QString figure_statement = "figure = plt.figure("+figure_number+")";
    dtkScriptInterpreterPython::instance()->interpret(figure_statement, &stat);
    dtkScriptInterpreterPython::instance()->interpret("s = figure.get_size_inches()", &stat);
    dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(10,10)", &stat);
    QString save_statement = "figure.savefig('"+path+"')";
    dtkScriptInterpreterPython::instance()->interpret(save_statement, &stat);
    dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(*s)", &stat);

}

void gnomonMplViewPrivate::clear(void)
{
    if (this->figureNumber != -1) {
        int stat;
        QString clearStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.clf()\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(clearStatement, &stat);
    }
}

void gnomonMplViewPrivate::updateFormVisualization(const QString& name)
{
    auto&& visu = this->formVisualization[name];
    auto&& form = q->form(name);
   
    visu->setView(q);

    bool update = false;

    if (name == "gnomonDataFrame") {
        auto formVisualizationDataFrame = std::dynamic_pointer_cast<gnomonAbstractDataFrameMplVisualization>(visu);
        if (formVisualizationDataFrame->dataFrame() != std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)->current()) {
            formVisualizationDataFrame->setDataFrame(std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)->current());
            update = true;
        }
    } else if (name == "gnomonLString") {
        auto formLStringVtkVisualization = std::dynamic_pointer_cast<gnomonAbstractLStringMplVisualization>(visu);
        if (formLStringVtkVisualization->lString() != std::dynamic_pointer_cast<gnomonLStringSeries>(form)->current()) {
            formLStringVtkVisualization->setLString(std::dynamic_pointer_cast<gnomonLStringSeries>(form)->current());
            update = true;
        }
    } else if (name == "gnomonTree") {
        auto formVisualizationTree = std::dynamic_pointer_cast<gnomonAbstractTreeMplVisualization>(visu);
        if (formVisualizationTree->tree() != std::dynamic_pointer_cast<gnomonTreeSeries>(form)->current()) {
            formVisualizationTree->setTree(std::dynamic_pointer_cast<gnomonTreeSeries>(form)->current());
            update = true;
        }
    }

    connect(visu.get(), &gnomonAbstractMplVisualization::parametersChanged, [=] () {
        emit q->formVisuParametersChanged();
    });

    emit q->formVisuParametersChanged();

    if (update) {
        visu->update();
    }
}

void gnomonMplViewPrivate::setFormVisualization(const QString& name, const QString& visu_name, const QVariantMap &parameters)
{
    // saving current parameters before change
    QMap<QString, std::shared_ptr<gnomonAbstractMplVisualization>>::iterator i;
    for (i = formVisualization.begin(); i != formVisualization.end(); ++i) {
        const auto& _visu_name = formVisualizationNames[i.key()];
        viewParameters.parameters[_visu_name] = visuMatplotlibParameters(i.value());
    }

    if (!this->formVisualizationNames.contains(name) || this->formVisualizationNames[name] != visu_name) {

        if (this->formVisualization[name]) {
            this->formVisualization[name]->clear();
            //delete this->formVisualization[name];
            //this->formVisualization[name] = nullptr;
        }

        q->d->visualizationCommands[name]->setForm(q->form(name));
        q->d->visualizationCommands[name]->setAlgorithmName(visu_name);
        auto&& visu = q->d->visualizationCommands[name]->visualization();
        this->formVisualization[name] = std::static_pointer_cast<gnomonAbstractMplVisualization>(visu);
        emit q->formVisualizationChanged();
    }

    auto&& form_visu = this->formVisualization[name];
    // taking saved parameters if none are provided and available
    if(viewParameters.parameters.contains(name) && parameters.size()==0) {
        setVisuMatplotlibParameters(form_visu, viewParameters.parameters[visu_name]);
    } else {
        setVisuMatplotlibParameters(form_visu, parameters);
    }

    this->formVisualizationNames[name] = visu_name;
    if (!this->formVisibility.contains(name)) {
        this->formVisibility[name] = true;
    }
    viewParameters.visuSelected[name] = visu_name;

    this->updateFormVisualization(name);
}

void gnomonMplViewPrivate::render(void)
{
    if (this->figureNumber != -1) {
        int stat;
        QString renderStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(renderStatement, &stat);
    }
}

void gnomonMplViewPrivate::adaptForm(const QString& adapter_plugin)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = this->form_to_adapt;

    if (std::shared_ptr<gnomonLStringSeries> lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form)) {
        gnomonLStringAdapterCommand *lStringCommand = dynamic_cast<gnomonLStringAdapterCommand *>(this->adapterCommands["gnomonLString"][adapter_plugin]);

        lStringCommand->setInput(lString);
        lStringCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedLString = lStringCommand->output();
        if (adaptedLString) {
            //gnomonPipelineManager::instance()->addAdapter(lStringCommand);
            q->setForm("gnomonLString",adaptedLString);
        }
    } else if (std::shared_ptr<gnomonTreeSeries> tree = std::dynamic_pointer_cast<gnomonTreeSeries>(form)) {
        gnomonTreeAdapterCommand *treeCommand = dynamic_cast<gnomonTreeAdapterCommand *>(this->adapterCommands["gnomonTree"][adapter_plugin]);

        treeCommand->setInput(tree);
        treeCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedTree = treeCommand->output();
        if (adaptedTree) {
            //gnomonPipelineManager::instance()->addAdapter(treeCommand);
            q->setForm("gnomonTree",adaptedTree);
        }
    }

    /*if (this->adapter_menu) {
        this->adapter_menu->close();
    }*/
}

// ///////////////////////////////////////////////////////////////////
// gnomonMplView
// ///////////////////////////////////////////////////////////////////

gnomonMplView::gnomonMplView(QObject *parent) : gnomonAbstractView(parent)
{
    dd = new gnomonMplViewPrivate(this);
    dd->q = this;


//
//    static int count = 0;
//    d->figureNumber = count++;

    d->visualizationCommands["gnomonDataFrame"] = new gnomonDataFrameMplVisualizationCommand;
    d->visualizationCommands["gnomonLString"] = new gnomonLStringMplVisualizationCommand;
    d->visualizationCommands["gnomonTree"] = new gnomonTreeMplVisualizationCommand;
    for (const auto &formType: d->visualizationCommands.keys()) {
        d->visualizationCommands[formType]->setView(this);
        d->acceptForms[formType] = false;
    }

    for (const auto& form : d->visualizationCommands.keys()) {
        if (form=="gnomonLString") {
            loadPluginGroup("lStringAdapter");
            for (const auto& key : gnomonCore::lStringAdapter::pluginFactory().keys())
            {
                gnomonAbstractLStringAdapter *adapter = dynamic_cast<gnomonAbstractLStringAdapter *>(gnomonCore::lStringAdapter::pluginFactory().create(key));
                if (!dd->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    dd->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    dd->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    dd->adapterCommands[form] = empty_list;
                }
                dd->adapterTargets[form][key] = adapter->target();
                dd->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                dd->adapterCommands[form][key] = new gnomonLStringAdapterCommand;
                dd->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        } else if (form=="gnomonTree") {
            loadPluginGroup("treeAdapter");
            for (const auto& key : gnomonCore::treeAdapter::pluginFactory().keys())
            {
                gnomonAbstractTreeAdapter *adapter = dynamic_cast<gnomonAbstractTreeAdapter *>(gnomonCore::treeAdapter::pluginFactory().create(key));
                if (!dd->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    dd->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    dd->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    dd->adapterCommands[form] = empty_list;
                }
                dd->adapterTargets[form][key] = adapter->target();
                dd->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                dd->adapterCommands[form][key] = new gnomonTreeAdapterCommand;
                dd->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        }
    }

    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import gnomon.utils.matplotlib_tools", &stat);

    connect(this, &gnomonMplView::formAdded, [=] (const QString& key) {
        this->render();
        emit formsChanged();
    });

    // just need to find a signal that's actually emitted when a parameter changes :|
    connect(this, &gnomonMplView::formVisuParametersChanged, [=] () {
        QMap<QString, std::shared_ptr<gnomonAbstractMplVisualization>>::iterator i;
        for (i = dd->formVisualization.begin(); i != dd->formVisualization.end(); ++i) {
            const auto& visu_name = dd->formVisualizationNames[i.key()];
            dd->viewParameters.parameters[visu_name] = visuMatplotlibParameters(i.value());
        }
    });
}

gnomonMplView::~gnomonMplView(void)
{
    delete dd;
}

// TODO: introduce a command pattern
void gnomonMplView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    QString form_name = form->formName();
    if (d->acceptForms[form_name]) {
        QString visu_name;
        QVariantMap parameters;

        if (dd->formVisualization.contains(form_name) &&  dd->formVisualization[form_name]) {
            std::shared_ptr<gnomonAbstractMplVisualization> current_visu =  dd->formVisualization[form_name];
            visu_name = current_visu->pluginName();
            parameters = current_visu->visuParameters();
        } else {
            visu_name = d->visualizationCommands[form_name]->algorithmName();
        }

        d->forms[form_name] = form;
        dd->setFormVisualization(form_name, visu_name, parameters);
        emit formAdded(form_name);
    } else {
        // TODO: restore the adaption mechanism
        // this->setAdaptedForm(form_name, form);
        emit badFormDropped(form->formName(), acceptedForms().join(", "));
    }
    return;

}


void gnomonMplView::setAdaptedForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractMplVisualization> visualization)
{
    if (dd->adapterCommands.contains(name)) {
        QVariantMap adapter_descs;
        for (const auto &key : dd->adapterCommands[name].keys()) {
            if (d->acceptForms[dd->adapterTargets[name][key]]) {
                adapter_descs[key] = dd->adapterDescriptions[name][key];
            }
        }
        if (adapter_descs.size() > 0) {
            // TODO : emit adapting signal?
            /*d->form_to_adapt = form;
            dd->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
            dd->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
            dd->adapter_menu->resize(dtkApp->window()->width() * 2/5, dtkApp->window()->height() - 40);
            dd->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
            dd->adapter_menu->show();

            QObject *context = dd->adapter_menu->rootObject();
            connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(d->adapter_menu);*/
        }
    }
}


void gnomonMplView::setIsModifiedForm(const QString& name)
{
    dd->formModified[name] = true;
    this->updateVisualizations();
}

void gnomonMplView::updateVisualizations(void)
{
    for (const auto& name : dd->formVisualization.keys()) {
        if (dd->formModified[name]) {
            dd->formVisualization[name]->update();
            dd->formModified[name] = false;
        }
    }
}

void gnomonMplView::setFormVisuName(const QString& name, const QString& visu_name)
{
    if (d->forms.contains(name)) {
        dd->setFormVisualization(name, visu_name);
    }
}

QJSValue gnomonMplView::formVisuParameters(const QString& name)
{
    if (d->forms.contains(name)) {
        return dtkCoreParameterCollection(dd->formVisualization[name]->parameters()).toJSValue(this->parent());
    } else {
        return QJSValue();
    }
}

QVariant gnomonMplView::formVisuParameter(const QString& name, const QString& parameter_name)
{
    if (d->forms.contains(name)) {
        auto params = dd->formVisualization[name]->parameters();
        if (params.keys().contains(parameter_name)) {
            dtkCoreParameter *param = params.value(parameter_name);
            // TODO: More specific cases to handle?
            if (auto string_param = dynamic_cast<dtkCoreParameterSimple<QString> *>(param))
            {
                return QVariant(string_param->value());
            } else {
                return param->variant();
            }
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

void gnomonMplView::setFormVisuParameter(const QString& name, const QString& parameter_name, const QVariant& value)
{
    if (d->forms.contains(name)) {
        auto visu = dd->formVisualization[name];
        visu->setParameter(parameter_name, value);
    }
}

void gnomonMplView::setFormVisible(const QString& name, bool visible)
{
    if (dd->formVisualization.contains(name)) {
        if (dd->formVisualization[name]) {
            // TODO: Implement visibility for mpl visus?
            // d->formVisualization[name]->setVisible(visible);
            dd->formVisibility[name] = visible;
        }
    }

    this->render();
}

// TODO: introduce a command pattern
void gnomonMplView::removeForm(const QString& name)
{
    if (dd->formVisualization.contains(name)) {
        if (dd->formVisualization[name]) {
            dd->formVisualization[name]->disconnect();
            dd->formVisualization[name]->clear();
        }
    }
    dd->formVisualization.remove(name);

    if (dd->formVisualizationNames.contains(name)) {
        dd->viewParameters.parameters.remove(dd->formVisualizationNames[name]);
    }
    dd->viewParameters.visuSelected.remove(name);

    dd->formVisualizationNames.remove(name);
    dd->formVisibility.remove(name);
    dd->formModified.remove(name);

    gnomonAbstractView::removeForm(name);
}

void gnomonMplView::render(void)
{
    dd->render();
}

void gnomonMplView::update(void)
{
     for (const auto& key : dd->formVisualization.keys()) {
         dd->formVisualization[key]->update();
     }
}

void gnomonMplView::clear(void)
{
    for (auto name : dd->formVisualization.keys()) {
        if (dd->formVisualization[name]) {
            dd->formVisualization[name]->disconnect();
            dd->formVisualization[name]->clear();
        }
    }
    dd->formVisualization.clear();

    for (auto name : dd->formVisualizationNames) {
        dd->viewParameters.parameters.remove(dd->formVisualizationNames[name]);
    }
    dd->viewParameters.visuSelected.clear();

    dd->formVisualizationNames.clear();
    dd->formVisibility.clear();
    dd->formModified.clear();

    dd->clear();

    gnomonAbstractView::clear();
}

int gnomonMplView::figureNumber(void)
{
    return dd->figureNumber;
}

void gnomonMplView::setFigureNumber(int num)
{
    dd->figureNumber = num;
    emit figureNumberChanged(num);
}

void gnomonMplView::notifyFormSelected(int index, QString formType) {
    dd->viewParameters.currentFormIndex = index;
    dd->viewParameters.currentFormType = std::move(formType);
}

int gnomonMplView::lastFormIndexSelected() {
    return dd->viewParameters.currentFormIndex;
}

QString gnomonMplView::lastFromTypeSelected() {
    return dd->viewParameters.currentFormType;
}

QString gnomonMplView::lastVisuSelected(QString formType) {
    return dd->viewParameters.visuSelected[formType];
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonMplView.moc"

//
// gnomonMplView.cpp ends here
