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

#include "gnomonComposerWidget.h"
#include "gnomonComposerWidget_p.h"

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerNode.h>
#include <dtkComposer/dtkComposerWidget.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerControls.h>
#include <dtkComposer/dtkComposerEvaluator.h>
#include <dtkComposer/dtkComposerEvaluatorToolBar.h>
#include <dtkComposer/dtkComposerNodeFactory.h>
#include <dtkComposer/dtkComposerNodeFactoryView.h>
#include <dtkComposer/dtkComposerGraph.h>
#include <dtkComposer/dtkComposerGraphView.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>
#include <dtkComposer/dtkComposerViewManager.h>
#include <dtkComposer/dtkComposerViewController.h>

#include <dtkCore/dtkCore.h>
#include <dtkCore/dtkCorePluginManager.h>

#include <dtkLog/dtkLog.h>

#include <dtkWidgets/dtkWidgetsLogView.h>
#include <dtkWidgets/dtkNotification.h>
#include <dtkWidgets/dtkNotificationDisplay.h>
#include <dtkWidgets/dtkScreenMenu.h>
#include <dtkWidgets/dtkRecentFilesMenu.h>
#include <dtkWidgets/dtkSpacer.h>
#include <dtkWidgets/dtkSplitter.h>

#include <QtCore>
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// gnomonComposerWidgetPrivate
// /////////////////////////////////////////////////////////////////

bool gnomonComposerWidgetPrivate::maySave(void)
{
    if(this->closing)
        return true;

    if (q->isWindowModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(q,
            q->tr("VisualProgramming"),
            q->tr("The composition has been modified.\n Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        if (ret == QMessageBox::Save)
            return q->compositionSave();
        else
            if(ret == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void gnomonComposerWidgetPrivate::setCurrentFile(const QString &file)
{
     this->current_composition = file;

     q->setWindowModified(false);

     QString shownName = this->current_composition;

     if (shownName.isEmpty())
         shownName = "untitled.dtk";

     q->setWindowFilePath(shownName);
}

void gnomonComposerWidgetPrivate::setModified(bool modified)
{
    q->setWindowModified(modified);
}

// /////////////////////////////////////////////////////////////////
// gnomonComposerWidget
// /////////////////////////////////////////////////////////////////

gnomonComposerWidget::gnomonComposerWidget(QWidget *parent) : QFrame(parent)
{
    d = new gnomonComposerWidgetPrivate;
    d->q = this;

    // -- Elements

    d->composer = new dtkComposerWidget;

    d->controls = nullptr;

    d->editor = new dtkComposerSceneNodeEditor(this);
    d->editor->setScene(d->composer->scene());
    d->editor->setStack(d->composer->stack());
    d->editor->setGraph(d->composer->graph());

    d->model = new dtkComposerSceneModel(this);
    d->model->setScene(d->composer->scene());

    d->scene = new dtkComposerSceneView(this);
    d->scene->setScene(d->composer->scene());
    d->scene->setModel(d->model);

    d->stack = new dtkComposerStackView(this);
    d->stack->setStack(d->composer->stack());

    d->nodes = new dtkComposerNodeFactoryView(this);
    d->nodes->setFactory(d->composer->factory());

    d->graph = new dtkComposerGraphView(this);
    d->graph->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    d->graph->setGraph (d->composer->graph());
    d->graph->setVisible(false);

    d->log_view = new dtkWidgetsLogView(this);
    d->log_view->setVisible(false);

    d->view_manager = new dtkComposerViewManager;
    d->view_manager->setVisible(false);

    connect(d->composer->scene(), SIGNAL(flagged(dtkComposerSceneNode *)), this, SLOT(onComposerNodeFlagged(dtkComposerSceneNode *)));

    d->closing = false;

    // -- Actions

    d->composition_open_action = new QAction("Open", this);
    d->composition_open_action->setShortcut(QKeySequence::Open);

    d->composition_save_action = new QAction("Save", this);
    d->composition_save_action->setShortcut(QKeySequence::Save);

    d->composition_saveas_action = new QAction("Save As...", this);
    d->composition_saveas_action->setShortcut(QKeySequence::SaveAs);

    d->composition_insert_action = new QAction("Insert", this);
    d->composition_insert_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_I);

    d->undo_action = d->composer->stack()->createUndoAction(this);
    d->undo_action->setShortcut(QKeySequence::Undo);

    d->redo_action = d->composer->stack()->createRedoAction(this);
    d->redo_action->setShortcut(QKeySequence::Redo);

    // -- Menus

    QMenuBar *menu_bar = new QMenuBar();

    d->recent_compositions_menu = new dtkRecentFilesMenu("Open recent...", this);

    d->composition_menu = menu_bar->addMenu("Composition");
    d->composition_menu->addAction(d->composition_open_action);
    d->composition_menu->addMenu(d->recent_compositions_menu);
    d->composition_menu->addAction(d->composition_save_action);
    d->composition_menu->addAction(d->composition_saveas_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_insert_action);

    d->edit_menu = menu_bar->addMenu("Edit");
    d->edit_menu->addAction(d->composer->view()->searchAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->undo_action);
    d->edit_menu->addAction(d->redo_action);
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->flagAsBlueAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGrayAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGreenAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsOrangeAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsPinkAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsRedAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsYellowAction());
    d->edit_menu->addAction(d->composer->scene()->setBreakPointAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->maskEdgesAction());
    d->edit_menu->addAction(d->composer->scene()->unmaskEdgesAction());

    dtkScreenMenu *screen_menu = new dtkScreenMenu("Screen",this);
    menu_bar->addMenu(screen_menu);

    QAction *showControlsAction = new QAction("Show controls", this);
    showControlsAction->setShortcut(QKeySequence(Qt::ShiftModifier + Qt::ControlModifier + Qt::AltModifier + Qt::Key_C));

    QMenu *window_menu = menu_bar->addMenu("Window");
    window_menu->addAction(showControlsAction);

    // -- Connections

    connect(showControlsAction, SIGNAL(triggered()), this, SLOT(showControls()));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));

    connect(d->composer, SIGNAL(modified(bool)), d, SLOT(setModified(bool)));

    connect(d->composition_open_action, SIGNAL(triggered()), this, SLOT(compositionOpen()));
    connect(d->composition_save_action, SIGNAL(triggered()), this, SLOT(compositionSave()));
    connect(d->composition_saveas_action, SIGNAL(triggered()), this, SLOT(compositionSaveAs()));
    connect(d->composition_insert_action, SIGNAL(triggered()), this, SLOT(compositionInsert()));

    connect(d->recent_compositions_menu, SIGNAL(recentFileTriggered(const QString&)), this, SLOT(compositionOpen(const QString&)));

    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    left->addWidget(d->nodes);
    left->addWidget(d->view_manager);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    right->addWidget(d->scene);
    right->addWidget(d->editor);
    right->addWidget(d->stack);
    right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);

    d->inner = new dtkSplitter(this);
    d->inner->setOrientation(Qt::Horizontal);
    d->inner->addWidget(left);
    d->inner->addWidget(d->graph);
    d->inner->addWidget(d->composer);
    d->inner->addWidget(right);

    QHBoxLayout *b_layout = new QHBoxLayout;
    b_layout->setContentsMargins(0, 0, 0, 0);
    b_layout->setSpacing(0);
    b_layout->addWidget(d->log_view);

    QWidget *bottom = new QWidget(this);
    bottom->setLayout(b_layout);

    dtkSplitter *central = new dtkSplitter(this);
    central->setOrientation(Qt::Vertical);
    central->addWidget(d->inner);
    central->addWidget(bottom);

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(menu_bar);
    main_layout->addWidget(central);

    this->setLayout(main_layout);

    d->setCurrentFile("");

    // Set up composition workspace.

    d->composer->setVisible(true);
    d->composer->compass()->setVisible(true);
    d->nodes->setVisible(true);
    d->scene->setVisible(true);
    d->editor->setVisible(true);
    d->stack->setVisible(false);
    d->view_manager->setVisible(false);

    d->graph->setVisible(false);
    d->log_view->setVisible(false);

    int wl = qMin(d->nodes->size().width(), 300);
    int wr = qMax(d->stack->size().width(), 300);
    int wc = parent->size().width() - wl - wr;

    d->inner->setSizes(QList<int>() << wl << 0 << wc << wr);
}

gnomonComposerWidget::~gnomonComposerWidget(void)
{
    delete d;
}

bool gnomonComposerWidget::compositionOpen(void)
{
    if(!d->maySave())
        return true;

    QFileDialog *dialog = new QFileDialog(this, tr("Open composition"), QString(), QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionOpen(const QString&)));

    return true;
}

bool gnomonComposerWidget::compositionOpen(const QString& file)
{
    if(sender() == d->recent_compositions_menu && !d->maySave())
        return true;

    bool status = d->composer->open(file);

    if(status) {
        d->recent_compositions_menu->addRecentFile(file);
        d->setCurrentFile(file);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Opened %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool gnomonComposerWidget::compositionSave(void)
{
    bool status;

    if(d->current_composition.isEmpty() || d->current_composition == "untitled.dtk")
        status = this->compositionSaveAs();
    else
        status = d->composer->save();

    if(status)
        this->setWindowModified(false);

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved %1</div>").arg(d->current_composition), 3000);

    return status;
}

bool gnomonComposerWidget::compositionSaveAs(void)
{
    bool status = false;

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QStringList nameFilters;
    nameFilters <<  "Ascii composition (*.dtk)";
    nameFilters << "Binary composition (*.dtk)";

    QFileDialog dialog(this, "Save composition as ...", path, QString("dtk composition (*.dtk)"));
    dialog.setStyleSheet("background-color: none ; color: none;");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("dtk");

    if(dialog.exec()) {

        if(dialog.selectedNameFilter() == nameFilters.at(0))
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Ascii);
        else
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Binary);
    }

    return status;
}

bool gnomonComposerWidget::compositionSaveAs(const QString& file, dtkComposerWriter::Type type)
{
    bool status = false;

    if(file.isEmpty())
        return status;

    status = d->composer->save(file, type);

    if(status) {
        d->setCurrentFile(file);
        this->setWindowModified(false);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved as %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool gnomonComposerWidget::compositionInsert(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionInsert(const QString&)));

    return true;
}

bool gnomonComposerWidget::compositionInsert(const QString& file)
{
    bool status = d->composer->insert(file);

    if(status)
        this->setWindowModified(true);

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    return status;
}

void gnomonComposerWidget::showControls(void)
{
    if(!d->controls) {
        d->controls = new dtkComposerControls(this);
        d->controls->setScene(d->composer->scene());
        d->controls->setWindowFlags(Qt::Dialog);
        d->controls->setWindowTitle("Composer Controls");

        if(!this->isFullScreen()) {
            d->controls->resize(d->controls->size().width(), this->size().height());
            d->controls->move(this->rect().topRight() + QPoint(10, 0));
        }
    }

    d->controls->show();
}

void gnomonComposerWidget::closeEvent(QCloseEvent *event)
{
    if (d->maySave()) {
         d->closing = true;
         event->accept();
     } else {
         event->ignore();
     }
}

void gnomonComposerWidget::onComposerNodeFlagged(dtkComposerSceneNode *node)
{
    dtkComposerViewController::instance()->insert(node);
}

dtkComposerWidget* gnomonComposerWidget::composerWidget()
{
    return d->composer;
}

//
// gnomonComposerWidget.cpp ends here
