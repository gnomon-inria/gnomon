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
    QMap<QString, gnomonAbstractVisualization *> formVisualization;
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
    qDebug()<<Q_FUNC_INFO<<"Not implemented yet!";
//    for (const auto& key : this->forms.keys()) {
//        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->formVisualization[key]);
//    }
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

void gnomonViewMatplotlibPrivate::refresh(void)
{
    this->formVisualizationPane->clearLayout();
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

    this->setAcceptDrops(true);
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete d;
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
    qDebug()<<Q_FUNC_INFO<<path;

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewMatplotlib.moc"

//
// gnomonViewMatplotlib.cpp ends here
