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

#include "gnomonViewVolumicOverlay.h"
#include "gnomonFormManager.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include <gnomonCore/gnomonAbstractCommand>
#include <gnomonCore/gnomonTreeReaderCommand>

#include <gnomonCore/gnomonAbstractForm>
#include <gnomonCore/gnomonDataFrame>
#include <gnomonCore/gnomonTree>

#include <gnomonCore/gnomonCoreParameter.h>
#include "gnomonWidgetsParameter.h"

#include "gnomonAbstractMatplotlibVisualization.h"
#include "gnomonAbstractMatplotlibVisualizationDataFrame.h"
#include "gnomonAbstractMatplotlibVisualizationTree.h"

#include <gnomonFonts>
#include <gnomonStyle>

#include <dtkScript>


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
    gnomonViewVolumicOverlay *export_button = nullptr;
    QColor export_color = QColor("#cccccc");

public slots:
    void exportToManager(void);

public:
    QMap<QString, gnomonAbstractForm *> forms;
    QMap<QString, gnomonAbstractMatplotlibVisualization *> formVisualization;
    QMap<QString, gnomonAbstractCommand *> formReaderCommand;

public:
    gnomonOverlayPaneItem *paneItemButton = nullptr;
    QPushButton *renderButton = nullptr;

    QMap<QString, QFormLayout *> parameterLayouts;
    QMap<QString, gnomonOverlayPaneItem *> formVisualizationPaneItems;

    gnomonOverlayPane *formVisualizationPane = nullptr;

public:
    gnomonOverlayPane *pane(QWidget *parent);

public slots:
    void configure(QWidget *parent, const QString& key);
    void refresh(void);

};

gnomonViewMatplotlibPrivate::gnomonViewMatplotlibPrivate(QWidget *parent) : QWidget(parent)
{
    this->layout = new QVBoxLayout(this);

    this->export_button = new gnomonViewVolumicOverlay(fa::arrowcircleup, "", parent);
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

QSize gnomonViewMatplotlibPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void gnomonViewMatplotlibPrivate::resizeEvent(QResizeEvent *event)
{
    this->export_button->move(event->size().width() - 40, 10);

    QWidget::resizeEvent(event);
}

gnomonOverlayPane *gnomonViewMatplotlibPrivate::pane(QWidget *parent)
{
    if(!this->formVisualizationPane) {
        this->formVisualizationPane = new gnomonOverlayPane(parent);
    }

    if(!this->renderButton) {
        this->renderButton = new QPushButton("Plot",parent);
    }
    this->renderButton->setCheckable(true);

    if(!this->paneItemButton) {
        this->paneItemButton = new gnomonOverlayPaneItem(parent);
    }
    this->paneItemButton->setTitle("View Form");
    this->paneItemButton->addWidget(this->renderButton);
    this->paneItemButton->toggle();

    this->refresh();
    this->formVisualizationPane->toggle();

    return this->formVisualizationPane;
}

void gnomonViewMatplotlibPrivate::configure(QWidget *parent, const QString& key)
{
    if (this->formVisualization.contains(key)) {
        gnomonAbstractMatplotlibVisualization *v = this->formVisualization[key];
        if(v) {
            qDebug()<<Q_FUNC_INFO<<key;
            if ((this->parameterLayouts.contains(key))&&(this->parameterLayouts[key])) {
                for(int row = 0, max_row = this->parameterLayouts[key]->count(); row < max_row; ++row) {
                    QLayoutItem *forDeletion = this->parameterLayouts[key]->takeAt(0);
                    forDeletion->widget()->disconnect();
                    delete forDeletion->widget();
                    delete forDeletion;
                }
            } else {
                this->parameterLayouts[key] = new QFormLayout;
                this->parameterLayouts[key]->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
            }

            if (!this->formVisualizationPaneItems.contains(key)) {
                this->refresh();
            }
            this->formVisualizationPaneItems[key]->addLayout(this->parameterLayouts[key]);

            qDebug()<<Q_FUNC_INFO<<v;
            QMap<QString, gnomonCoreParameter *> parameters = v->parameters();
            qDebug()<<Q_FUNC_INFO<<parameters;
            for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
                QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
                if (widget) {
                    this->parameterLayouts[key]->addRow(it.key(), widget);
                }
            }
        }
    }

    this->refresh();
}

void gnomonViewMatplotlibPrivate::refresh(void)
{
    this->formVisualizationPane->clearLayout();

        for (const auto& key : this->formVisualization.keys()) {

        if ((!this->formVisualizationPaneItems.contains(key))||(!this->formVisualizationPaneItems[key])) {
            this->formVisualizationPaneItems[key] = new gnomonOverlayPaneItem((QWidget *) q->parent());
            this->formVisualizationPaneItems[key]->setTitle(key+" Visualization");

            QComboBox *combo_box = new QComboBox(this);
            QStringList combo_box_keys = {};
            if (key == "gnomonTree") {
                combo_box_keys = gnomonWidgets::matplotlibVisualizationTree::pluginFactory().keys();
            } else  if (key == "gnomonDataFrame") {
                combo_box_keys = gnomonWidgets::matplotlibVisualizationDataFrame::pluginFactory().keys();
            }
            for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
                combo_box->addItem(*it);
            }
            combo_box->model()->sort(0);

            QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& visu) {
                qDebug()<<"Visualization changed"<<visu;
                if (this->formVisualization[key]) {
                    delete this->formVisualization[key];
                    this->formVisualization[key] = nullptr;
                }

                if (key == "gnomonTree") {
                    this->formVisualization[key] = gnomonWidgets::matplotlibVisualizationTree::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractMatplotlibVisualizationTree *formVisualizationTree = (gnomonAbstractMatplotlibVisualizationTree *)this->formVisualization[key];
                    gnomonTree *tree = (gnomonTree *)this->forms[key];
                    formVisualizationTree->setTree(tree);
                    formVisualizationTree->update();
                } else if (key == "gnomonDataFrame") {
                    this->formVisualization[key] = gnomonWidgets::matplotlibVisualizationDataFrame::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractMatplotlibVisualizationDataFrame *formVisualizationDataFrame = (gnomonAbstractMatplotlibVisualizationDataFrame *)this->formVisualization[key];
                    gnomonDataFrame *dataFrame = (gnomonDataFrame *)this->forms[key];
                    formVisualizationDataFrame->setDataFrame(dataFrame);
                    formVisualizationDataFrame->update();
                }
                this->configure((QWidget *) q->parent(), key);
            });

            this->formVisualizationPaneItems[key]->addWidget(combo_box);
            this->formVisualizationPaneItems[key]->toggle();
        }

        this->formVisualizationPane->addWidget(this->formVisualizationPaneItems[key]);
    }

    this->formVisualizationPane->addWidget(this->paneItemButton);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewMatplotlib::gnomonViewMatplotlib(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewMatplotlibPrivate(this);
    d->q = this;

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d, 0, 0, 1, 1);
    layout->addWidget(d->pane(parent), 0, 1, 1, 1);

    connect(d->export_button, SIGNAL(iconClicked()), d, SLOT(exportToManager()));

    this->setObjectName("ViewMatplotlib");

    QFile file(":gnomon/matplotlib_figure.py");

    if (file.open(QIODevice::ReadOnly)) {
        int stat;
        QString matplotlib_script  = file.readAll();
        file.close();
        dtkScriptInterpreterPython::instance()->interpret(matplotlib_script, &stat);
    } else {
        qWarning() << "Can't open matplotlib figure script";
    }

    connect(this, &gnomonViewMatplotlib::formAdded, [=] (const QString& key) {
        d->configure((QWidget *)this->parent(), key);
    });

    connect(d->renderButton, &QPushButton::clicked, [=] () {
        for (const auto& key : d->formVisualization.keys()) {
            gnomonAbstractMatplotlibVisualization *v = d->formVisualization[key];
            if(v) {
                v->update();
            }
        }
    });

    this->setAcceptDrops(true);
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete d;
}

void gnomonViewMatplotlib::setForm(const QString& name, gnomonAbstractForm *form, gnomonAbstractMatplotlibVisualization *visualization)
{
    if (gnomonTree *tree = dynamic_cast<gnomonTree *>(form)) {
        d->forms["gnomonTree"] = tree;

        int stat;
        dtkScriptInterpreterPython::instance()->interpret("import gnomonMatplotlibVisualizationTree", &stat);
    
        qDebug()<<Q_FUNC_INFO<<gnomonWidgets::matplotlibVisualizationTree::pluginFactory().keys();
        QString key = gnomonWidgets::matplotlibVisualizationTree::pluginFactory().keys()[0];
    
        if ((!d->formVisualization.contains("gnomonTree"))||(!d->formVisualization["gnomonTree"]))
        {
            d->formVisualization["gnomonTree"] = gnomonWidgets::matplotlibVisualizationTree::pluginFactory().create(key);
            d->formVisualization["gnomonTree"]->setView(this);
        }
        gnomonAbstractMatplotlibVisualizationTree *formVisualizationTree = (gnomonAbstractMatplotlibVisualizationTree *)d->formVisualization["gnomonTree"];
        formVisualizationTree->setTree(tree);
        if (visualization) {
            formVisualizationTree->setParameters(visualization->parameters());
        }
        formVisualizationTree->update();

        emit formAdded("gnomonTree");
    } else if (gnomonDataFrame *dataFrame = dynamic_cast<gnomonDataFrame *>(form)) {
        d->forms["gnomonDataFrame"] = dataFrame;

        int stat;
        dtkScriptInterpreterPython::instance()->interpret("import gnomonMatplotlibVisualizationDataFrame", &stat);
    
        qDebug()<<Q_FUNC_INFO<<gnomonWidgets::matplotlibVisualizationDataFrame::pluginFactory().keys();
        QString key = gnomonWidgets::matplotlibVisualizationDataFrame::pluginFactory().keys()[0];
    
        if ((!d->formVisualization.contains("gnomonDataFrame"))||(!d->formVisualization["gnomonDataFrame"]))
        {
            d->formVisualization["gnomonDataFrame"] = gnomonWidgets::matplotlibVisualizationDataFrame::pluginFactory().create(key);
            d->formVisualization["gnomonDataFrame"]->setView(this);
        }
        gnomonAbstractMatplotlibVisualizationDataFrame *formVisualizationDataFrame = (gnomonAbstractMatplotlibVisualizationDataFrame *)d->formVisualization["gnomonDataFrame"];
        formVisualizationDataFrame->setDataFrame(dataFrame);
        if (visualization) {
            formVisualizationDataFrame->setParameters(visualization->parameters());
        }
        formVisualizationDataFrame->update();

        emit formAdded("gnomonDataFrame");
    }
}


void gnomonViewMatplotlib::addWidget(QWidget *widget)
{
    widget->setStyleSheet(gnomonStyleSheet());
    d->layout->addWidget(widget);
    this->resize(800,this->height());
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
        gnomonAbstractForm *form = gnomonFormManager::instance()->get(path.remove(":").toInt());
        this->setForm("formManager",form);

    } else {
        if (path.endsWith("xml"))  {
            if ((!d->formReaderCommand.contains("gnomonTree"))||(!d->formReaderCommand["gnomonTree"]))
                d->formReaderCommand["gnomonTree"] = new gnomonTreeReaderCommand("gnomonTreeReaderTreex");
            gnomonTreeReaderCommand *treeCommand = (gnomonTreeReaderCommand *) d->formReaderCommand["gnomonTree"];
            treeCommand->setPath(path.remove("file://"));
            treeCommand->redo();

            gnomonTree * tree = (gnomonTree *) treeCommand->tree()->clone();
            if (!tree) {
                qWarning() << Q_FUNC_INFO << "Resulting cell image is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonTree",tree);
        }
    }

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
