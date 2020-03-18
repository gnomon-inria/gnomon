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

#include "gnomonComposerNodeWorkspace.h"
#include "gnomonComposerWidget.h"
#include "gnomonComposerWidget_p.h"

#include "gnomonPipeline.h"

#include "gnomonPipelineNode.h"

#include <dtkComposer>
#include <dtkCore>
#include <dtkLog>
#include <dtkWidgets>
#include <dtkThemes>

#include <QtCore>
#include <QtWidgets>

#include <gnomonWidgets>

// /////////////////////////////////////////////////////////////////
// gnomonComposerWidgetPrivate
// /////////////////////////////////////////////////////////////////

bool gnomonComposerWidgetPrivate::maySave(void)
{
    if(this->closing)
        return true;

//    if (q->isWindowModified()) {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("gnomon");
//        msgBox.setText("The composition has been modified.");
//        msgBox.setInformativeText("Do you want to save your changes?");
//        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//        msgBox.setDefaultButton(QMessageBox::Save);
//        msgBox.setStyleSheet("");
//        int ret = msgBox.exec();
//
//        if (ret == QMessageBox::Save)
//            return q->compositionSave();
//        else
//            if(ret == QMessageBox::Cancel)
//                return false;
//    }

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

gnomonComposerWidget *gnomonComposerWidget::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonComposerWidget;

    return s_instance;
}

QSize gnomonComposerWidget::sizeHint(void) const
{
    return QSize(200, 10);
}


void gnomonComposerWidget::resizeEvent(QResizeEvent *event)
{
    d->save_button->move(event->size().width() -40, 30);
    d->layout_button->move(event->size().width() -80, 30);

    QWidget::resizeEvent(event);
}

gnomonComposerWidget::gnomonComposerWidget(QWidget *parent) : QFrame(parent)
{
    d = new gnomonComposerWidgetPrivate;
    d->q = this;

    // -- Elements

    d->composer = new dtkComposerWidget;
    d->composer->view()->setBackgroundBrush(QColor(dtkThemesEngine::instance()->color("@bg")));

    d->closing = false;

    // -- Connections

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));

    connect(d->composer, SIGNAL(modified(bool)), d, SLOT(setModified(bool)));

    // -- Layout

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(d->composer);

    d->save_button = new gnomonOverlayButton(fa::save, "", d->composer);
    connect(d->save_button, SIGNAL(iconClicked()), this, SLOT(pipelineSave()));

    d->layout_button = new gnomonOverlayButton(fa::random, "", d->composer);
    connect(d->layout_button, &gnomonOverlayButton::iconClicked, [=] () {
        gnomonPipeline::instance()->updateLayout();
    });

    this->setLayout(main_layout);

    d->setCurrentFile("");

    // ---

    d->composer->setVisible(true);

    // --

    this->setMouseTracking(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // --

//    this->addWorkspace("Browser");
}

gnomonComposerWidget::~gnomonComposerWidget(void)
{
    delete d;
}

void gnomonComposerWidget::addWorkspace(const QString& title)
{
    qDebug() << Q_FUNC_INFO << title;

    gnomonComposerNodeWorkspace *node = new gnomonComposerNodeWorkspace(title);

    dtkComposerScenePort *i_port = 0;

    if (d->last_node) {
        i_port = new dtkComposerScenePort(dtkComposerScenePort::Input, node);

        node->addInputPort(i_port);

        node->setPos(d->last_node->pos() + QPointF(300, 0));
    }

    dtkComposerScenePort *port = new dtkComposerScenePort(dtkComposerScenePort::Output, node);

    node->addOutputPort(port);
    node->layout();
   
    dtkComposerSceneEdge *edge = 0;

    if (d->last_port) {
        qDebug() << Q_FUNC_INFO << "Adding edge";
        edge = new dtkComposerSceneEdge;
        edge->setSource(d->last_port);
        edge->setDestination(i_port);
        edge->link(true);

        d->last_node->addOutputEdge(edge);
    }


    d->composer->scene()->addItem(node);

    if (edge) {
        qDebug() << Q_FUNC_INFO << "Creating edge";

        d->composer->scene()->root()->addEdge(edge);
        d->composer->scene()->addItem(edge);
    }

    d->last_node = node;
    d->last_port = port;
}

void gnomonComposerWidget::addNode(gnomonPipelineNode *node)
{
//    if (d->last_node) {
//        node->setPos(d->last_node->pos() + QPointF(300, 0));
//    }
    node->layout();

    d->composer->scene()->addItem(node);

    for (const auto& edge : node->inputEdges()) {
//        d->composer->scene()->root()->addEdge(edge);
        d->composer->scene()->addItem(edge);
    }

    d->last_node = node;
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
    dialog.setStyleSheet("");
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

    return status;
}

bool gnomonComposerWidget::compositionInsert(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("");
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

bool gnomonComposerWidget::pipelineSave(void)
{
     bool status = false;

    QSettings settings("inria", "dtk");
    settings.beginGroup("General");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog dialog(this, "Save pipeline", path, QString("Python script (*.py)"));
    dialog.setStyleSheet("");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
//    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDefaultSuffix("py");

    if(dialog.exec()) {
        gnomonPipeline::instance()->exportToLuigiScript(dialog.selectedFiles().first());
        status = true;
    }

    return status;
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

dtkComposerWidget *gnomonComposerWidget::composerWidget(void)
{
    return d->composer;
}

void gnomonComposerWidget::enterEvent(QEvent *)
{
    d->inside = true;

    this->repaint();
}

void gnomonComposerWidget::leaveEvent(QEvent *)
{
    d->inside = false;

    this->repaint();
}

void gnomonComposerWidget::mousePressEvent(QMouseEvent *event)
{
    QRect handle = QRect(this->size().width() / 2 - 100, 0, 200, 14);

    if (handle.contains(event->pos())) {

        if(this->size().height() < 150) {
            emit expand();
            d->state = gnomonComposerWidgetPrivate::Expanded;
        } else {
            emit shrink();
            d->state = gnomonComposerWidgetPrivate::Collapsed;
        }
    }
}

void gnomonComposerWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setPen(dtkThemesEngine::instance()->color("@bgalt"));
    painter.drawLine(event->rect().topLeft(), event->rect().topRight());
    painter.drawLine(event->rect().bottomLeft(), event->rect().bottomRight());

    if(!d->inside)
        return;

    painter.setBrush(Qt::red);
    painter.drawRoundedRect(event->rect().width() / 2 - 100, 6, 200, 6, 3, 3);
}

gnomonComposerWidget *gnomonComposerWidget::s_instance = nullptr;

//
// gnomonComposerWidget.cpp ends here
