#include "gnomonMplView.h"
#include "gnomonAbstractView_p.h"

#include <dtkScript>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

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

public slots:
    void saveFigure(const QString& path);
    void clearFigure(void);
    void renderFigure(void);
    void updateFigureLimits(void);

public slots:
    void adaptForm(const QString& adapter_plugin);

public:
    gnomonMplView *q = nullptr;

public:
    int figureNumber;
    bool no_python;

    double x_min = 0;
    double x_max = 1;
    double y_min = 0;
    double y_max = 1;
    
public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    std::shared_ptr<gnomonAbstractDynamicForm> form_to_adapt = nullptr;

};

gnomonMplViewPrivate::gnomonMplViewPrivate(QObject *parent) : QObject(parent)
{
    this->figureNumber = -1;
}

gnomonMplViewPrivate::~gnomonMplViewPrivate(void)
{
}

void gnomonMplViewPrivate::saveFigure(const QString& path)
{
    /*QSettings settings("inria", "gnomon");
    settings.beginGroup("General");
    QString path = settings.value("last_saved_file", QDir::homePath()).toString();
    settings.endGroup();

    QString export_file_path;
    export_file_path = QFileDialog::getSaveFileName(this, tr("Save figure"), path, tr("Figures (*.png *.eps *.pdf *.svg)"));*/

    QString figure_number = QString::number(this->figureNumber);

    if (!this->no_python) {
        int stat;
        dtkScriptInterpreterPython::instance()->interpret("import matplotlib.pyplot as plt", &stat);
        QString figure_statement = "figure = plt.figure(" + figure_number + ")";
        dtkScriptInterpreterPython::instance()->interpret(figure_statement, &stat);
        dtkScriptInterpreterPython::instance()->interpret("s = figure.get_size_inches()", &stat);
        dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(10,10)", &stat);
        QString save_statement = "figure.savefig('" + path + "')";
        dtkScriptInterpreterPython::instance()->interpret(save_statement, &stat);
        dtkScriptInterpreterPython::instance()->interpret("figure.set_size_inches(*s)", &stat);
    }

}

void gnomonMplViewPrivate::clearFigure(void)
{
    if ((this->figureNumber != -1) && (!this->no_python)) {
        int stat;
        QString clearStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.clf()\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(clearStatement, &stat);
    }
}

void gnomonMplViewPrivate::renderFigure(void)
{
    if ((this->figureNumber != -1) && (!this->no_python)) {
        int stat;
        QString renderStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\nfigure = gnomon_figure(" + QString::number(this->figureNumber) + ")\nfigure.canvas.draw()";
        dtkScriptInterpreterPython::instance()->interpret(renderStatement, &stat);
    }
}

void gnomonMplViewPrivate::updateFigureLimits(void)
{
    if ((this->figureNumber != -1) && (!this->no_python)) {
        int stat;
        QString limitsStatement = "from gnomon.utils.matplotlib_tools import gnomon_figure\n";
        limitsStatement += "figure = gnomon_figure(" + QString::number(this->figureNumber) + ")\n";
        limitsStatement += "figure.gca().set_xlim(" + QString::number(this->x_min) + ", " + QString::number(this->x_max) + ")\n";
        limitsStatement += "figure.gca().set_ylim(" + QString::number(this->y_min) + ", " + QString::number(this->y_max) + ")\n";
        dtkScriptInterpreterPython::instance()->interpret(limitsStatement, &stat);
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
}

// ///////////////////////////////////////////////////////////////////
// gnomonMplView
// ///////////////////////////////////////////////////////////////////

gnomonMplView::gnomonMplView(QObject *parent, bool no_python) : gnomonAbstractView(parent)
{
    setObjectName("gnomonMplView");
    dd = new gnomonMplViewPrivate(this);
    dd->q = this;

    dd->no_python = no_python;

    d->acceptForms["gnomonDataFrame"] = false;
    d->acceptForms["gnomonLString"] = false;
    d->acceptForms["gnomonTree"] = false;

    if (!dd->no_python) {
        int stat;
        dtkScriptInterpreterPython::instance()->interpret("import gnomon.utils.matplotlib_tools", &stat);
    }

    connect(this, &gnomonMplView::formAdded, [=] (const QString& key) {
        this->render();
        emit formsChanged();
    });
}

gnomonMplView::~gnomonMplView(void)
{
    delete dd;
}

void gnomonMplView::setAcceptForm(const QString& form_type, bool accept)
{
    if(!d->acceptForms.contains(form_type)) { // Form not supported by VtkView
        return;
    }
    d->acceptForms[form_type] = accept;

    if(!accept) {
        return;
    }

    if(form_type == "gnomonDataFrame") {
        d->visualizationCommands["gnomonDataFrame"] = std::make_shared<gnomonDataFrameMplVisualizationCommand>();
    } else if(form_type == "gnomonLString") {
        d->visualizationCommands["gnomonLString"] = std::make_shared<gnomonLStringMplVisualizationCommand>();
    } else if(form_type == "gnomonTree") {
        d->visualizationCommands["gnomonTree"] = std::make_shared<gnomonTreeMplVisualizationCommand>();
    }

    d->visualizationCommands[form_type]->setView(this);
    connect(d->visualizationCommands[form_type].get(), &gnomonAbstractVisualizationCommand::visuParametersChanged, [=] () {
        emit formVisuParametersChanged();
    });

    if (form_type=="gnomonLString") {
        loadPluginGroup("lStringAdapter");
        for (const auto& key : gnomonCore::lStringAdapter::pluginFactory().keys())
        {
            gnomonAbstractLStringAdapter *adapter = dynamic_cast<gnomonAbstractLStringAdapter *>(gnomonCore::lStringAdapter::pluginFactory().create(key));
            if (!dd->adapterCommands.contains(form_type))
            {
                QMap<QString, QString> empty_target;
                dd->adapterTargets[form_type] = empty_target;
                QMap<QString, QString> empty_desc;
                dd->adapterDescriptions[form_type] = empty_desc;
                QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                dd->adapterCommands[form_type] = empty_list;
            }
            dd->adapterTargets[form_type][key] = adapter->target();
            dd->adapterDescriptions[form_type][key] = adapter->documentation().split("\n")[1];
            dd->adapterCommands[form_type][key] = new gnomonLStringAdapterCommand;
            dd->adapterCommands[form_type][key]->setAlgorithmName(key);
            delete adapter;
        }
    } else if (form_type=="gnomonTree") {
        loadPluginGroup("treeAdapter");
        for (const auto& key : gnomonCore::treeAdapter::pluginFactory().keys())
        {
            gnomonAbstractTreeAdapter *adapter = dynamic_cast<gnomonAbstractTreeAdapter *>(gnomonCore::treeAdapter::pluginFactory().create(key));
            if (!dd->adapterCommands.contains(form_type))
            {
                QMap<QString, QString> empty_target;
                dd->adapterTargets[form_type] = empty_target;
                QMap<QString, QString> empty_desc;
                dd->adapterDescriptions[form_type] = empty_desc;
                QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                dd->adapterCommands[form_type] = empty_list;
            }
            dd->adapterTargets[form_type][key] = adapter->target();
            dd->adapterDescriptions[form_type][key] = adapter->documentation().split("\n")[1];
            dd->adapterCommands[form_type][key] = new gnomonTreeAdapterCommand;
            dd->adapterCommands[form_type][key]->setAlgorithmName(key);
            delete adapter;
        }
    }
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
void gnomonMplView::render(void)
{
    dd->renderFigure();
}

void gnomonMplView::clear(void)
{
    gnomonAbstractView::clear();
    dd->clearFigure();
}

void gnomonMplView::saveScreenshot(const QString& filename)
{
    QString file_path;
    const QUrl url(filename);
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = filename;
    }

    if ((dd->figureNumber != -1) && (!dd->no_python)) {
        int stat;
        QString screenshotStatement = "";
        screenshotStatement += "from gnomon.utils.matplotlib_tools import gnomon_figure\n";
        screenshotStatement += "figure = gnomon_figure(" + QString::number(dd->figureNumber) + ")\n";
        // TODO: allow user to pass a size (through a dialog)
        // screenshotStatement += "figure.set_size_inches(10, 10)\n";
        screenshotStatement += "figure.savefig('" + file_path + "')";
        dtkScriptInterpreterPython::instance()->interpret(screenshotStatement, &stat);
    }
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

void gnomonMplView::setXMin(double x_min)
{
    if (x_min != dd->x_min) {
        dd->x_min = x_min;
        dd->updateFigureLimits();
        emit limitsChanged();
    }
}

void gnomonMplView::setXMax(double x_max)
{
    if (x_max != dd->x_max) {
        dd->x_max = x_max;
        dd->updateFigureLimits();
        emit limitsChanged();
    }
}

void gnomonMplView::setYMin(double y_min)
{
    if (y_min != dd->y_min) {
        dd->y_min = y_min;
        dd->updateFigureLimits();
        emit limitsChanged();
    }
}

void gnomonMplView::setYMax(double y_max)
{
    if (y_max != dd->y_max) {
        dd->y_max = y_max;
        dd->updateFigureLimits();
        emit limitsChanged();
    }
}

void gnomonMplView::updateLimits(void)
{
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject* pModule_mpl_tools = PyImport_Import(PyUnicode_FromString("gnomon.utils.matplotlib_tools"));
    if(!pModule_mpl_tools) {
        dtkWarn() << Q_FUNC_INFO << "Error importing module gnomon.utils.matplotlib_tools";
        PyGILState_Release(gstate);
        return;
    }

    PyObject* pFigure = PyObject_CallMethodOneArg(pModule_mpl_tools, PyUnicode_FromString("gnomon_figure"), PyLong_FromLong(dd->figureNumber));
    if(!pFigure) {
        dtkWarn() << Q_FUNC_INFO << "Error calling function gnomon_figure("+QString::number(dd->figureNumber)+")";
        PyGILState_Release(gstate);
        return;
    }

    PyObject* pAxes = PyObject_CallMethodNoArgs(pFigure, PyUnicode_FromString("gca"));
    PyObject* pXlim = PyObject_CallMethodNoArgs(pAxes, PyUnicode_FromString("get_xlim"));
    PyObject* pYlim = PyObject_CallMethodNoArgs(pAxes, PyUnicode_FromString("get_ylim"));

    double x_min = PyFloat_AsDouble(PyTuple_GetItem(pXlim, 0));
    double x_max = PyFloat_AsDouble(PyTuple_GetItem(pXlim, 1));
    double y_min = PyFloat_AsDouble(PyTuple_GetItem(pYlim, 0));
    double y_max = PyFloat_AsDouble(PyTuple_GetItem(pYlim, 1));
    
    bool limits_changed = (x_min != dd->x_min) || (x_max != dd->x_max) || (y_min != dd->y_min) || (y_max != dd->y_max);

    dd->x_min = x_min;
    dd->x_max = x_max;
    dd->y_min = y_min;
    dd->y_max = y_max;

    if (limits_changed) {
        emit this->limitsChanged();
    }

    Py_DECREF(pXlim);
    Py_DECREF(pYlim);
    Py_DECREF(pAxes);
    Py_DECREF(pFigure);

    PyGILState_Release(gstate);
}

double gnomonMplView::xMin(void)
{
    return dd->x_min;    
}

double gnomonMplView::xMax(void)
{
    return dd->x_max;
}

double gnomonMplView::yMin(void)
{
    return dd->y_min;
}

double gnomonMplView::yMax(void)
{
    return dd->y_max;
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonMplView.moc"

//
// gnomonMplView.cpp ends here
