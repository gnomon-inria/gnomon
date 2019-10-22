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

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameReaderCommand>

#include <gnomonCore>
#include <gnomonWidgets>

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

public:
    QMap<QString, gnomonAbstractDynamicForm *> forms;
    QMap<QString, gnomonAbstractMatplotlibVisualization *> formVisualization;
    QMap<QString, gnomonAbstractCommand *> formReaderCommand;

public:
    int figureNumber;

public:
    // gnomonOverlayPaneItem *paneItemButton = nullptr;

    QPushButton *renderButton = nullptr;

    QMap<QString, QFormLayout *> parameterLayouts;
    // QMap<QString, gnomonOverlayPaneItem *> formVisualizationPaneItems;

    // gnomonOverlayPane *formVisualizationPane = nullptr;

public:
    dtkWidgetsMenu *menu(void);
    
public:
    // gnomonOverlayPane *pane(QWidget *parent);
    dtkWidgetsMenu *view_menu;


public slots:
    void configure(QWidget *parent, const QString& key);
    void refresh(void);

};

gnomonViewMatplotlibPrivate::gnomonViewMatplotlibPrivate(QWidget *parent) : QWidget(parent)
{
    this->layout = new QVBoxLayout(this);

    this->export_button = new gnomonOverlayButton(fa::arrowcircleup, "", parent);
    this->save_button = new gnomonOverlayButton(fa::save, "", parent);

    static int count = 0;
    this->view_menu = new dtkWidgetsMenu(fa::circlethin, "View " + QString::number(count++));
}

gnomonViewMatplotlibPrivate::~gnomonViewMatplotlibPrivate(void)
{
}

void gnomonViewMatplotlibPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->formVisualization[key]);
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

    QString figure_number = "0";
    for(int row = 0, max_row = this->layout->count(); row < max_row; ++row) {
        QLayoutItem *item = this->layout->itemAt(0);
        QWidget *widget = item->widget();
        if (widget->objectName().contains("MplTabWidget")) {
            QStringList name_items = widget->objectName().split(" ");
            if (name_items.size()>1) {
                figure_number = name_items[2];
            }
        }
    }

    //TODO: Script
    // int stat;
    // dtkScriptInterpreterPython::instance()->interpret("import matplotlib.pyplot as plt", &stat);
    // QString figure_statement = "figure = plt.figure("+figure_number+")";
    // dtkScriptInterpreterPython::instance()->interpret(figure_statement, &stat);
    // dtkScriptInterpreterPython::instance()->interpret("s = figure.get_size_inches()", &stat);
    // dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(10,10)", &stat);
    // QString save_statement = "figure.savefig('"+export_file_path+"')";
    // dtkScriptInterpreterPython::instance()->interpret(save_statement, &stat);
    // dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(*s)", &stat);

}

QSize gnomonViewMatplotlibPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void gnomonViewMatplotlibPrivate::resizeEvent(QResizeEvent *event)
{
    this->export_button->move(event->size().width() - 40, 10);
    this->save_button->move(event->size().width() -80, 10);

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

void gnomonViewMatplotlibPrivate::configure(QWidget *parent, const QString& key)
{
    // if (this->formVisualization.contains(key)) {
    //     gnomonAbstractMatplotlibVisualization *v = this->formVisualization[key];
    //     if(v) {
    //         if ((this->parameterLayouts.contains(key))&&(this->parameterLayouts[key])) {
    //             for(int row = 0, max_row = this->parameterLayouts[key]->count(); row < max_row; ++row) {
    //                 QLayoutItem *forDeletion = this->parameterLayouts[key]->takeAt(0);
    //                 forDeletion->widget()->disconnect();
    //                 delete forDeletion->widget();
    //                 delete forDeletion;
    //             }
    //         } else {
    //             this->parameterLayouts[key] = new QFormLayout;
    //             this->parameterLayouts[key]->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    //         }

    //         if (!this->formVisualizationPaneItems.contains(key)) {
    //             this->refresh();
    //         }
    //         this->formVisualizationPaneItems[key]->addLayout(this->parameterLayouts[key]);

    //         QMap<QString, gnomonCoreParameter *> parameters = v->parameters();
    //         for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
    //             QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
    //             if (widget) {
    //                 this->parameterLayouts[key]->addRow(it.key(), widget);
    //             }
    //         }
    //     }
    // }

    // this->refresh();
}

void gnomonViewMatplotlibPrivate::refresh(void)
{
//    this->formVisualizationPane->clearLayout();
//
//    for (const auto& key : this->formVisualization.keys()) {
//
//        if ((!this->formVisualizationPaneItems.contains(key))||(!this->formVisualizationPaneItems[key])) {
//            this->formVisualizationPaneItems[key] = new gnomonOverlayPaneItem((QWidget *) q->parent());
//            this->formVisualizationPaneItems[key]->setTitle(key+" Visualization");
//
//            QComboBox *combo_box = new QComboBox(this);
//            QStringList combo_box_keys = {};
//            if (key == "gnomonTree") {
//                combo_box_keys = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().keys();
//            } else  if (key == "gnomonDataFrame") {
//                combo_box_keys = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().keys();
//            } else  if (key == "gnomonLString") {
//                combo_box_keys = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys();
//            }
//            for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
//                combo_box->addItem(*it);
//            }
//            combo_box->model()->sort(0);
//
//            QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& visu) {
//                if (this->formVisualization[key]) {
//                    delete this->formVisualization[key];
//                    this->formVisualization[key] = nullptr;
//                }
//
//                if (key == "gnomonTree") {
//                    this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationTree::pluginFactory().create(visu);
//                    this->formVisualization[key]->setView(q);
//                    gnomonAbstractMatplotlibVisualizationTree *formVisualizationTree = (gnomonAbstractMatplotlibVisualizationTree *)this->formVisualization[key];
//                    gnomonTree *tree = (gnomonTree *)this->forms[key];
//                    formVisualizationTree->setTree(tree);
//                    formVisualizationTree->update();
//                } else if (key == "gnomonDataFrame") {
//                    this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationDataFrame::pluginFactory().create(visu);
//                    this->formVisualization[key]->setView(q);
//                    gnomonAbstractMatplotlibVisualizationDataFrame *formVisualizationDataFrame = (gnomonAbstractMatplotlibVisualizationDataFrame *)this->formVisualization[key];
//                    gnomonDataFrame *dataFrame = (gnomonDataFrame *)this->forms[key];
//                    formVisualizationDataFrame->setDataFrame(dataFrame);
//                    formVisualizationDataFrame->update();
//                } else if (key == "gnomonLString") {
//                    this->formVisualization[key] = gnomonVisualization::matplotlibVisualizationLString::pluginFactory().create(visu);
//                    this->formVisualization[key]->setView(q);
//                    gnomonAbstractMatplotlibVisualizationLString *formVisualizationLString = (gnomonAbstractMatplotlibVisualizationLString *)this->formVisualization[key];
//                    gnomonLString *lString = (gnomonLString *)this->forms[key];
//                    formVisualizationLString->setLString(lString);
//                    formVisualizationLString->update();
//                }
//                this->configure((QWidget *) q->parent(), key);
//            });
//
//            this->formVisualizationPaneItems[key]->addWidget(combo_box);
//            this->formVisualizationPaneItems[key]->toggle();
//        }
//
//        this->formVisualizationPane->addWidget(this->formVisualizationPaneItems[key]);
//    }
//
//    this->formVisualizationPane->addWidget(this->paneItemButton);
}

dtkWidgetsMenu *gnomonViewMatplotlibPrivate::menu(void)
{
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

    static int count = 0;
    d->figureNumber = count++;

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d, 0, 0, 1, 1);
    // layout->addWidget(d->pane(parent), 0, 1, 1, 1);

    connect(d->export_button, SIGNAL(iconClicked()), d, SLOT(exportToManager()));
    connect(d->save_button, SIGNAL(iconClicked()), d, SLOT(saveFigure()));

    this->setObjectName("ViewMatplotlib");

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
        d->configure((QWidget *)this->parent(), key);
    });

    // connect(d->renderButton, &QPushButton::clicked, [=] () {
    //     for (const auto& key : d->formVisualization.keys()) {
    //         gnomonAbstractMatplotlibVisualization *v = d->formVisualization[key];
    //         if(v) {
    //             v->update();
    //         }
    //     }
    // });

    this->setAcceptDrops(true);
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete d;
}

void gnomonViewMatplotlib::setForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractMatplotlibVisualization *visualization)
{
    if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        d->forms["gnomonTree"] = tree;

        loadPluginGroup("matplotlibVisualizationTree");


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
        formVisualizationTree->update();

        emit formAdded("gnomonTree");
    } else if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        d->forms["gnomonDataFrame"] = dataFrame;

        loadPluginGroup("matplotlibVisualizationDataFrame");

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
        formVisualizationDataFrame->update();

        emit formAdded("gnomonDataFrame");
    } else if (gnomonLStringSeries *lString = dynamic_cast<gnomonLStringSeries *>(form)) {
        d->forms["gnomonLString"] = lString;

        loadPluginGroup("matplotlibVisualizationLString");

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
        formVisualizationLString->update();

        emit formAdded("gnomonLString");
    }
}

gnomonAbstractDynamicForm *gnomonViewMatplotlib::form(const QString& name)
{
  return d->forms[name];
}

void gnomonViewMatplotlib::addWidget(QWidget *widget)
{
    //TODO
    dtkThemesEngine::instance()->color("@bg");
//    widget->setStyleSheet( gnomonStyleSheet());

    d->layout->addWidget(widget);

    this->resize(800,this->height());
}

int gnomonViewMatplotlib::figureNumber(void)
{
    return d->figureNumber;
}

dtkWidgetsMenu *gnomonViewMatplotlib::menu(void)
{
    return d->menu();
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
//        this->setForm("formManager",form);

    } else {
        if ((path.endsWith("xml")) || (path.endsWith("txt")))   {
            if ((!d->formReaderCommand.contains("gnomonTree"))||(!d->formReaderCommand["gnomonTree"]))
                d->formReaderCommand["gnomonTree"] = new gnomonTreeReaderCommand("gnomonTreeReaderTreex");
            gnomonTreeReaderCommand *treeCommand = (gnomonTreeReaderCommand *) d->formReaderCommand["gnomonTree"];
            treeCommand->setPath(path.remove("file://"));
            treeCommand->redo();

            gnomonTreeSeries * tree = (gnomonTreeSeries *) treeCommand->tree()->clone();
            if (!tree) {
                qWarning() << Q_FUNC_INFO << "Resulting tree is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonTree",tree);
        } else if (path.endsWith("csv")) {
            if ((!d->formReaderCommand.contains("gnomonDataFrame"))||(!d->formReaderCommand["gnomonDataFrame"]))
                d->formReaderCommand["gnomonDataFrame"] = new gnomonDataFrameReaderCommand("gnomonDataFrameReaderPandas");
            gnomonDataFrameReaderCommand *dataFrameCommand = (gnomonDataFrameReaderCommand *) d->formReaderCommand["gnomonDataFrame"];
            dataFrameCommand->setPath(path.remove("file://"));
            dataFrameCommand->redo();

            gnomonDataFrameSeries * dataFrame = (gnomonDataFrameSeries *) dataFrameCommand->dataFrame()->clone();
            if (!dataFrame) {
                qWarning() << Q_FUNC_INFO << "Resulting dataframe is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonDataFrame",dataFrame);
        }
    }

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
