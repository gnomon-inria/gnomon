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

#include "gnomonWorkspacePythonSimulator.h"

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
// gnomonCodeEditorToolBar
// ///////////////////////////////////////////////////////////////////

class gnomonCodeEditorToolBar : public QFrame
{
    Q_OBJECT

public:
     gnomonCodeEditorToolBar(QWidget *parent);
    ~gnomonCodeEditorToolBar(void);

public:
    void addAction(const QIcon &icon, const QObject *receiver, const QString& tool_tip, std::function<void(void)> function);
    void addStretch(void);

private:
    QHBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
// gnomonCodeEditorToolBar
// ///////////////////////////////////////////////////////////////////

gnomonCodeEditorToolBar::gnomonCodeEditorToolBar(QWidget *parent) : QFrame(parent)
{
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 0, 10, 0);
    this->layout->setAlignment(Qt::AlignRight);

    this->setFixedHeight(36);
}

gnomonCodeEditorToolBar::~gnomonCodeEditorToolBar(void)
{

}

void gnomonCodeEditorToolBar::addAction(const QIcon &icon, const QObject *receiver, const QString & tool_tip, std::function<void(void)> function)
{
    QToolButton *button = new QToolButton(this);
    button->setIconSize(QSize(16, 16));
    button->setIcon(icon);
    button->setToolTip(tool_tip);

    connect(button, &QToolButton::clicked, receiver, function);

    this->layout->addWidget(button);
}

void gnomonCodeEditorToolBar::addStretch(void)
{
    this->layout->addStretch();
}

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonSimulatorPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonSimulatorPrivate
{
public:
    dtkMacsWidget *editor = nullptr;
    dtkWidgetsMenu *menu_ubi_simulation = nullptr;
    dtkWidgetsMenu *menu_simulation = nullptr;
    dtkWidgetsMenu *menu_file = nullptr;

public:
    dtkWidgetsMenuBarContainer *dashboard;

    gnomonInterpreterJupyter *terminal;

public:
    gnomonViewForm *view;

public:
    QFormLayout *parameter_layout = nullptr;
    QVBoxLayout *viewer_layout = nullptr;

public:
    QHash<QString, dtkCoreParameter *> parameters;

public:
    gnomonAbstractEvolutionModel *model = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    d->editor = new dtkMacsWidget(this);


// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();

    dtkWidgetsParameterMenuBarGenerator menubar_generator(":pythonSimulator_menu.json", ":pythonSimulator_params.json");
    menubar_generator.populate(d->dashboard);

    d->parameters = menubar_generator.parameters();

    d->dashboard->setFixedWidth(300);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    d->parameters["python/load"]->setValue(settings.value("Python/load", ".").toString());
    d->parameters["python/load"]->connect([=] (QVariant v) {
        QString path = v.value<dtk::d_path>().path();
        if(path.isEmpty())
            return;

        //d->font_awesome->icon(fa::folderopen)
        QFile f(path);
        if(f.open(QIODevice::ReadOnly)) {
            QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
            settings.setValue("Python/load", path);

            QTextStream s(&f);
            d->editor->setText(s.readAll());
        } else {
            qInfo() << "couldn't load file " << path;
        }
     });

    d->parameters["python/save"]->connect([=] (QVariant v) {
        bool start = v.value<dtk::d_bool>().value();
        if (start) {
            QWidget::setCursor(Qt::BusyCursor);
            QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
            QString old_path = settings.value("Python/save", QDir::homePath()).toString();
            QString file_path = QFileDialog::getSaveFileName(nullptr,
                                                  tr("Save Python File"),
                                                  old_path,
                                                  tr("Python (*.py)"));

            //if the user press cancel, do nothing
            if(!file_path.isEmpty()) {
                QFile f(file_path);
                if(f.open(QIODevice::WriteOnly)) {
                    QTextStream out(&f);
                    out << d->editor->toPlainText();
                } else {
                    qWarning() << "couldn t save to file" << file_path;
                }
                settings.setValue("Python/save", file_path);
            }
            QWidget::setCursor(Qt::ArrowCursor);
            d->parameters["python/save"]->setValue(false);
        }

    });

    d->parameters["python/play"]->connect([=] (QVariant v) {
        bool start = v.value<dtk::d_bool>().value();
        if (start) {
            QWidget::setCursor(Qt::BusyCursor);
            this->apply();
            QWidget::setCursor(Qt::ArrowCursor);
            d->parameters["python/play"]->setValue(false);
        }

    });

    d->parameters["python/run"]->connect([=] (QVariant v) {
        bool start = v.value<dtk::d_bool>().value();
        if (start) {
            QWidget::setCursor(Qt::BusyCursor);
            this->run();
            QWidget::setCursor(Qt::ArrowCursor);
            d->parameters["python/run"]->setValue(false);
        }
    });

    d->parameters["python/step"]->connect([=] (QVariant v) {
        bool start = v.value<dtk::d_bool>().value();
        if (start) {
            QWidget::setCursor(Qt::BusyCursor);
            this->step();
            QWidget::setCursor(Qt::ArrowCursor);
            d->parameters["python/step"]->setValue(false);
        }
    });

    d->parameters["python/reset"]->connect([=] (QVariant v) {
        bool start = v.value<dtk::d_bool>().value();
        if (start) {
            QWidget::setCursor(Qt::BusyCursor);
            this->reset();
            QWidget::setCursor(Qt::ArrowCursor);
            d->parameters["python/reset"]->setValue(false);
        }
    });

    // -- Organizing the editor column --
    QVBoxLayout *editor_layout = new QVBoxLayout;
    editor_layout->setContentsMargins(0, 0, 0, 0);
    editor_layout->setSpacing(0);
    editor_layout->addWidget(d->editor);

    d->terminal = new gnomonInterpreterJupyter(this);
    //d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    editor_layout->addWidget(d->terminal);

    QWidget *editor_widget = new QWidget(this);
    editor_widget->setLayout(editor_layout);
    editor_widget->resize(800, editor_widget->height());

    d->view = new gnomonViewForm(this);

    // -- Organizing the viewer column --
    d->viewer_layout = new QVBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    d->viewer_layout->addWidget(d->view);

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

gnomonWorkspacePythonSimulator::~gnomonWorkspacePythonSimulator(void)
{
    delete d;
}


void gnomonWorkspacePythonSimulator::enter(void)
{
//    dtkApp->window()->menubar()->insertMenu(0, d->view->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePythonSimulator::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->view->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePythonSimulator::apply(void)
{
    //gnomonCore::evolutionModel::pluginFactory().clear();
    int stat;
    QString output = dtkScriptInterpreterPython::instance()->interpret(d->editor->toPlainText(), &stat);
    // qDebug()<< output;

    QString key = gnomonCore::evolutionModel::pluginFactory().keys()[0];

    qDebug()<<Q_FUNC_INFO<<key;

    if (d->model) {
        delete d->model;
        d->model = nullptr;
    }

    d->model = gnomonCore::evolutionModel::pluginFactory().create(key);

    if(d->view->mesh()){
        d->model->setForm("mesh", d->view->mesh());
    }

    if(d->view->cellComplex()){
        d->model->setForm("cellComplex", d->view->cellComplex());
    }

    d->model->reset();

    QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();
    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }
}

void gnomonWorkspacePythonSimulator::run(void)
{
    dtk::d_real initial_time = d->parameters["python/initial_time"];
    dtk::d_real final_time = d->parameters["python/final_time"];
    dtk::d_real dt = d->parameters["python/dt"];
    dtk::d_bool animate = d->parameters["python/animate"];


    // d->model->run(initial_time,final_time,dt);

    QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();

    double t = initial_time;

    while (t < final_time) {
        qDebug()<<"Step: "<<t;
        d->model->step(t,dt);
        t = t + dt;

        if (animate) {
            forms = d->model->forms();
            for (const auto& name : forms.keys())
            {
                d->view->setForm(name,forms[name]);
            }
            d->view->onTimeChanged(t);
            QCoreApplication::processEvents();
        }
    }

    if(!animate) {
        forms = d->model->forms();
        for (const auto& name : forms.keys())
        {
            d->view->setForm(name,forms[name]);
        }
        d->view->onTimeChanged(t);
    }
}

void gnomonWorkspacePythonSimulator::step(void)
{
    dtk::d_real initial_time = d->parameters["python/initial_time"];
    dtk::d_real final_time = d->parameters["python/final_time"];
    dtk::d_real dt = d->parameters["python/dt"];
    double t = initial_time;

    d->model->step(t,dt);
    t = t + dt;

    QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();


    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }
    d->view->onTimeChanged(t);
}

void gnomonWorkspacePythonSimulator::reset(void)
{
    d->model->reset();
    qDebug()<<"Reset model";
    QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();

    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }
}

const QColor gnomonWorkspacePythonSimulator::color = QColor("#a38948");

// ///////////////////////////////////////////////////////////////////

#include "gnomonWorkspacePythonSimulator.moc"

//
// gnomonWorkspacePythonSimulator.cpp ends here
