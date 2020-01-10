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

#include "gnomonWorkspacePythonAlgorithm.h"

#include <gnomonCore>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkScript>
#include <dtkFonts>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithmPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonAlgorithmPrivate
{
public:
    dtkWidgetsMenu *menu(dtkWidgetsWorkspace *);
    void configure(void);

public:
    dtkMacsWidget *editor = nullptr;

public:
    dtkWidgetsMenuBarContainer *dashboard;
    gnomonInterpreterJupyter *terminal;

public:
    gnomonViewForm *source;
    gnomonViewForm *target;

    gnomonViewFormPool *pool = nullptr;

    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

public:
    QFormLayout *layout = nullptr;
    QVBoxLayout *viewer_layout = nullptr;

public:
    QHash<QString, dtkCoreParameter *> parameters;

public:
    gnomonAbstractFormAlgorithm *algorithm = nullptr;
};

dtkWidgetsMenu *gnomonWorkspacePythonAlgorithmPrivate::menu(dtkWidgetsWorkspace *parent)
{
    qDebug()<<Q_FUNC_INFO<<"Set Algo";

    this->layout = new QFormLayout;

    QPushButton *open_button = new QPushButton("Open");
    open_button->setCheckable(true);

    QObject::connect(open_button, &QPushButton::clicked, [=] () {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

        QString file_path;
        file_path = QFileDialog::getOpenFileName(parent, "Load Python script", settings.value("Python/load", ".").toString(), "Python scripts (*.py)");

        QFile f(file_path);
        if(f.open(QIODevice::ReadOnly)) {
            QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
            settings.setValue("Python/load", file_path);

            QTextStream s(&f);
            this->editor->setText(s.readAll());

            this->configure();
        }
    });

    dtkWidgetsMenuItemDIY *menu_load = new dtkWidgetsMenuItemDIY("Python Script", open_button);
    menu_load->setShowTitle(false);
    menu_load->setSizePolicy(QSizePolicy::Expanding);

    QWidget *pane_item_parameters = new QWidget;
    pane_item_parameters->setLayout(this->layout);

    dtkWidgetsMenuItemDIY *menu_parameters = new dtkWidgetsMenuItemDIY("Parameters");
    menu_parameters->addWidget(pane_item_parameters);
    menu_parameters->setSizePolicy(QSizePolicy::Expanding);

    QPushButton *pane_item_button = new QPushButton("Apply");
    pane_item_button->setCheckable(true);

    dtkWidgetsMenuItemDIY *menu_button = new dtkWidgetsMenuItemDIY("Python Algorithm", pane_item_button);
    menu_button->setShowTitle(false);
    menu_button->setSizePolicy(QSizePolicy::Expanding);

    dtkWidgetsMenu *pane = new dtkWidgetsMenu(fa::circlethin, "Python Algorithm");
    pane->addItem(menu_load);
    pane->addItem(menu_parameters);
    pane->addItem(menu_button);

    QObject::connect(pane_item_button, &QPushButton::clicked, [=] () {
        dtkApp->window()->setCursor(Qt::BusyCursor);
        dynamic_cast<gnomonWorkspacePythonAlgorithm*>(parent)->run();
        dtkApp->window()->setCursor(Qt::ArrowCursor);
    });

    return pane;
}

void gnomonWorkspacePythonAlgorithmPrivate::configure(void)
{
    gnomonCore::formAlgorithm::pluginFactory().clear();
    qDebug()<<Q_FUNC_INFO<<gnomonCore::formAlgorithm::pluginFactory().keys();

    int stat;
    QString output = dtkScriptInterpreterPython::instance()->interpret(this->editor->toPlainText(), &stat);

    qDebug()<<Q_FUNC_INFO<<gnomonCore::formAlgorithm::pluginFactory().keys();

    QString key = gnomonCore::formAlgorithm::pluginFactory().keys()[0];
    qDebug()<<Q_FUNC_INFO<<key;

    if (this->layout) {

        for(int row = 0, max_row = this->layout->count(); row < max_row; ++row) {
            QLayoutItem *forDeletion = this->layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }
    } else {
        this->layout = new QFormLayout;
    }

    this->algorithm = gnomonCore::formAlgorithm::pluginFactory().create(key);
    Q_ASSERT(this->algorithm);

    if (this->algorithm) {

        QMap<QString, gnomonCoreParameter *> parameters = this->algorithm->parameters();

        for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
            QWidget *widget = gnomonWidgetsParameter::widget(it.value(), 0);
            if (widget)
                this->layout->addRow(it.key(), widget);
        }

        this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePythonAlgorithm::gnomonWorkspacePythonAlgorithm(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspacePythonAlgorithmPrivate;

    d->editor = new dtkMacsWidget(this);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();

    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
    d->dashboard->setFixedWidth(300);

    // -- Organizing the editor column --
    QVBoxLayout *editor_layout = new QVBoxLayout;
    editor_layout->setContentsMargins(0, 0, 0, 0);
    editor_layout->setSpacing(0);
    editor_layout->addWidget(d->editor);

    d->terminal = new gnomonInterpreterJupyter(this);
    //d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());
//    editor_layout->addWidget(d->terminal);

    QWidget *editor_widget = new QWidget(this);
    editor_widget->setLayout(editor_layout);
    editor_widget->resize(800, editor_widget->height());

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(this->color);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);
    d->pool->linkAll();

    // /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->setCurrentWidget(d->target_message);

    // -- Organizing the viewer column --
    d->viewer_layout = new QVBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    d->viewer_layout->addWidget(d->source);
    d->viewer_layout->addWidget(d->target_stack);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(d->viewer_layout);

//     // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
//    splitter->addWidget(finder);
    splitter->addWidget(editor_widget);
    splitter->addWidget(viewer);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->dashboard);
}

gnomonWorkspacePythonAlgorithm::~gnomonWorkspacePythonAlgorithm(void)
{
    delete d;
}


void gnomonWorkspacePythonAlgorithm::enter(void)
{
}

void gnomonWorkspacePythonAlgorithm::leave(void)
{
}

void gnomonWorkspacePythonAlgorithm::apply(void)
{
    d->configure();
}

void gnomonWorkspacePythonAlgorithm::run(void)
{
    Q_ASSERT(d->algorithm);

    if (d->source->cellComplex()) {
        d->algorithm->setInputCellComplex(d->source->cellComplex());
    }
    if (d->source->cellImage()) {
        d->algorithm->setInputCellImage(d->source->cellImage());
    }
    if (d->source->image()) {
        d->algorithm->setInputImage(d->source->image());
    }
    if (d->source->mesh()) {
        d->algorithm->setInputMesh(d->source->mesh());
    }
    if (d->source->pointCloud()) {
        d->algorithm->setInputPointCloud(d->source->pointCloud());
    }

    d->algorithm->run();

    d->target_stack->setCurrentWidget(d->target_message);
    d->source->setEnableLinking(false);
    d->target->setEnableLinking(false);


    gnomonCellComplexSeries *cellComplex = d->algorithm->outputCellComplex();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        qDebug()<<"No CellComplex!";
    } else {
        d->target->setForm("gnomonCellComplex",cellComplex);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonCellImageSeries *cellImage = d->algorithm->outputCellImage();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        qDebug()<<"No CellImage!";
    } else {
        d->target->setForm("gnomonCellImage",cellImage);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonImageSeries *image = d->algorithm->outputImage();
     if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        qDebug()<<"No Image!";
    } else {
        d->target->setForm("gnomonImage",image);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonMeshSeries *mesh = d->algorithm->outputMesh();
    if ((!mesh)||(mesh->times().size()==0)) {
        qDebug()<<"No Mesh!";
    } else {
        d->target->setForm("gnomonMesh",mesh);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonPointCloudSeries *pointCloud = d->algorithm->outputPointCloud();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        qDebug()<<"No PointCloud!";
    } else {
        d->target->setForm("gnomonPointCloud",pointCloud);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }
}


const QColor gnomonWorkspacePythonAlgorithm::color = QColor("#a38948");


//
// gnomonWorkspacePythonAlgorithm.cpp ends here
