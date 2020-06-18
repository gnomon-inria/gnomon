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

#include <dtkThemes>
#include <dtkWidgets>
#include <dtkScript>

#include <gnomonCore>
#include <gnomonComposer>
#include <gnomonWidgets>

#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringAdapterCommand>

#include "gnomonFormAdapterMenu.h"

#include "gnomonManager/gnomonFormManager.h"

#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"
#include "gnomonVisualizations/gnomonDataFrame/gnomonAbstractMatplotlibVisualizationDataFrame.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h"
#include "gnomonVisualizations/gnomonTree/gnomonAbstractMatplotlibVisualizationTree.h"


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewMatplotlibPrivate : public QWidget
{
    Q_OBJECT

public:
     gnomonViewMatplotlibPrivate(QWidget *parent = Q_NULLPTR);
    ~gnomonViewMatplotlibPrivate(void);

public:
    QVBoxLayout *layout;

public:
    gnomonViewMatplotlib *q = nullptr;

public:
    QSize sizeHint(void) const;

protected:
    void resizeEvent(QResizeEvent *);

public:
    gnomonOverlayButton *export_button = nullptr;
    gnomonOverlayButton *save_button = nullptr;
    QColor export_color = QColor("#cccccc");

public slots:
    void exportToManager(void);
    void saveFigure(void);

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

public:
    dtkWidgetsMenuItemDIY *paneItemButton = nullptr;

    QPushButton *renderButton = nullptr;
    QPushButton *clearButton = nullptr;

public:
    QMap<QString, QFormLayout *> parameterLayouts;
    
    QMap<QString, dtkWidgetsMenu *> formVisualizationMenus;
    QMap<QString, dtkWidgetsMenuItemDIY *> formVisualizationPaneItems;

    // gnomonOverlayPane *formVisualizationPane = nullptr;

public:
    dtkWidgetsMenu *menu(void);
    
public:
    // gnomonOverlayPane *pane(QWidget *parent);
    dtkWidgetsMenu *view_menu;
    dtkWidgetsMenuBar *view_menubar;

public:
    gnomonFormAdapterMenu *adapter_menu = nullptr;

public slots:
    void configure(dtkWidgetsMenuItemDIY *parent, const QString& key);
    void addFormMenu(const QString& key);
    void refresh(void);

public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    gnomonAbstractDynamicForm *form_to_adapt = nullptr;

public slots:
    void adaptForm(const QString& adapter_plugin);
};

gnomonViewMatplotlibPrivate::gnomonViewMatplotlibPrivate(QWidget *parent) : QWidget(parent)
{
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(38,0,0,0);

    this->export_button = new gnomonOverlayButton(fa::arrowcircleup, "", parent);
    this->save_button = new gnomonOverlayButton(fa::save, "", parent);

    static int count = 0;
    this->figureNumber = count++;
    this->view_menu = new dtkWidgetsMenu(fa::square, "Matplotlib Figure " + QString::number(this->figureNumber));
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

void gnomonViewMatplotlibPrivate::saveFigure(void)
{
    QSettings settings("inria", "gnomon");
    settings.beginGroup("General");
    QString path = settings.value("last_saved_file", QDir::homePath()).toString();
    settings.endGroup();

    QString export_file_path;
    export_file_path = QFileDialog::getSaveFileName(this, tr("Save figure"), path, tr("Figures (*.png *.eps *.pdf *.svg)"));

    QString figure_number = QString::number(this->figureNumber);
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import matplotlib.pyplot as plt", &stat);
    QString figure_statement = "figure = plt.figure("+figure_number+")";
    dtkScriptInterpreterPython::instance()->interpret(figure_statement, &stat);
    dtkScriptInterpreterPython::instance()->interpret("s = figure.get_size_inches()", &stat);
    dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(10,10)", &stat);
    QString save_statement = "figure.savefig('"+export_file_path+"')";
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

    this->parameterLayouts[key]->disconnect();
    delete this->parameterLayouts[key];
    this->parameterLayouts.remove(key);

    this->formVisualizationMenus[key]->removeItem(this->formVisualizationPaneItems[key]);

    this->formVisualizationPaneItems[key]->disconnect();
    this->formVisualizationPaneItems[key]->clear();
    delete this->formVisualizationPaneItems[key];
    this->formVisualizationPaneItems.remove(key);

    this->view_menubar->removeMenu(this->formVisualizationMenus[key]);

    this->formVisualizationMenus[key]->disconnect();
    this->formVisualizationMenus[key]->clear();
    delete this->formVisualizationMenus[key];
    this->formVisualizationMenus.remove(key);

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

QSize gnomonViewMatplotlibPrivate::sizeHint(void) const
{
    return QSize(1200, 800);
}

void gnomonViewMatplotlibPrivate::resizeEvent(QResizeEvent *event)
{
    this->export_button->move(event->size().width() - 40, 10);
    this->save_button->move(event->size().width() -80, 10);

    if (this->view_menubar)
        this->view_menubar->setFixedHeight(event->size().height());

    QWidget::resizeEvent(event);
}

// gnomonOverlayPane *gnomonViewMatplotlibPrivate::pane(QWidget *parent)
// {
//     if(!this->formVisualizationPane) {
//         this->formVisualizationPane = new gnomonOverlayPane(parent);
//     }

//     if(!this->renderButton) {
//         this->renderButton = new QPushButton("Plot",parent);
//     }
//     this->renderButton->setCheckable(true);

//     if(!this->paneItemButton) {
//         this->paneItemButton = new gnomonOverlayPaneItem(parent);
//     }
//     this->paneItemButton->setTitle("View Form");
//     this->paneItemButton->addWidget(this->renderButton);
//     this->paneItemButton->toggle();

//     this->refresh();
//     this->formVisualizationPane->toggle();

//     return this->formVisualizationPane;
// }

void gnomonViewMatplotlibPrivate::configure(dtkWidgetsMenuItemDIY *parent, const QString& key)
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

            QMap<QString, gnomonCoreParameter *> parameters = v->parameters();

            for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
                QWidget *widget = gnomonWidgetsParameter::widget(it.value(), 0);
                if (widget) {
                    this->parameterLayouts[key]->addRow(it.key(), widget);
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

}

void gnomonViewMatplotlibPrivate::refresh(void)
{
    this->view_menu->removeItem(this->paneItemButton);

    for (const auto& menu : this->view_menubar->menus()) {
        this->view_menubar->removeMenu(menu);
    }

    for (const auto& key : this->formVisualizationMenus.keys()) {
        this->view_menubar->addMenu(this->formVisualizationMenus[key]);
    }

    this->view_menubar->addMenu(this->view_menu);
    this->view_menu->addItem(this->paneItemButton);

    this->view_menubar->touch();

//    int stat;
//    QString refreshStatement = "import matplotlib.pyplot as plt\nfigure = plt.figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
//    dtkScriptInterpreterPython::instance()->interpret(refreshStatement, &stat);
}

void gnomonViewMatplotlibPrivate::adaptForm(const QString& adapter_plugin)
{
    gnomonAbstractDynamicForm *form = this->form_to_adapt;

    if (gnomonLStringSeries* lString = dynamic_cast<gnomonLStringSeries *>(form)) {
        gnomonLStringAdapterCommand *lStringCommand = dynamic_cast<gnomonLStringAdapterCommand *>(this->adapterCommands["gnomonLString"][adapter_plugin]);

        lStringCommand->setInput(lString);
        lStringCommand->redo();
        gnomonAbstractDynamicForm *adaptedLString = lStringCommand->output();
        if (adaptedLString) {
            gnomonPipeline::instance()->addAdapter(lStringCommand);
            q->setForm("gnomonLString",adaptedLString);
        }
    }

    if (this->adapter_menu) {
        this->adapter_menu->close();
    }
}

dtkWidgetsMenu *gnomonViewMatplotlibPrivate::menu(void)
{
    if(!this->renderButton)
    {
        this->renderButton = new QPushButton("Render");

        connect(this->renderButton, &QPushButton::clicked, [=] ()
        {
            for (const auto& key : this->formVisualization.keys()) {
                gnomonAbstractMatplotlibVisualization *v = this->formVisualization[key];
                if(v) {
                    dtkApp->window()->setCursor(Qt::BusyCursor);
                    v->update();
                    dtkApp->window()->setCursor(Qt::ArrowCursor);
                }
            }
        });
    }

    if(!this->clearButton)
    {
        this->clearButton = new QPushButton("Clear");

        connect(this->clearButton, &QPushButton::clicked, [=] ()
        {
            this->clear();
        });
     }

    this->renderButton->setCheckable(true);
    this->clearButton->setCheckable(true);

    static int count = 0;

    if(!this->paneItemButton) {
        this->paneItemButton = new dtkWidgetsMenuItemDIY("View controls" + QString::number(count));
        this->paneItemButton->setShowTitle(false);
    }

    this->paneItemButton->addWidget(this->renderButton);
    this->paneItemButton->addWidget(this->clearButton);

    this->refresh();

    return this->view_menu;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewMatplotlib::gnomonViewMatplotlib(QWidget *parent) : QFrame(parent)
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
                d->adapterCommands[form][key] = new gnomonLStringAdapterCommand(key);
                delete adapter;
            }
        }
    }

    connect(d->export_button, SIGNAL(iconClicked()), d, SLOT(exportToManager()));
    connect(d->save_button, SIGNAL(iconClicked()), d, SLOT(saveFigure()));

    this->setObjectName("ViewMatplotlib");
    this->setMinimumHeight(8);

    QFile file(":gnomon/matplotlib_figure.py");

    if (file.open(QIODevice::ReadOnly)) {
        int stat;
        QString numberStatement = "num = " + QString::number(d->figureNumber);
        dtkScriptInterpreterPython::instance()->interpret(numberStatement, &stat);
        QString matplotlib_script  = file.readAll();
        file.close();
        dtkScriptInterpreterPython::instance()->interpret(matplotlib_script, &stat);
    } else {
        qWarning() << "Can't open matplotlib figure script";
    }

    connect(this, &gnomonViewMatplotlib::formAdded, [=] (const QString& key) {
        d->addFormMenu(key);
        d->configure(d->formVisualizationPaneItems[key], key);
//        this->render();
    });

    // connect(d->renderButton, &QPushButton::clicked, [=] () {
    //     for (const auto& key : d->formVisualization.keys()) {
    //         gnomonAbstractMatplotlibVisualization *v = d->formVisualization[key];
    //         if(v) {
    //             v->update();
    //         }
    //     }
    // });

    d->view_menubar = new dtkWidgetsMenuBar(d);
    d->view_menubar->setInteractive(false);
    d->view_menubar->setWidth(32);
    d->view_menubar->setMargins(6);
    d->view_menubar->addMenu(d->menu());
    d->view_menubar->touch();

    connect(d->view_menubar, &dtkWidgetsMenuBar::clicked, [=] () {
        this->resizeEvent(new QResizeEvent(this->size(), QSize()));
    });

    connect(d->view_menubar, &dtkWidgetsMenuBar::left, [=] () {
        this->resizeEvent(new QResizeEvent(this->size(), QSize()));
    });

    connect(d->view_menubar, &dtkWidgetsMenuBar::entered, [=] () {
        this->resizeEvent(new QResizeEvent(this->size(), QSize()));
    });

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
//    layout->addWidget(d->view_menubar, 0, 0, 1, 1);
    layout->addWidget(d, 0, 1, 1, 1);
    // layout->addWidget(d->pane(parent), 0, 1, 1, 1);

    this->setAcceptDrops(true);
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete d;
}

void gnomonViewMatplotlib::setForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractMatplotlibVisualization *visualization)
{
    if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
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
            d->form_to_adapt = form;
            d->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
            d->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
            d->adapter_menu->resize(dtkApp->window()->width() * 2/5, dtkApp->window()->height() - 40);
            d->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
            d->adapter_menu->show();

            QObject *context = d->adapter_menu->rootObject();
            connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(d->adapter_menu);
        }
    }
}

void gnomonViewMatplotlib::setAcceptForm(const QString& name, bool accept)
{
    if (d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
    }
}

void gnomonViewMatplotlib::setIsModifiedForm(const QString& name)
{
    d->formModified[name] = true;
    if (this->isVisible()) {
        this->updateVisualizations();
    }
}

void gnomonViewMatplotlib::setInputView(bool input)
{
    this->setAcceptDrops(input);
    if (input) {
        d->export_button->changeIcon(fa::arrowcircledown);
        d->export_button->toggle(false);
        d->export_button->activate(false);
    } else {
        d->export_button->changeIcon(fa::arrowcircleup);
        d->export_button->toggle(true);
        d->export_button->activate(true);
    }
}

void gnomonViewMatplotlib::updateVisualizations(void)
{
    for (const auto& name : d->formVisualization.keys()) {
        if (d->formModified[name]) {
            dtkApp->window()->setCursor(Qt::BusyCursor);
            d->formVisualization[name]->update();
            dtkApp->window()->setCursor(Qt::ArrowCursor);
            d->formModified[name] = false;
        }
    }
}

void gnomonViewMatplotlib::addWidget(QWidget *widget)
{
    //TODO
    dtkThemesEngine::instance()->color("@bg");
//    widget->setStyleSheet( gnomonStyleSheet());

    d->layout->addWidget(widget);

    this->resize(1200,this->height());
}

int gnomonViewMatplotlib::figureNumber(void)
{
    return d->figureNumber;
}

dtkWidgetsMenu *gnomonViewMatplotlib::menu(void)
{
    return d->menu();
}

void gnomonViewMatplotlib::showEvent(QShowEvent *event)
{
    this->updateVisualizations();
    QFrame::showEvent(event);
}

void gnomonViewMatplotlib::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->accept();
        return;
    }
    event->ignore();
}

void gnomonViewMatplotlib::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void gnomonViewMatplotlib::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void gnomonViewMatplotlib::dropEvent(QDropEvent *event)
{
    QString path = event->mimeData()->text();

    if(path.startsWith(":")) {
        gnomonAbstractDynamicForm *form = gnomonFormManager::instance()->get(path.remove(":").toInt());
        this->setForm("formManager",form);
        event->accept();
    } else if (path.startsWith("file://")) {
        emit fileDropped(path);
        event->accept();
    }
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
