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
        QMap<QString, QString> visuSelected;  // form_type --> visu_name
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
    QMap<QString, std::shared_ptr<gnomonAbstractMplVisualization> > formVisualization;

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

void gnomonMplViewPrivate::setFormVisualization(const QString& form_type, const QString& visu_name, const QVariantMap &parameters)
{
    // saving current parameters before change
    for (const auto& form_type : q->d->forms.keys()) {
        const auto& _visu_name = q->d->visualizationCommands[form_type]->algorithmName();
        viewParameters.parameters[_visu_name] = q->d->visualizationCommands[form_type]->visualizationParameters();
    }

    auto visu_parameters = parameters;
    if(viewParameters.parameters.contains(visu_name) && parameters.size()==0) {
        visu_parameters = viewParameters.parameters[visu_name];
    }

    q->d->visualizationCommands[form_type]->setForm(q->form(form_type));
    q->d->visualizationCommands[form_type]->setFormVisualization(visu_name, visu_parameters);
    auto&& visu = q->d->visualizationCommands[form_type]->visualization();
    this->formVisualization[form_type] = std::static_pointer_cast<gnomonAbstractMplVisualization>(visu);
    emit q->formVisualizationChanged();

    viewParameters.visuSelected[form_type] = visu_name;

    this->updateFormVisualization(form_type);

    connect(visu.get(), &gnomonAbstractMplVisualization::parametersChanged, [=] () {
        emit q->formVisuParametersChanged();
    });

    q->formVisuParametersChanged();
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
    for (const auto &form_type: d->visualizationCommands.keys()) {
        d->visualizationCommands[form_type]->setView(this);
        d->acceptForms[form_type] = false;
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
        for (const auto& form_type : d->forms.keys()) {
            const auto& visu_name = d->visualizationCommands[form_type]->algorithmName();
            dd->viewParameters.parameters[visu_name] = d->visualizationCommands[form_type]->visualizationParameters();
        }
    });
}

gnomonMplView::~gnomonMplView(void)
{
    delete dd;
}

// TODO: introduce a command pattern
void gnomonMplView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractVisualization> visualization)
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

    if (d->forms.contains(name)) {
        QString visu_name = d->visualizationCommands[name]->algorithmName();
        dd->viewParameters.parameters.remove(visu_name);
    }
    dd->viewParameters.visuSelected.remove(name);
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

    for (const auto & form_type : d->forms.keys()) {
        QString visu_name = d->visualizationCommands[form_type]->algorithmName();
        dd->viewParameters.parameters.remove(visu_name);
    }
    dd->viewParameters.visuSelected.clear();

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

void gnomonMplView::notifyFormSelected(int index, QString form_type) {
    dd->viewParameters.currentFormIndex = index;
    dd->viewParameters.currentFormType = std::move(form_type);
}

int gnomonMplView::lastFormIndexSelected() {
    return dd->viewParameters.currentFormIndex;
}

QString gnomonMplView::lastFromTypeSelected() {
    return dd->viewParameters.currentFormType;
}

QString gnomonMplView::lastVisuSelected(QString form_type) {
    return dd->viewParameters.visuSelected[form_type];
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonMplView.moc"

//
// gnomonMplView.cpp ends here
