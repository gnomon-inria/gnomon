#include "gnomonViewMatplotlib.h"
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

#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"
#include "gnomonVisualizations/gnomonDataFrame/gnomonAbstractMatplotlibVisualizationDataFrame.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h"
#include "gnomonVisualizations/gnomonTree/gnomonAbstractMatplotlibVisualizationTree.h"


QVariantMap visuMatplotlibParameters(std::shared_ptr<gnomonAbstractMatplotlibVisualization> visu)
{
    QVariantMap parameters;

    dtkCoreParameters dtkParameters = visu->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        QVariant param_value = dtkParameters[param_name]->variant();
        parameters.insert(param_name, param_value);
    }
    return parameters;
};

void setVisuMatplotlibParameters(std::shared_ptr<gnomonAbstractMatplotlibVisualization> visu, QVariantMap parameters)
{
    for(const auto& param_name: parameters.keys()) {
        QVariant param = parameters[param_name];
        visu->setParameter(param_name, param);
    }
};


// ///////////////////////////////////////////////////////////////////
// gnomonViewMatplotlibPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewMatplotlibPrivate : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        QString currentFormType;
        int currentFormIndex = -1;
        QMap<QString, QString> visuSelected;  // formType --> visu_name
        QMap<QString, QVariantMap> parameters; // visu_name --> parameters

    } viewMatplotlibParameters;

public:
     gnomonViewMatplotlibPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonViewMatplotlibPrivate(void);

public:
    gnomonViewMatplotlib *q = nullptr;

public slots:
    void saveFigure(const QString& path);

    void clear(void);

public:
    QMap<QString, QString> formVisualizationNames;
    QMap<QString, std::shared_ptr<gnomonAbstractMatplotlibVisualization> > formVisualization;

    QMap<QString, bool > formVisibility;
    viewMatplotlibParameters viewParameters;

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

gnomonViewMatplotlibPrivate::gnomonViewMatplotlibPrivate(QObject *parent) : QObject(parent)
{
    this->figureNumber = -1;
    viewParameters.currentFormIndex = -1;
}

gnomonViewMatplotlibPrivate::~gnomonViewMatplotlibPrivate(void)
{
}

/*void gnomonViewMatplotlibPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->formVisualization[key]);
        q->emit exportedForm(this->forms[key]);
    }
}*/

void gnomonViewMatplotlibPrivate::saveFigure(const QString& path)
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

void gnomonViewMatplotlibPrivate::clear(void)
{
    if (this->figureNumber != -1) {
        int stat;
        QString clearStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.clf()\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(clearStatement, &stat);
    }
}

void gnomonViewMatplotlibPrivate::updateFormVisualization(const QString& name)
{
    auto&& visu = this->formVisualization[name];
    auto&& form = q->form(name);
   
    visu->setView(q);

    bool update = false;

    if (name == "gnomonDataFrame") {
        auto formVisualizationDataFrame = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationDataFrame>(visu);
        if (formVisualizationDataFrame->dataFrame() != std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)->current()) {
            formVisualizationDataFrame->setDataFrame(std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)->current());
            update = true;
        }
    } else if (name == "gnomonLString") {
        auto formVisualizationLString = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationLString>(visu);
        if (formVisualizationLString->lString() != std::dynamic_pointer_cast<gnomonLStringSeries>(form)->current()) {
            formVisualizationLString->setLString(std::dynamic_pointer_cast<gnomonLStringSeries>(form)->current());
            update = true;
        }
    } else if (name == "gnomonTree") {
        auto formVisualizationTree = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationTree>(visu);
        if (formVisualizationTree->tree() != std::dynamic_pointer_cast<gnomonTreeSeries>(form)->current()) {
            formVisualizationTree->setTree(std::dynamic_pointer_cast<gnomonTreeSeries>(form)->current());
            update = true;
        }
    }

    connect(visu.get(), &gnomonAbstractMatplotlibVisualization::parametersChanged, [=] () {
        emit q->formVisuParametersChanged();
    });

    emit q->formVisuParametersChanged();

    if (update) {
        visu->update();
    }
}

void gnomonViewMatplotlibPrivate::setFormVisualization(const QString& name, const QString& visu_name, const QVariantMap &parameters)
{
    // saving current parameters before change
    QMap<QString, std::shared_ptr<gnomonAbstractMatplotlibVisualization>>::iterator i;
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

        if (name == "gnomonDataFrame") {
            q->d->visualizationCommands[name]->setAlgorithmName(visu_name);
            auto visu = q->d->visualizationCommands[name]->visualization();
            this->formVisualization[name] = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualization>(visu);
            //this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame>(gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(visu));
            emit q->formVisualizationChanged();
        } else if (name == "gnomonLString") {
            this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString>(gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(visu_name));
            emit q->formVisualizationChanged();
        } else if (name == "gnomonTree") {
            this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree>(gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(visu_name));
            emit q->formVisualizationChanged();
        }
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

void gnomonViewMatplotlibPrivate::render(void)
{
    if (this->figureNumber != -1) {
        int stat;
        QString renderStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(renderStatement, &stat);
    }
}

void gnomonViewMatplotlibPrivate::adaptForm(const QString& adapter_plugin)
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
// gnomonViewMatplotlib
// ///////////////////////////////////////////////////////////////////

gnomonViewMatplotlib::gnomonViewMatplotlib(QObject *parent) : gnomonAbstractView(parent)
{
    dd = new gnomonViewMatplotlibPrivate(this);
    dd->q = this;


//
//    static int count = 0;
//    d->figureNumber = count++;

    loadPluginGroup("matplotlibVisualizationTree");
    //loadPluginGroup("matplotlibVisualizationDataFrame");
    d->visualizationCommands["gnomonDataFrame"] = new gnomonDataFrameMplVisualizationCommand;
    loadPluginGroup("matplotlibVisualizationLString");

    d->acceptForms["gnomonTree"] = false;
    d->acceptForms["gnomonDataFrame"] = false;
    d->acceptForms["gnomonLString"] = false;


    for (const auto& form : d->acceptForms.keys()) {
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

    /*QFile file(":gnomon/matplotlib_figure.py");
    if (file.open(QIODevice::ReadOnly)) {
        int stat;
        QString numberStatement = "num = " + QString::number(d->figureNumber);
        dtkScriptInterpreterPython::instance()->interpret(numberStatement, &stat);
        QString matplotlib_script  = file.readAll();
        file.close();
        dtkScriptInterpreterPython::instance()->interpret(matplotlib_script, &stat);
    } else {
        qWarning() << "Can't open matplotlib figure script";
    }*/

    connect(this, &gnomonViewMatplotlib::formAdded, [=] (const QString& key) {
//        d->addFormMenu(key);
//        d->configure(d->formVisualizationPaneItems[key], key);
        this->render();
        emit formsChanged();
    });


    // just need to find a signal that's actually emitted when a parameter changes :|
    connect(this, &gnomonViewMatplotlib::formVisuParametersChanged, [=] () {
        QMap<QString, std::shared_ptr<gnomonAbstractMatplotlibVisualization>>::iterator i;
        for (i = dd->formVisualization.begin(); i != dd->formVisualization.end(); ++i) {
            const auto& visu_name = dd->formVisualizationNames[i.key()];
            dd->viewParameters.parameters[visu_name] = visuMatplotlibParameters(i.value());
        }
    });

    // connect(d->renderButton, &QPushButton::clicked, [=] () {
    //     for (const auto& key : d->formVisualization.keys()) {
    //         gnomonAbstractMatplotlibVisualization *v = d->formVisualization[key];
    //         if(v) {
    //             v->update();
    //         }
    //     }
    // });
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete dd;
}

// TODO: introduce a command pattern
void gnomonViewMatplotlib::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (std::shared_ptr<gnomonTreeSeries> tree = std::dynamic_pointer_cast<gnomonTreeSeries>(form)) {
        if (d->acceptForms["gnomonTree"]) {
            d->forms["gnomonTree"] = tree;
            if ((!dd->formVisualization.contains("gnomonTree"))||(!dd->formVisualization["gnomonTree"])) {
                QString key = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys()[0];

                dd->formVisualization["gnomonTree"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree>(gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(key));
                dd->formVisualization["gnomonTree"]->setView(this);
                dd->viewParameters.visuSelected[name] = key;
                emit formVisualizationChanged();
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree> formVisualizationTree = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationTree>(dd->formVisualization["gnomonTree"]);
            formVisualizationTree->setTree(std::dynamic_pointer_cast<gnomonTree>(tree->current()));
            this->setIsModifiedForm("gnomonTree");
            emit formAdded("gnomonTree");
        } else {
            this->setAdaptedForm("gnomonTree", tree);
        }
    } else if (std::shared_ptr<gnomonDataFrameSeries> dataFrame = std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)) {
        if (d->acceptForms["gnomonDataFrame"]) {
            d->forms["gnomonDataFrame"] = dataFrame;

            if ((!dd->formVisualization.contains("gnomonDataFrame"))||(!dd->formVisualization["gnomonDataFrame"]))
            {
                QString key = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys()[0];
                dd->formVisualization["gnomonDataFrame"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame>(gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(key));
                dd->formVisualization["gnomonDataFrame"]->setView(this);
                dd->viewParameters.visuSelected[name] = key;
                emit formVisualizationChanged();
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame> formVisualizationDataFrame = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationDataFrame>(dd->formVisualization["gnomonDataFrame"]);
            formVisualizationDataFrame->setDataFrame(std::dynamic_pointer_cast<gnomonDataFrame>(dataFrame->current()));
            this->setIsModifiedForm("gnomonDataFrame");
            emit formAdded("gnomonDataFrame");
        }
    } else if (std::shared_ptr<gnomonLStringSeries> lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form)) {
        if (d->acceptForms["gnomonLString"]) {
            d->forms["gnomonLString"] = lString;

            if ((!dd->formVisualization.contains("gnomonLString"))||(!dd->formVisualization["gnomonLString"]))
            {
                QString key = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys()[0];
                dd->formVisualization["gnomonLString"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString>(gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(key));
                dd->formVisualization["gnomonLString"]->setView(this);
                dd->viewParameters.visuSelected[name] = key;
                emit formVisualizationChanged();
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString> formVisualizationLString = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationLString>(dd->formVisualization["gnomonLString"]);
            formVisualizationLString->setLString(std::dynamic_pointer_cast<gnomonLString>(lString->current()));
            this->setIsModifiedForm("gnomonLString");
            emit formAdded("gnomonLString");
        } else {
            this->setAdaptedForm("gnomonLString", lString);
        }
    }
}


void gnomonViewMatplotlib::setAdaptedForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, gnomonAbstractMatplotlibVisualization *visualization)
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


void gnomonViewMatplotlib::setIsModifiedForm(const QString& name)
{
    dd->formModified[name] = true;
    this->updateVisualizations();
}

void gnomonViewMatplotlib::updateVisualizations(void)
{
    for (const auto& name : dd->formVisualization.keys()) {
        if (dd->formModified[name]) {
            dd->formVisualization[name]->update();
            dd->formModified[name] = false;
        }
    }
}


QString gnomonViewMatplotlib::formVisuName(const QString& name)
{
    QString visu_name;
    if (d->forms.contains(name)) {
        visu_name = dd->formVisualizationNames[name];
    }
    return visu_name;
}

QVariantList gnomonViewMatplotlib::formVisualizations(const QString& name)
{
    if (d->forms.contains(name)) {
        if (name == "gnomonDataFrame") {
            return gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().dataList();
        } else if (name == "gnomonLString") {
            return gnomonVisualization::matplotlibVisualizationLString::pluginFactory().dataList();
        } else if (name == "gnomonTree") {
            return gnomonVisualization::matplotlibVisualizationTree::pluginFactory().dataList();
        }
    }
    return {};
}

void gnomonViewMatplotlib::setFormVisuName(const QString& name, const QString& visu_name)
{
    if (d->forms.contains(name)) {
        dd->setFormVisualization(name, visu_name);
    }
}

QJSValue gnomonViewMatplotlib::formVisuParameters(const QString& name)
{
    if (d->forms.contains(name)) {
        return dtkCoreParameterCollection(dd->formVisualization[name]->parameters()).toJSValue(this->parent());
    } else {
        return QJSValue();
    }
}

QVariant gnomonViewMatplotlib::formVisuParameter(const QString& name, const QString& parameter_name)
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

void gnomonViewMatplotlib::setFormVisuParameter(const QString& name, const QString& parameter_name, const QVariant& value)
{
    if (d->forms.contains(name)) {
        auto visu = dd->formVisualization[name];
        visu->setParameter(parameter_name, value);
    }
}

void gnomonViewMatplotlib::setFormVisible(const QString& name, bool visible)
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
void gnomonViewMatplotlib::removeForm(const QString& name)
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

void gnomonViewMatplotlib::render(void)
{
    dd->render();
}

void gnomonViewMatplotlib::update(void)
{
     for (const auto& key : dd->formVisualization.keys()) {
         dd->formVisualization[key]->update();
     }
}

void gnomonViewMatplotlib::clear(void)
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

int gnomonViewMatplotlib::figureNumber(void)
{
    return dd->figureNumber;
}

void gnomonViewMatplotlib::setFigureNumber(int num)
{
    dd->figureNumber = num;
    emit figureNumberChanged(num);
}

void gnomonViewMatplotlib::notifyFormSelected(int index, QString formType) {
    dd->viewParameters.currentFormIndex = index;
    dd->viewParameters.currentFormType = std::move(formType);
}

int gnomonViewMatplotlib::lastFormIndexSelected() {
    return dd->viewParameters.currentFormIndex;
}

QString gnomonViewMatplotlib::lastFromTypeSelected() {
    return dd->viewParameters.currentFormType;
}

QString gnomonViewMatplotlib::lastVisuSelected(QString formType) {
    return dd->viewParameters.visuSelected[formType];
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
