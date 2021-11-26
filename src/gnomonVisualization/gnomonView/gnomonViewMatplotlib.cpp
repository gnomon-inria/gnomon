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

#include "gnomonViewMatplotlib.h"

#include <dtkScript>

#include <gnomonCore>
#include <gnomonPipeline>

#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeAdapterCommand>

#include "gnomonFormAdapterMenu.h"

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
    QMap<QString, gnomonAbstractDynamicForm *> forms;
    QMap<QString, gnomonAbstractMatplotlibVisualization *> formVisualization;

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
//    void configure(dtkWidgetsMenuItemDIY *parent, const QString& key);
//    void addFormMenu(const QString& key);
    void refresh(void);

public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    gnomonAbstractDynamicForm *form_to_adapt = nullptr;

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
        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->formVisualization[key]);
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
    this->formVisualization[key]->disconnect();
//        this->formVisualization[key]->clearConnections();
//        this->formVisualization[key]->clear();
    delete this->formVisualization[key];
    this->formVisualization.remove(key);

    this->formModified.remove(key);

    this->forms.remove(key);

    this->refresh();

    q->emit formRemoved(key);
}

void gnomonViewMatplotlibPrivate::clear(void)
{
    for (const auto& key : this->formVisualization.keys()) {
        this->removeForm(key);
    }
}

/*void gnomonViewMatplotlibPrivate::configure(dtkWidgetsMenuItemDIY *parent, const QString& key)
{
    if (this->formVisualization.contains(key)) {

        gnomonAbstractMatplotlibVisualization *v = this->formVisualization[key];

        if(v) {
             if ((this->parameterLayouts.contains(key)) && (this->parameterLayouts[key])) {
                 for(int row = 0, max_row = this->parameterLayouts[key]->count(); row < max_row; ++row) {
                     QLayoutItem *forDeletion = this->parameterLayouts[key]->takeAt(0);
                     forDeletion->widget()->disconnect();
                     delete forDeletion->widget();
                     delete forDeletion;
                 }
             } else {

                this->parameterLayouts[key] = new QFormLayout;
                this->parameterLayouts[key]->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

                parent->addLayout(this->parameterLayouts[key]);
            }

            dtkCoreParameters parameters = v->parameters();

            QList<QString> keys = parameters.keys();
            keys.sort();
            for(QString k : keys) {
                QWidget *widget = gnomonWidgetsParameter::widget(parameters[k], 0);
                if (widget) {
                    this->parameterLayouts[key]->addRow(k, widget);
                }
            }
        }
    }

    this->refresh();
}

void gnomonViewMatplotlibPrivate::addFormMenu(const QString& key)
{
    if ((!this->formVisualizationPaneItems.contains(key))||(!this->formVisualizationPaneItems[key]))
    {
        this->formVisualizationMenus[key] = new dtkWidgetsMenu(fa::table, key);
        this->formVisualizationPaneItems[key] = new dtkWidgetsMenuItemDIY(key);

        this->formVisualizationPaneItems[key]->setShowTitle(false);
        this->formVisualizationMenus[key]->addItem(this->formVisualizationPaneItems[key]);

        QComboBox *combo_box = new QComboBox;

        QWidget *contents = new QWidget;

        QStringList combo_box_keys = {};

        if (key == "gnomonTree") {
            combo_box_keys = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys();
        } else if (key == "gnomonDataFrame") {
            combo_box_keys = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys();
        } else if (key == "gnomonLString") {
            combo_box_keys = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys();
        }
        for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
            combo_box->addItem(*it);
        }
        combo_box->model()->sort(0);

        connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& visu)
        {
            if (this->formVisualization[key]) {
//                this->formVisualization[key]->clear();
                delete this->formVisualization[key];
                this->formVisualization[key] = nullptr;
            }

            if (key == "gnomonTree") {
                this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractMatplotlibVisualizationTree *formVisualizationTree = (gnomonAbstractMatplotlibVisualizationTree *)this->formVisualization[key];
                gnomonTreeSeries *tree = (gnomonTreeSeries *)this->forms[key];
                formVisualizationTree->setTree(dynamic_cast<gnomonTree *>(tree->current()));
                q->setIsModifiedForm("gnomonTree");
            } else if (key == "gnomonDataFrame") {
                this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractMatplotlibVisualizationDataFrame *formVisualizationDataFrame = (gnomonAbstractMatplotlibVisualizationDataFrame *)this->formVisualization[key];
                gnomonDataFrameSeries *dataFrame = (gnomonDataFrameSeries *)this->forms[key];
                formVisualizationDataFrame->setDataFrame(dynamic_cast<gnomonDataFrame *>(dataFrame->current()));
                q->setIsModifiedForm("gnomonDataFrame");
            } else if (key == "gnomonLString") {
                this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractMatplotlibVisualizationLString *formVisualizationLString = (gnomonAbstractMatplotlibVisualizationLString *)this->formVisualization[key];
                gnomonLStringSeries *lString = (gnomonLStringSeries *)this->forms[key];
                formVisualizationLString->setLString(dynamic_cast<gnomonLString *>(lString->current()));
                q->setIsModifiedForm("gnomonLString");
            }

            this->configure(formVisualizationPaneItems[key], key);
        });

        this->formVisualizationPaneItems[key]->addWidget(combo_box);
        this->formVisualizationPaneItems[key]->addWidget(contents);

//        this->formVisualizationMenus[key]->addItem(this->view_item);
        this->view_menubar->addMenu(this->formVisualizationMenus[key]);
        this->view_menubar->touch();
    }

}*/

void gnomonViewMatplotlibPrivate::refresh(void)
{
//    int stat;
//    QString refreshStatement = "import matplotlib.pyplot as plt\nfigure = plt.figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
//    dtkScriptInterpreterPython::instance()->interpret(refreshStatement, &stat);
}

void gnomonViewMatplotlibPrivate::adaptForm(const QString& adapter_plugin)
{
    gnomonAbstractDynamicForm *form = this->form_to_adapt;
    qDebug()<<Q_FUNC_INFO<<form;

    if (gnomonLStringSeries* lString = dynamic_cast<gnomonLStringSeries *>(form)) {
        gnomonLStringAdapterCommand *lStringCommand = dynamic_cast<gnomonLStringAdapterCommand *>(this->adapterCommands["gnomonLString"][adapter_plugin]);

        lStringCommand->setInput(lString);
        lStringCommand->redo();
        gnomonAbstractDynamicForm *adaptedLString = lStringCommand->output();
        if (adaptedLString) {
            gnomonPipeline::instance()->addAdapter(lStringCommand);
            q->setForm("gnomonLString",adaptedLString);
        }
    } else if (gnomonTreeSeries* tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        qDebug()<<Q_FUNC_INFO<<tree<<adapter_plugin;
        gnomonTreeAdapterCommand *treeCommand = dynamic_cast<gnomonTreeAdapterCommand *>(this->adapterCommands["gnomonTree"][adapter_plugin]);

        treeCommand->setInput(tree);
        qDebug()<<Q_FUNC_INFO<<treeCommand->input();
        treeCommand->redo();
        qDebug()<<Q_FUNC_INFO<<treeCommand->output();
        gnomonAbstractDynamicForm *adaptedTree = treeCommand->output();
        if (adaptedTree) {
            gnomonPipeline::instance()->addAdapter(treeCommand);
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
    dtkScriptInterpreterPython::instance()->interpret("import gnomon_utils.gnomonMpl", &stat);

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

    /*connect(this, &gnomonViewMatplotlib::formAdded, [=] (const QString& key) {
        d->addFormMenu(key);
        d->configure(d->formVisualizationPaneItems[key], key);
//        this->render();
    });*/

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

void gnomonViewMatplotlib::setForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractMatplotlibVisualization *visualization)
{
    qDebug()<<Q_FUNC_INFO<<form;
    if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        qDebug()<<Q_FUNC_INFO<<"gnomonTree"<<tree<<d->acceptForms["gnomonTree"];
        if (d->acceptForms["gnomonTree"]) {
            d->forms["gnomonTree"] = tree;

            QString key = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys()[0];

            if ((!d->formVisualization.contains("gnomonTree"))||(!d->formVisualization["gnomonTree"]))
            {
                d->formVisualization["gnomonTree"] = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(key);
                d->formVisualization["gnomonTree"]->setView(this);
            }
            gnomonAbstractMatplotlibVisualizationTree *formVisualizationTree = (gnomonAbstractMatplotlibVisualizationTree *)d->formVisualization["gnomonTree"];
            formVisualizationTree->setTree(dynamic_cast<gnomonTree *>(tree->current()));
            if (visualization) {
                formVisualizationTree->setParameters(visualization->parameters());
            }
            this->setIsModifiedForm("gnomonTree");
            emit formAdded("gnomonTree");
        } else {
            this->setAdaptedForm("gnomonTree", tree, visualization);
        }
    } else if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        if (d->acceptForms["gnomonDataFrame"]) {
            d->forms["gnomonDataFrame"] = dataFrame;

            QString key = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys()[0];

            if ((!d->formVisualization.contains("gnomonDataFrame"))||(!d->formVisualization["gnomonDataFrame"]))
            {
                d->formVisualization["gnomonDataFrame"] = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(key);
                d->formVisualization["gnomonDataFrame"]->setView(this);
            }
            gnomonAbstractMatplotlibVisualizationDataFrame *formVisualizationDataFrame = (gnomonAbstractMatplotlibVisualizationDataFrame *)d->formVisualization["gnomonDataFrame"];
            formVisualizationDataFrame->setDataFrame(dynamic_cast<gnomonDataFrame *>(dataFrame->current()));
            if (visualization) {
                formVisualizationDataFrame->setParameters(visualization->parameters());
            }
            this->setIsModifiedForm("gnomonDataFrame");
            emit formAdded("gnomonDataFrame");
        }
    } else if (gnomonLStringSeries *lString = dynamic_cast<gnomonLStringSeries *>(form)) {
        qDebug()<<Q_FUNC_INFO<<"gnomonLString"<<lString<<d->acceptForms["gnomonLString"];
        if (d->acceptForms["gnomonLString"]) {
            d->forms["gnomonLString"] = lString;

            QString key = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys()[0];

            if ((!d->formVisualization.contains("gnomonLString"))||(!d->formVisualization["gnomonLString"]))
            {
                d->formVisualization["gnomonLString"] = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(key);
                d->formVisualization["gnomonLString"]->setView(this);
            }
            gnomonAbstractMatplotlibVisualizationLString *formVisualizationLString = (gnomonAbstractMatplotlibVisualizationLString *)d->formVisualization["gnomonLString"];
            formVisualizationLString->setLString(dynamic_cast<gnomonLString *>(lString->current()));
            if (visualization) {
                formVisualizationLString->setParameters(visualization->parameters());
            }
            this->setIsModifiedForm("gnomonLString");
            emit formAdded("gnomonLString");
        } else {
            this->setAdaptedForm("gnomonLString", lString, visualization);
        }
    }
}

QStringList gnomonViewMatplotlib::formNames(void)
{
     return d->forms.keys();
}

gnomonAbstractDynamicForm *gnomonViewMatplotlib::form(const QString& name)
{
  return d->forms[name];
}

void gnomonViewMatplotlib::clearForm(const QString& name)
{
    return d->removeForm(name);
}

void gnomonViewMatplotlib::setAdaptedForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractMatplotlibVisualization *visualization)
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
    qDebug()<<Q_FUNC_INFO<<d->formModified;
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
