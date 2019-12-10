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

#include "gnomonWorkspaceBrowser.h"

#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFinderListView : public QListView
{
    Q_OBJECT

public:
     gnomonFinderListView(QWidget *parent = 0);
    ~gnomonFinderListView(void);

signals:
    void changed(const QString& path);

protected:
     void keyPressEvent(QKeyEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void startDrag(Qt::DropActions supportedActions);

private:
    class gnomonFinderListViewPrivate *d;
};

class gnomonFinderListViewPrivate
{
public:
    QMenu *menu;
    QList<QAction *> defaultActions;
    QList<QAction *> customActions;
};

gnomonFinderListView::gnomonFinderListView(QWidget *parent) : QListView(parent), d(new gnomonFinderListViewPrivate)
{
    this->setViewMode(QListView::IconMode);
    this->setWordWrap(true);
    this->setWrapping(true);
    // this->setIconSize(QSize(128, 128));
    this->setResizeMode(QListView::Adjust);
    this->setGridSize(QSize(64, 64));
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(this, &gnomonFinderListView::changed, [=] (const QString& path) -> void
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", path);
    });
}

gnomonFinderListView::~gnomonFinderListView(void)
{
    delete d;

    d = nullptr;
}

void gnomonFinderListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
            QDir dir = QDir(model->filePath(this->rootIndex()));
            dir.cdUp();
            this->setRootIndex(model->index(dir.absolutePath()));
            emit changed(dir.absolutePath());
            event->accept();
            return;
        }
    }

    if(event->key() == Qt::Key_Down &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

            if(!this->selectionModel()->selectedIndexes().count())
                return;

            QFileInfo selection = model->fileInfo(this->selectionModel()->selectedIndexes().first());

            if(selection.isDir()) {
                this->setRootIndex(model->index(selection.filePath()));
                emit changed(selection.absoluteFilePath());
                event->accept();
            }

            return;
        }
    }

    QListView::keyPressEvent(event);
}

void gnomonFinderListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        Q_UNUSED(model);

        QModelIndex index = indexAt(event->pos());

        if(!index.isValid())
            return;

        QDir dir = QDir(model->filePath(index));

        emit changed(dir.absolutePath());

        this->setRootIndex(index);

        QListView::mouseDoubleClickEvent(event);
    }
}

void gnomonFinderListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();

    if(indexes.count() > 0) {

        QMimeData *data = model()->mimeData(indexes);
        if(!data)
            return;

        QFileIconProvider provider;

        QPixmap pixmap = provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
        drag->exec(supportedActions, Qt::IgnoreAction);
    }
}

// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceBrowserPrivate
{
public:
    gnomonViewForm *browse_view;
};

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;

    d->browse_view = new gnomonViewForm(this);
    d->browse_view->setExportColor(gnomonToolBar::browser_color);

// /////////////////////////////////////////////////////////////////////////////

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

    QFileSystemModel *model = new QFileSystemModel(this);

    gnomonFinderListView *browser = new gnomonFinderListView(this);
    browser->setModel(model);
    browser->setFixedWidth(300);
    browser->setRootIndex(model->setRootPath(settings.value("path").toString()));

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->browse_view);
    layout->addWidget(browser);
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

void gnomonWorkspaceBrowser::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceBrowser::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceBrowser::apply(void)
{

}

// /////////////////////////////////////////////////////////////////////////////

#include "gnomonWorkspaceBrowser.moc"

//
// gnomonWorkspaceBrowser.cpp ends here
