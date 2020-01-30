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

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudReaderCommand>

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
    this->setResizeMode(QListView::Adjust);
    this->setGridSize(QSize(96, 96));
    this->setFrameStyle(QFrame::NoFrame);
    this->setWordWrap(true);
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

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

gnomonFinderTreeView::gnomonFinderTreeView(QWidget *parent) : QTreeView(parent)
{
    this->setDragEnabled(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    this->header()->setSectionResizeMode(QHeaderView::Interactive);
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

/////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceBrowserPrivate
{
public:
    gnomonViewForm *browse_view;
    gnomonWorkspaceBrowser *q;

public:
    QStackedWidget *view_stack = nullptr;
    gnomonMessageBoard *view_message = nullptr;

public:
    QSplitter *splitter;

public:
    QMap<QString, QMap<QString, gnomonAbstractCommand *> > fileReaderCommands;

public:
     gnomonWorkspaceBrowserPrivate(void);
    ~gnomonWorkspaceBrowserPrivate(void);

public:
    void addFormFromFile(const QString& path);
};

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowserPrivate
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowserPrivate::gnomonWorkspaceBrowserPrivate(void)
{
    loadPluginGroup("imageReader");
    QStringList image_reader_plugins = gnomonCore::imageReader::pluginFactory().keys();
    for (const auto& key : image_reader_plugins)
    {
        gnomonAbstractImageReader *reader = dynamic_cast<gnomonAbstractImageReader *>(gnomonCore::imageReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderCommands[ext][key] = new gnomonImageReaderCommand(key);
        }
        delete reader;
    }
       
    loadPluginGroup("cellImageReader");
    QStringList cellImage_reader_plugins = gnomonCore::cellImageReader::pluginFactory().keys();
    for (const auto& key : cellImage_reader_plugins)
    {
        gnomonAbstractCellImageReader *reader = dynamic_cast<gnomonAbstractCellImageReader *>(gnomonCore::cellImageReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderCommands[ext][key] = new gnomonCellImageReaderCommand(key);
        }
        delete reader;
    }
    
    loadPluginGroup("cellComplexReader");
    QStringList cellComplex_reader_plugins = gnomonCore::cellComplexReader::pluginFactory().keys();
    for (const auto& key : cellComplex_reader_plugins)
    {
        gnomonAbstractCellComplexReader *reader = dynamic_cast<gnomonAbstractCellComplexReader *>(gnomonCore::cellComplexReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderCommands[ext][key] = new gnomonCellComplexReaderCommand(key);
        }
        delete reader;
    }
    
    loadPluginGroup("meshReader");
    QStringList mesh_reader_plugins = gnomonCore::meshReader::pluginFactory().keys();
    for (const auto& key : mesh_reader_plugins)
    {
        gnomonAbstractMeshReader *reader = dynamic_cast<gnomonAbstractMeshReader *>(gnomonCore::meshReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderCommands[ext][key] = new gnomonMeshReaderCommand(key);
        }
        delete reader;
    }
    
    loadPluginGroup("pointCloudReader");
    QStringList pointCloud_reader_plugins = gnomonCore::pointCloudReader::pluginFactory().keys();
    for (const auto& key : pointCloud_reader_plugins)
    {
        gnomonAbstractPointCloudReader *reader = dynamic_cast<gnomonAbstractPointCloudReader *>(gnomonCore::pointCloudReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderCommands[ext][key] = new gnomonPointCloudReaderCommand(key);
        }
        delete reader;
    }
}

gnomonWorkspaceBrowserPrivate::~gnomonWorkspaceBrowserPrivate(void)
{
}

void gnomonWorkspaceBrowserPrivate::addFormFromFile(const QString& path)
{
    QString filename = path;

    QString ext;
    if (filename.endsWith("gz")) {
        ext = filename.split(".")[filename.split(".").size()-2] + ".gz";
    } else {
        ext = filename.split(".")[filename.split(".").size()-1];
    }

    if (this->fileReaderCommands.contains(ext))
    {
        qDebug()<<this->fileReaderCommands[ext];

        gnomonAbstractCommand *readerCommand = nullptr;

        if (this->fileReaderCommands[ext].size()==1) {
            readerCommand = this->fileReaderCommands[ext].values()[0];
        } else {
            QDialog *dialog = new QDialog(this->q);
            dialog->setFixedSize(QSize(400,200));

            QVBoxLayout *layout = new QVBoxLayout();

            QComboBox *combo_box = new QComboBox();
            for (const auto &key : this->fileReaderCommands[ext].keys()) {
                combo_box->addItem(key);
            }
            combo_box->model()->sort(0);
            layout->addWidget(combo_box);

            QPushButton *button = new QPushButton("OK");
            QObject::connect(button, &QPushButton::clicked, [=] () {
                dialog->accept();
            });
            layout->addWidget(button);

            dialog->setLayout(layout);
            int status = dialog->exec();

            if (status == QDialog::Accepted) {
                QString key = combo_box->currentText();
                readerCommand = this->fileReaderCommands[ext][key];
            }
        }

        if (gnomonImageReaderCommand *imageCommand = dynamic_cast<gnomonImageReaderCommand *>(readerCommand))
        {
            imageCommand->setPath(filename.remove("file://"));
            imageCommand->redo();
            gnomonImageSeries * image_series = (gnomonImageSeries *) imageCommand->image()->clone();
            if (!image_series) {
                qWarning() << Q_FUNC_INFO << "Resulting image series is void.";
            } else {
                this->browse_view->setForm("gnomonImage",image_series);
                this->view_stack->setCurrentWidget(this->browse_view);
            }
        } else if (gnomonCellImageReaderCommand *cellImageCommand = dynamic_cast<gnomonCellImageReaderCommand *>(readerCommand))
        {
            cellImageCommand->setPath(filename.remove("file://"));
            cellImageCommand->redo();
            gnomonCellImageSeries * cellImage_series = (gnomonCellImageSeries *) cellImageCommand->cellImage()->clone();
            if (!cellImage_series) {
                qWarning() << Q_FUNC_INFO << "Resulting cellImage series is void.";
            } else {
                this->browse_view->setForm("gnomonCellImage",cellImage_series);
                this->view_stack->setCurrentWidget(this->browse_view);
            }
        } else if (gnomonCellComplexReaderCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexReaderCommand *>(readerCommand))
        {
            cellComplexCommand->setPath(filename.remove("file://"));
            cellComplexCommand->redo();
            gnomonCellComplexSeries * cellComplex_series = (gnomonCellComplexSeries *) cellComplexCommand->cellComplex()->clone();
            if (!cellComplex_series) {
                qWarning() << Q_FUNC_INFO << "Resulting cellComplex series is void.";
            } else {
                this->browse_view->setForm("gnomonCellComplex",cellComplex_series);
                this->view_stack->setCurrentWidget(this->browse_view);
            }
        } else if (gnomonMeshReaderCommand *meshCommand = dynamic_cast<gnomonMeshReaderCommand *>(readerCommand))
        {
            meshCommand->setPath(filename.remove("file://"));
            meshCommand->redo();
            gnomonMeshSeries * mesh_serie = (gnomonMeshSeries *) meshCommand->mesh()->clone();
            if (!mesh_serie) {
                qWarning() << Q_FUNC_INFO << "Resulting mesh series is void.";
            } else {
                this->browse_view->setForm("gnomonMesh",mesh_serie);
                this->view_stack->setCurrentWidget(this->browse_view);
            }
        } else if (gnomonPointCloudReaderCommand *pointCloudCommand = dynamic_cast<gnomonPointCloudReaderCommand *>(readerCommand))
        {
            pointCloudCommand->setPath(filename.remove("file://"));
            pointCloudCommand->redo();
            gnomonPointCloudSeries * pointCloud_series = (gnomonPointCloudSeries *) pointCloudCommand->pointCloud()->clone();
            if (!pointCloud_series) {
                qWarning() << Q_FUNC_INFO << "Resulting pointCloud series is void.";
            } else {
                this->browse_view->setForm("gnomonPointCloud",pointCloud_series);
                this->view_stack->setCurrentWidget(this->browse_view);
            }
        } 
    } else {
        qWarning() << Q_FUNC_INFO << "File format"<<ext<<"is not supported.";
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowser
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;
    d->q = this;

    d->browse_view = new gnomonViewForm(this);
    d->browse_view->setExportColor(this->color);

    d->view_message = new gnomonMessageBoard(this);
    d->view_message->setMessage("Double-click or drop a file");

    d->view_stack = new QStackedWidget(this);
    d->view_stack->addWidget(d->view_message);
    d->view_stack->addWidget(d->browse_view);

/////////////////////////////////////////////////////////////////////////////

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

    QLineEdit *path = new QLineEdit(settings.value("path").toString(), this);

    QToolButton *up = new QToolButton(this); up->setIcon(dtkFontAwesome::instance()->icon(fa::arrowleft));
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
    browser->addWidget(t_browser);
    browser->addWidget(l_browser);

    QVBoxLayout *r_layout = new QVBoxLayout;
    r_layout->setContentsMargins(0, 0, 0, 0);
    r_layout->setSpacing(0);
    r_layout->addLayout(t_layout);
    r_layout->addWidget(browser);

    QWidget *finder = new QWidget(this);
    finder->setLayout(r_layout);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->view_stack);
    d->splitter->addWidget(finder);

/////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);

/////////////////////////////////////////////////////////////////////////////
    connect(d->browse_view, &gnomonViewForm::fileDropped, [=] (const QString& filename)
    {
        d->addFormFromFile(filename);
    });

    connect(d->view_message, &gnomonMessageBoard::fileDropped, [=] (const QString& filename)
    {
        d->addFormFromFile(filename);
    });

    connect(l_browser, &gnomonFinderListView::opened, [=] (const QString& filename) -> void
    {
        d->addFormFromFile(filename);
    });

    connect(t_browser, &gnomonFinderTreeView::opened, [=] (const QString& filename) -> void
    {
        d->addFormFromFile(filename);
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

/////////////////////////////////////////////////////////////////////////////

    d->splitter->restoreState(settings.value("browser/splitter").toByteArray());

    t_browser->header()->resizeSection(0,300);
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

const QColor gnomonWorkspaceBrowser::color = QColor("#ff3b30");

// /////////////////////////////////////////////////////////////////////////////

#include "gnomonWorkspaceBrowser.moc"

//
// gnomonWorkspaceBrowser.cpp ends here
