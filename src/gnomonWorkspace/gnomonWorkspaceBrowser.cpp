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
    void opened(const QString& path);

protected:
     void keyPressEvent(QKeyEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void startDrag(Qt::DropActions supportedActions);
};

// /////////////////////////////////////////////////////////////////////////////

gnomonFinderListView::gnomonFinderListView(QWidget *parent) : QListView(parent)
{
    this->setViewMode(QListView::IconMode);
    // this->setWordWrap(true);
    // this->setWrapping(true);
    this->setResizeMode(QListView::Adjust);
    this->setGridSize(QSize(64, 64));
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
}

gnomonFinderListView::~gnomonFinderListView(void)
{

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

        QModelIndex index = indexAt(event->pos());

        if(!index.isValid())
            return;

        QFileInfo info(model->filePath(index));

        if(info.isDir()) {

            QDir dir = QDir(model->filePath(index));

            emit changed(dir.absolutePath());

            this->setRootIndex(index);

        } else {

            emit opened(model->filePath(index));
        }
    }

    QListView::mouseDoubleClickEvent(event);
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
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFinderTreeView : public QTreeView
{
    Q_OBJECT

public:
     gnomonFinderTreeView(QWidget *parent = 0);
    ~gnomonFinderTreeView(void);

signals:
    void changed(const QString& path);
    void opened(const QString& path);
       
protected:
     void keyPressEvent(QKeyEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void startDrag(Qt::DropActions supportedActions);
};

// /////////////////////////////////////////////////////////////////////////////

gnomonFinderTreeView::gnomonFinderTreeView(QWidget *parent) : QTreeView(parent)
{
    this->setDragEnabled(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    this->header()->setSectionResizeMode(QHeaderView::Stretch);
}

gnomonFinderTreeView::~gnomonFinderTreeView(void)
{

}

void gnomonFinderTreeView::keyPressEvent(QKeyEvent *event)
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

    QTreeView::keyPressEvent(event);
}

void gnomonFinderTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        QModelIndex index = indexAt(event->pos());

        if(!index.isValid())
            return;

        QFileInfo info(model->filePath(index));

        if(info.isDir()) {

            QDir dir = QDir(model->filePath(index));

            emit changed(dir.absolutePath());

            this->setRootIndex(index);

        } else {

            emit opened(model->filePath(index));
        }
    }

    QTreeView::mouseDoubleClickEvent(event);
}

void gnomonFinderTreeView::startDrag(Qt::DropActions supportedActions)
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

public:
    QSplitter *splitter;
};

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;

    d->browse_view = new gnomonViewForm(this);
    d->browse_view->setExportColor(gnomonToolBar::browser_color);

// /////////////////////////////////////////////////////////////////////////////

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

    QLineEdit *path = new QLineEdit(settings.value("path").toString(), this);

    QToolButton *up = new QToolButton(this); up->setIcon(dtkFontAwesome::instance()->icon(fa::arrowup));
    QToolButton *ls = new QToolButton(this); ls->setIcon(dtkFontAwesome::instance()->icon(fa::th));
    QToolButton *tr = new QToolButton(this); tr->setIcon(dtkFontAwesome::instance()->icon(fa::list));

    QHBoxLayout *t_layout = new QHBoxLayout;
    t_layout->addWidget(up);
    t_layout->addWidget(ls);
    t_layout->addWidget(tr);
    t_layout->addWidget(path);

    QFileSystemModel *model = new QFileSystemModel(this);

    gnomonFinderListView *l_browser = new gnomonFinderListView(this);
    l_browser->setModel(model);
    l_browser->setRootIndex(model->setRootPath(settings.value("path").toString()));

    gnomonFinderTreeView *t_browser = new gnomonFinderTreeView(this);
    t_browser->setModel(model);
    t_browser->setRootIndex(model->setRootPath(settings.value("path").toString()));

    QStackedWidget *browser = new QStackedWidget(this);
    browser->addWidget(l_browser);
    browser->addWidget(t_browser);

    QVBoxLayout *r_layout = new QVBoxLayout;
    r_layout->setContentsMargins(0, 0, 0, 0);
    r_layout->setSpacing(0);
    r_layout->addLayout(t_layout);
    r_layout->addWidget(browser);

    QWidget *finder = new QWidget(this);
    finder->setLayout(r_layout);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->browse_view);
    d->splitter->addWidget(finder);

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);

// /////////////////////////////////////////////////////////////////////////////

    connect(l_browser, &gnomonFinderListView::opened, [=] (const QString& value) -> void
    {
        // TODO: Help yourself
    });

    connect(t_browser, &gnomonFinderTreeView::opened, [=] (const QString& value) -> void
    {
        // TODO: Help yourself
    });
    
    connect(l_browser, &gnomonFinderListView::changed, [=] (const QString& value) -> void
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", value);

        t_browser->setRootIndex(model->setRootPath(value));

        path->setText(value);
    });

    connect(t_browser, &gnomonFinderTreeView::changed, [=] (const QString& value) -> void
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", value);

        l_browser->setRootIndex(model->setRootPath(value));

        path->setText(value);
    });

    connect(path, &QLineEdit::editingFinished, [=] (void) -> void
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", path->text());

        l_browser->setRootIndex(model->setRootPath(path->text()));
        t_browser->setRootIndex(model->setRootPath(path->text()));
    });

    connect(up, &QToolButton::clicked, [=] (void) -> void
    {
        QDir dir = QDir(model->filePath(l_browser->rootIndex()));
        dir.cdUp();

        l_browser->setRootIndex(model->index(dir.absolutePath()));
        t_browser->setRootIndex(model->index(dir.absolutePath()));

        path->setText(dir.absolutePath());

        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", dir.absolutePath());
    });

    connect(ls, &QToolButton::clicked, [=] (void) -> void
    {
        browser->setCurrentWidget(l_browser);
    });

    connect(tr, &QToolButton::clicked, [=] (void) -> void
    {
        browser->setCurrentWidget(t_browser);
    });

// /////////////////////////////////////////////////////////////////////////////

    d->splitter->restoreState(settings.value("browser/splitter").toByteArray());
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    settings.setValue("browser/splitter", d->splitter->saveState());

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
