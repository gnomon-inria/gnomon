#include "gnomonViewMatplotlib.h"

#include <dtkScript>

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringAdapter>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeAdapter>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeAdapterCommand>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include "gnomonManager/gnomonFormManager.h"

#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"
#include "gnomonVisualizations/gnomonDataFrame/gnomonAbstractMatplotlibVisualizationDataFrame.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h"
#include "gnomonVisualizations/gnomonTree/gnomonAbstractMatplotlibVisualizationTree.h"


// ///////////////////////////////////////////////////////////////////
// gnomonViewMatplotlibPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewMatplotlibPrivate : public QObject
{
    Q_OBJECT

public:
     gnomonViewMatplotlibPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonViewMatplotlibPrivate(void);

public:
    gnomonViewMatplotlib *q = nullptr;

public slots:
    void exportToManager(void);
    void saveFigure(const QString& path);

    void removeForm(const QString&);
    void clear(void);

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;
    QMap<QString, QString> formVisualizationNames;
    QMap<QString, std::shared_ptr<gnomonAbstractMatplotlibVisualization> > formVisualization;

public:
    QMap<QString, bool> acceptForms;

public:
    QMap<QString, bool> formModified;

public:
    int figureNumber;
    bool input_view = false;

public:
    QColor export_color = QColor("#cccccc");

public slots:
    void setFormVisualization(const QString& name, const QString& visu);
    void updateFormVisualization(const QString& name);
    
//    void configure(dtkWidgetsMenuItemDIY *parent, const QString& key);
//    void addFormMenu(const QString& key);
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
}

gnomonViewMatplotlibPrivate::~gnomonViewMatplotlibPrivate(void)
{
}

void gnomonViewMatplotlibPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->formVisualization[key]);
        q->emit exportedForm(this->forms[key]);
    }
}

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

void gnomonViewMatplotlibPrivate::removeForm(const QString& key)
{
    //this->formVisualization[key]->disconnect();
//        this->formVisualization[key]->clearConnections();
//        this->formVisualization[key]->clear();
    //delete this->formVisualization[key];
    this->formVisualization.remove(key);

    this->formModified.remove(key);

    this->forms.remove(key);

    this->render();

    q->emit formRemoved(key);
}

void gnomonViewMatplotlibPrivate::clear(void)
{
    for (const auto& key : this->formVisualization.keys()) {
        this->removeForm(key);
    }
    if (this->figureNumber != -1) {
        int stat;
        QString clearStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.clf()\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(clearStatement, &stat);
    }
}

void gnomonViewMatplotlibPrivate::updateFormVisualization(const QString& name)
{
    auto&& visu = this->formVisualization[name];
    auto&& form = this->forms[name];
   
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

    if (update) {
        visu->update();
    }
}

void gnomonViewMatplotlibPrivate::setFormVisualization(const QString& name, const QString& visu)
{
    if (!this->formVisualizationNames.contains(name) || this->formVisualizationNames[name] != visu) {

        if (this->formVisualization[name]) {
            this->formVisualization[name]->clear();
            //delete this->formVisualization[name];
            //this->formVisualization[name] = nullptr;
        }

        if (name == "gnomonDataFrame") {
            this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame>(gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(visu));
        } else if (name == "gnomonLString") {
            this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString>(gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(visu));
        } else if (name == "gnomonTree") {
            this->formVisualization[name] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree>(gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(visu));
        }
    }

    this->formVisualizationNames[name] = visu;
    this->updateFormVisualization(name);
}

void gnomonViewMatplotlibPrivate::render(void)
{
    if (this->figureNumber != -1) {
        int stat;
        QString clearStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(clearStatement, &stat);
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

gnomonViewMatplotlib::gnomonViewMatplotlib(QObject *parent) : QObject(parent)
{
    d = new gnomonViewMatplotlibPrivate(this);
    d->q = this;
//
//    static int count = 0;
//    d->figureNumber = count++;

    loadPluginGroup("matplotlibVisualizationTree");
    loadPluginGroup("matplotlibVisualizationDataFrame");
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
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonLStringAdapterCommand;
                d->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        } else if (form=="gnomonTree") {
            loadPluginGroup("treeAdapter");
            for (const auto& key : gnomonCore::treeAdapter::pluginFactory().keys())
            {
                gnomonAbstractTreeAdapter *adapter = dynamic_cast<gnomonAbstractTreeAdapter *>(gnomonCore::treeAdapter::pluginFactory().create(key));
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonTreeAdapterCommand;
                d->adapterCommands[form][key]->setAlgorithmName(key);
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
    delete d;
}

void gnomonViewMatplotlib::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (std::shared_ptr<gnomonTreeSeries> tree = std::dynamic_pointer_cast<gnomonTreeSeries>(form)) {
        if (d->acceptForms["gnomonTree"]) {
            d->forms["gnomonTree"] = tree;
            if ((!d->formVisualization.contains("gnomonTree"))||(!d->formVisualization["gnomonTree"])) {
                QString key = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys()[0];

                d->formVisualization["gnomonTree"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree>(gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(key));
                d->formVisualization["gnomonTree"]->setView(this);
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationTree> formVisualizationTree = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationTree>(d->formVisualization["gnomonTree"]);
            formVisualizationTree->setTree(std::dynamic_pointer_cast<gnomonTree>(tree->current()));
            this->setIsModifiedForm("gnomonTree");
            emit formAdded("gnomonTree");
        } else {
            this->setAdaptedForm("gnomonTree", tree);
        }
    } else if (std::shared_ptr<gnomonDataFrameSeries> dataFrame = std::dynamic_pointer_cast<gnomonDataFrameSeries>(form)) {
        if (d->acceptForms["gnomonDataFrame"]) {
            d->forms["gnomonDataFrame"] = dataFrame;

            if ((!d->formVisualization.contains("gnomonDataFrame"))||(!d->formVisualization["gnomonDataFrame"]))
            {
                QString key = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys()[0];
                d->formVisualization["gnomonDataFrame"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame>(gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(key));
                d->formVisualization["gnomonDataFrame"]->setView(this);
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationDataFrame> formVisualizationDataFrame = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationDataFrame>(d->formVisualization["gnomonDataFrame"]);
            formVisualizationDataFrame->setDataFrame(std::dynamic_pointer_cast<gnomonDataFrame>(dataFrame->current()));
            this->setIsModifiedForm("gnomonDataFrame");
            emit formAdded("gnomonDataFrame");
        }
    } else if (std::shared_ptr<gnomonLStringSeries> lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form)) {
        if (d->acceptForms["gnomonLString"]) {
            d->forms["gnomonLString"] = lString;

            if ((!d->formVisualization.contains("gnomonLString"))||(!d->formVisualization["gnomonLString"]))
            {
                QString key = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys()[0];
                d->formVisualization["gnomonLString"] = std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString>(gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(key));
                d->formVisualization["gnomonLString"]->setView(this);
            }
            std::shared_ptr<gnomonAbstractMatplotlibVisualizationLString> formVisualizationLString = std::dynamic_pointer_cast<gnomonAbstractMatplotlibVisualizationLString>(d->formVisualization["gnomonLString"]);
            formVisualizationLString->setLString(std::dynamic_pointer_cast<gnomonLString>(lString->current()));
            this->setIsModifiedForm("gnomonLString");
            emit formAdded("gnomonLString");
        } else {
            this->setAdaptedForm("gnomonLString", lString);
        }
    }
}

QStringList gnomonViewMatplotlib::formNames(void)
{
     return d->forms.keys();
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonViewMatplotlib::form(const QString& name)
{
  return d->forms[name];
}

void gnomonViewMatplotlib::clearForm(const QString& name)
{
    return d->removeForm(name);
}

void gnomonViewMatplotlib::setAdaptedForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, gnomonAbstractMatplotlibVisualization *visualization)
{
    if (d->adapterCommands.contains(name)) {
        QVariantMap adapter_descs;
        for (const auto &key : d->adapterCommands[name].keys()) {
            if (d->acceptForms[d->adapterTargets[name][key]]) {
                adapter_descs[key] = d->adapterDescriptions[name][key];
            }
        }
        if (adapter_descs.size() > 0) {
            // TODO : emit adaptig signal?
            /*d->form_to_adapt = form;
            d->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
            d->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
            d->adapter_menu->resize(dtkApp->window()->width() * 2/5, dtkApp->window()->height() - 40);
            d->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
            d->adapter_menu->show();

            QObject *context = d->adapter_menu->rootObject();
            connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(d->adapter_menu);*/
        }
    }
}

void gnomonViewMatplotlib::setAcceptForm(const QString& name, bool accept)
{
    if (d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
    }
}

QStringList gnomonViewMatplotlib::acceptedForms(void)
{
    QStringList forms;
    for (const auto& name : d->acceptForms.keys()) {
        if (d->acceptForms[name]) {
            forms << name;
        }
    }
    return forms;
}

void gnomonViewMatplotlib::setIsModifiedForm(const QString& name)
{
    d->formModified[name] = true;
    this->updateVisualizations();
}

bool gnomonViewMatplotlib::inputView(void)
{
    return d->input_view;
}

void gnomonViewMatplotlib::setInputView(bool input)
{
    d->input_view = input;
}

void gnomonViewMatplotlib::updateVisualizations(void)
{
    for (const auto& name : d->formVisualization.keys()) {
        if (d->formModified[name]) {
            d->formVisualization[name]->update();
            d->formModified[name] = false;
        }
    }
}


QString gnomonViewMatplotlib::formVisuName(const QString& name)
{
    QString visu_name;
    if (d->forms.contains(name)) {
        visu_name = d->formVisualizationNames[name];
    }
    return visu_name;
}

QStringList gnomonViewMatplotlib::formVisualizations(const QString& name)
{   
    QStringList visu_names;
    
    if (d->forms.contains(name)) {  
        if (name == "gnomonDataFrame") {
             visu_names = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys();
         } else if (name == "gnomonLString") {
             visu_names = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys();
         } else if (name == "gnomonTree") {
             visu_names = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys();
         }
    }
    
    return visu_names;
}

void gnomonViewMatplotlib::setFormVisuName(const QString& name, const QString& visu_name)
{
    if (d->forms.contains(name)) {
        d->setFormVisualization(name, visu_name);
    }
}

QJSValue gnomonViewMatplotlib::formVisuParameters(const QString& name)
{
    if (d->forms.contains(name)) {
        return dtkCoreParameterCollection(d->formVisualization[name]->parameters()).toJSValue(this->parent());
    } else {
        return QJSValue();
    }
}

void gnomonViewMatplotlib::render(void)
{
    d->render();
}

void gnomonViewMatplotlib::update(void)
{
     for (const auto& key : d->formVisualization.keys()) {
         d->formVisualization[key]->update();
     }
}

void gnomonViewMatplotlib::clear(void)
{
    d->clear();
    emit formsChanged();
}

void gnomonViewMatplotlib::transmit(void)
{
    d->exportToManager();
}

int gnomonViewMatplotlib::figureNumber(void)
{
    return d->figureNumber;
}

void gnomonViewMatplotlib::setFigureNumber(int num)
{
    d->figureNumber = num;
    emit figureNumberChanged(num);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
