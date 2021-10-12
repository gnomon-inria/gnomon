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

// #include <gnomonWidgets>
// #include <gnomonVisualization>
// #include <gnomonComposer>

// #include <dtkThemes>
// #include <dtkWidgets>
// #include <dtkWidgetsMenuBar_p.h>
// #include <dtkWidgetsMenu+ux.h>

// #include <QtGui>
// #include <QtWidgets>
// #include <QtQuick>
// #include <QtQuickWidgets>

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeReaderCommand>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

// class gnomonFinderListView : public QListView
// {
//     Q_OBJECT

// public:
//      gnomonFinderListView(QWidget *parent = 0);
//     ~gnomonFinderListView(void);

// signals:
//     void changed(const QString& path);
//     void opened(const QString& path);

// protected:
//      void keyPressEvent(QKeyEvent *event);
//      void mouseDoubleClickEvent(QMouseEvent *event);
//      void startDrag(Qt::DropActions supportedActions);
// };

// // /////////////////////////////////////////////////////////////////////////////

// gnomonFinderListView::gnomonFinderListView(QWidget *parent) : QListView(parent)
// {
//     this->setViewMode(QListView::IconMode);
//     this->setResizeMode(QListView::Adjust);
//     this->setGridSize(QSize(96, 96));
//     this->setFrameStyle(QFrame::NoFrame);
//     this->setWordWrap(true);
//     this->setAttribute(Qt::WA_MacShowFocusRect, false);
// }

// gnomonFinderListView::~gnomonFinderListView(void)
// {

// }

// void gnomonFinderListView::keyPressEvent(QKeyEvent *event)
// {
//     if(event->key() == Qt::Key_Up &&(event->modifiers() & Qt::ControlModifier)) {
//         if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
//             QDir dir = QDir(model->filePath(this->rootIndex()));
//             dir.cdUp();
//             this->setRootIndex(model->index(dir.absolutePath()));
//             emit changed(dir.absolutePath());
//             event->accept();
//             return;
//         }
//     }

//     if(event->key() == Qt::Key_Down &&(event->modifiers() & Qt::ControlModifier)) {
//         if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

//             if(!this->selectionModel()->selectedIndexes().count())
//                 return;

//             QFileInfo selection = model->fileInfo(this->selectionModel()->selectedIndexes().first());

//             if(selection.isDir()) {
//                 this->setRootIndex(model->index(selection.filePath()));
//                 emit changed(selection.absoluteFilePath());
//                 event->accept();
//             }

//             return;
//         }
//     }

//     QListView::keyPressEvent(event);
// }

// void gnomonFinderListView::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

//         QModelIndex index = indexAt(event->pos());

//         if(!index.isValid())
//             return;

//         QFileInfo info(model->filePath(index));

//         if(info.isDir()) {

//             QDir dir = QDir(model->filePath(index));

//             emit changed(dir.absolutePath());

//             this->setRootIndex(index);

//         } else {

//             emit opened(model->filePath(index));
//         }
//     }

//     QListView::mouseDoubleClickEvent(event);
// }

// void gnomonFinderListView::startDrag(Qt::DropActions supportedActions)
// {
//     QModelIndexList indexes = selectedIndexes();

//     if(indexes.count() > 0) {

//         QMimeData *data = model()->mimeData(indexes);
//         if(!data)
//             return;

//         QFileIconProvider provider;

//         QPixmap pixmap = provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
//         QDrag *drag = new QDrag(this);
//         drag->setPixmap(pixmap);
//         drag->setMimeData(data);
//         drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
//         drag->exec(supportedActions, Qt::IgnoreAction);
//     }
// }

// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////

// class gnomonFinderTreeView : public QTreeView
// {
//     Q_OBJECT

// public:
//      gnomonFinderTreeView(QWidget *parent = 0);
//     ~gnomonFinderTreeView(void);

// signals:
//     void changed(const QString& path);
//     void opened(const QString& path);

// protected:
//      void keyPressEvent(QKeyEvent *event);
//      void mouseDoubleClickEvent(QMouseEvent *event);
//      void startDrag(Qt::DropActions supportedActions);
// };

// /////////////////////////////////////////////////////////////////////////////

// gnomonFinderTreeView::gnomonFinderTreeView(QWidget *parent) : QTreeView(parent)
// {
//     this->setDragEnabled(true);
//     this->setFrameStyle(QFrame::NoFrame);
//     this->setAttribute(Qt::WA_MacShowFocusRect, false);
//     this->setSortingEnabled(true);
//     this->sortByColumn(0, Qt::AscendingOrder);

//     this->header()->setSectionResizeMode(QHeaderView::Interactive);
// }

// gnomonFinderTreeView::~gnomonFinderTreeView(void)
// {

// }

// void gnomonFinderTreeView::keyPressEvent(QKeyEvent *event)
// {
//     if(event->key() == Qt::Key_Up &&(event->modifiers() & Qt::ControlModifier)) {
//         if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
//             QDir dir = QDir(model->filePath(this->rootIndex()));
//             dir.cdUp();
//             this->setRootIndex(model->index(dir.absolutePath()));
//             emit changed(dir.absolutePath());
//             event->accept();
//             return;
//         }
//     }

//     if(event->key() == Qt::Key_Down &&(event->modifiers() & Qt::ControlModifier)) {
//         if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

//             if(!this->selectionModel()->selectedIndexes().count())
//                 return;

//             QFileInfo selection = model->fileInfo(this->selectionModel()->selectedIndexes().first());

//             if(selection.isDir()) {
//                 this->setRootIndex(model->index(selection.filePath()));
//                 emit changed(selection.absoluteFilePath());
//                 event->accept();
//             }

//             return;
//         }
//     }

//     QTreeView::keyPressEvent(event);
// }

// void gnomonFinderTreeView::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

//         QModelIndex index = indexAt(event->pos());

//         if(!index.isValid())
//             return;

//         QFileInfo info(model->filePath(index));

//         if(info.isDir()) {

//             QDir dir = QDir(model->filePath(index));

//             emit changed(dir.absolutePath());

//             this->setRootIndex(index);

//         } else {

//             emit opened(model->filePath(index));
//         }
//     }

//     QTreeView::mouseDoubleClickEvent(event);
// }

// void gnomonFinderTreeView::startDrag(Qt::DropActions supportedActions)
// {
//     QModelIndexList indexes = selectedIndexes();

//     if(indexes.count() > 0) {

//         QMimeData *data = model()->mimeData(indexes);
//         if(!data)
//             return;

//         QFileIconProvider provider;

//         QPixmap pixmap = provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
//         QDrag *drag = new QDrag(this);
//         drag->setPixmap(pixmap);
//         drag->setMimeData(data);
//         drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
//         drag->exec(supportedActions, Qt::IgnoreAction);
//     }
// }

// // ///////////////////////////////////////////////////////////////////
// //
// // ///////////////////////////////////////////////////////////////////

// class gnomonBrowserReaderMenu : public QQuickWidget
// {
//     Q_OBJECT

// public:
//     gnomonBrowserReaderMenu(QVariantMap, QWidget *parent = nullptr);

// protected:
//     void focusOutEvent(QFocusEvent *event) override
//     {
//         this->close();
//         this->deleteLater();

//         QQuickWidget::focusOutEvent(event);
//     }
// };

// gnomonBrowserReaderMenu::gnomonBrowserReaderMenu(QVariantMap reader_descs, QWidget *parent) : QQuickWidget(parent)
// {
//     this->engine()->addImportPath("qrc:/");

//     QQmlContext *context = this->rootContext();
//     context->setContextProperty("font", dtkFontAwesome::instance());
//     context->setContextProperty("theme", dtkThemesEngine::instance());
//     context->setContextProperty("reader_descs", reader_descs);

//     this->setResizeMode(QQuickWidget::SizeRootObjectToView);
//     this->setSource(QUrl("qrc:/gnomonWorkspaceBrowser.qml"));
//     this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
//     this->setFocus(Qt::PopupFocusReason);
// }


/////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceBrowserPrivate: public QObject
{
    Q_OBJECT

public:
//   gnomonPipeline *pipeline;

public:
//    gnomonViewForm *browse_view;
//    gnomonViewMatplotlib *browse_figure;

public:
//    QStackedWidget *view_stack = nullptr;
//    gnomonMessageBoard *view_message = nullptr;

public:
//    QSplitter *splitter;

public:
    QMap<QString, QMap<QString, gnomonAbstractCommand *> > fileReaderCommands;
    QMap<QString, QMap<QString, QString> > fileReaderDescriptions;

public:
    QString filename;
    QString ext;
//    gnomonBrowserReaderMenu *menu = nullptr;

public:
    gnomonWorkspaceBrowser *q;

public:
     gnomonWorkspaceBrowserPrivate(void);
    ~gnomonWorkspaceBrowserPrivate(void);

public:
//    void addFormFromFile(const QString& path, vtkRenderer *renderer);
    void addFormFromFile(const QString& path);

public slots:
    void readForm(const QString& reader_plugin);
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
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
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
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
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
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
            fileReaderCommands[ext][key] = new gnomonCellComplexReaderCommand(key);
        }
        delete reader;
    }

    loadPluginGroup("dataFrameReader");
    QStringList dataFrame_reader_plugins = gnomonCore::dataFrameReader::pluginFactory().keys();
    for (const auto& key : dataFrame_reader_plugins)
    {
        gnomonAbstractDataFrameReader *reader = dynamic_cast<gnomonAbstractDataFrameReader *>(gnomonCore::dataFrameReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
            fileReaderCommands[ext][key] = new gnomonDataFrameReaderCommand(key);
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
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
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
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
            fileReaderCommands[ext][key] = new gnomonPointCloudReaderCommand(key);
        }
        delete reader;
    }

    loadPluginGroup("treeReader");
    QStringList tree_reader_plugins = gnomonCore::treeReader::pluginFactory().keys();
    for (const auto& key : tree_reader_plugins)
    {
        gnomonAbstractTreeReader *reader = dynamic_cast<gnomonAbstractTreeReader *>(gnomonCore::treeReader::pluginFactory().create(key));
        qDebug()<<key<<reader->extensions();
        for (const auto& ext : reader->extensions())
        {
            if (!this->fileReaderCommands.contains(ext))
            {
                QMap<QString, QString> empty_desc;
                fileReaderDescriptions[ext] = empty_desc;
                QMap<QString, gnomonAbstractCommand *> empty_list;
                fileReaderCommands[ext] = empty_list;
            }
            fileReaderDescriptions[ext][key] = reader->documentation().split("\n")[1];
            fileReaderCommands[ext][key] = new gnomonTreeReaderCommand(key);
        }
        delete reader;
    }
}

gnomonWorkspaceBrowserPrivate::~gnomonWorkspaceBrowserPrivate(void)
{
}

void gnomonWorkspaceBrowserPrivate::addFormFromFile(const QString& path)
{
    this->filename = path;

    if (this->filename.endsWith("gz")) {
        this->ext = this->filename.split(".")[this->filename.split(".").size()-2] + ".gz";
    } else {
        this->ext = this->filename.split(".")[this->filename.split(".").size()-1];
    }

    qDebug() << Q_FUNC_INFO << "WANTS" << this->ext << "IN" << this->fileReaderCommands.keys();

    if (this->fileReaderCommands.contains(this->ext))
    {
        qDebug()<< Q_FUNC_INFO <<  this->fileReaderCommands[this->ext];

        if (this->fileReaderCommands[this->ext].size()==1) {

            qDebug() << Q_FUNC_INFO << "Reading using" << this->ext;

            this->readForm(this->fileReaderCommands[this->ext].keys()[0]);
        } else {
            QVariantMap reader_descs;
            for (const auto &key : this->fileReaderCommands[this->ext].keys()) {
                reader_descs[key] = fileReaderDescriptions[ext][key];
            }

            emit q->available(reader_descs);

            // this->readForm(this->fileReaderCommands[this->ext].keys()[0]); // TODO: Fixme

//             this->menu = new gnomonBrowserReaderMenu(reader_descs);
//             this->menu->setAttribute(Qt::WA_DeleteOnClose, true);
//             this->menu->resize(dtkApp->window()->width() * 1/3, dtkApp->window()->height() - 40);
// //            this->menu->move(dtkApp->window()->frameGeometry().topRight() - QPoint(this->menu->width(),0));
//             this->menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
//             this->menu->show();

//             QObject *context = this->menu->rootObject();
//             connect(context, SIGNAL(clicked(const QString&)), this, SLOT(readForm(const QString&)));

//             QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(menu);
        }

    } else {
        qWarning() << Q_FUNC_INFO << "File format"<<this->ext<<"is not supported.";
    }


    return;
}

void gnomonWorkspaceBrowserPrivate::readForm(const QString& reader_plugin)
{
    gnomonAbstractCommand *readerCommand = this->fileReaderCommands[this->ext][reader_plugin];

    qDebug() << Q_FUNC_INFO << readerCommand;

    QString path = filename.remove("file://");
    if (gnomonImageReaderCommand *imageCommand = dynamic_cast<gnomonImageReaderCommand *>(readerCommand))
    {
        imageCommand->setPath(path);
        imageCommand->redo();
        gnomonImageSeries * image_series = (gnomonImageSeries *) imageCommand->image();
        if (!image_series) {
            qWarning() << Q_FUNC_INFO << "Resulting image series is void.";
        } else {
//TODO            this->browse_view->setForm("gnomonImage",image_series->clone());
//             this->pipeline->addClonedForm(image_series,this->browse_view->image());
// //            this->view_stack->setCurrentWidget(this->browse_view);
//             this->pipeline->addReader(imageCommand);
        }
    } else if (gnomonCellImageReaderCommand *cellImageCommand = dynamic_cast<gnomonCellImageReaderCommand *>(readerCommand))
    {


        cellImageCommand->setPath(path);
        cellImageCommand->redo();
        gnomonCellImageSeries * cellImage_series = (gnomonCellImageSeries *) cellImageCommand->cellImage();
        if (!cellImage_series) {
            qWarning() << Q_FUNC_INFO << "Resulting cellImage series is void.";
        } else {
//TODO             this->browse_view->setForm("gnomonCellImage",cellImage_series->clone());
//             this->pipeline->addClonedForm(cellImage_series,this->browse_view->cellImage());
// //            this->view_stack->setCurrentWidget(this->browse_view);
//             this->pipeline->addReader(cellImageCommand);
        }
    } else if (gnomonCellComplexReaderCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexReaderCommand *>(readerCommand))
    {
        cellComplexCommand->setPath(path);
        cellComplexCommand->redo();
        gnomonCellComplexSeries * cellComplex_series = (gnomonCellComplexSeries *) cellComplexCommand->cellComplex();
        if (!cellComplex_series) {
            qWarning() << Q_FUNC_INFO << "Resulting cellComplex series is void.";
        } else {

            qDebug() << Q_FUNC_INFO << "HERE" << cellComplex_series;

//TODO            this->browse_view->setForm("gnomonCellComplex",cellComplex_series->clone());
//             this->pipeline->addClonedForm(cellComplex_series,this->browse_view->cellComplex());
// //            this->view_stack->setCurrentWidget(this->browse_view);
//             this->pipeline->addReader(cellComplexCommand);
        }
    } else if (gnomonDataFrameReaderCommand *dataFrameCommand = dynamic_cast<gnomonDataFrameReaderCommand *>(readerCommand))
    {
        dataFrameCommand->setPath(path);
        dataFrameCommand->redo();
        gnomonDataFrameSeries * dataFrame_series = (gnomonDataFrameSeries *) dataFrameCommand->dataFrame();
        if (!dataFrame_series) {
            qWarning() << Q_FUNC_INFO << "Resulting dataFrame series is void.";
        } else {
//TODO            this->browse_figure->setForm("gnomonDataFrame",dataFrame_series->clone());
//             this->pipeline->addClonedForm(dataFrame_series,this->browse_figure->form("gnomonDataFrame"));
// //            this->view_stack->setCurrentWidget(this->browse_figure);
//             this->pipeline->addReader(dataFrameCommand);
        }
    } else if (gnomonMeshReaderCommand *meshCommand = dynamic_cast<gnomonMeshReaderCommand *>(readerCommand))
    {
        meshCommand->setPath(path);
        meshCommand->redo();
        gnomonMeshSeries * mesh_series = (gnomonMeshSeries *) meshCommand->mesh();
        if (!mesh_series) {
            qWarning() << Q_FUNC_INFO << "Resulting mesh series is void.";
        } else {

            qDebug() << Q_FUNC_INFO << "HERE" << mesh_series;

//TODO            this->browse_view->setForm("gnomonMesh",mesh_series->clone());
//             this->pipeline->addClonedForm(mesh_series,this->browse_view->mesh());
// //            this->view_stack->setCurrentWidget(this->browse_view);
//             this->pipeline->addReader(meshCommand);
        }
    } else if (gnomonPointCloudReaderCommand *pointCloudCommand = dynamic_cast<gnomonPointCloudReaderCommand *>(readerCommand))
    {
        pointCloudCommand->setPath(path);
        pointCloudCommand->redo();
        gnomonPointCloudSeries * pointCloud_series = (gnomonPointCloudSeries *) pointCloudCommand->pointCloud();
        if (!pointCloud_series) {
            qWarning() << Q_FUNC_INFO << "Resulting pointCloud series is void.";
        } else {

            qDebug() << Q_FUNC_INFO << "HERE - 2" << pointCloud_series;

//TODO            this->browse_view->setForm("gnomonPointCloud",pointCloud_series->clone());
//             this->pipeline->addClonedForm(pointCloud_series,this->browse_view->pointCloud());
// //            this->view_stack->setCurrentWidget(this->browse_view);
//             this->pipeline->addReader(pointCloudCommand);
        }
    } else if (gnomonTreeReaderCommand *treeCommand = dynamic_cast<gnomonTreeReaderCommand *>(readerCommand))
    {
        treeCommand->setPath(path);
        treeCommand->redo();
        gnomonTreeSeries * tree_series = (gnomonTreeSeries *) treeCommand->tree();
        if (!tree_series) {
            qWarning() << Q_FUNC_INFO << "Resulting tree series is void.";
        } else {
//TODO            this->browse_figure->setForm("gnomonTree",tree_series->clone());
//             this->pipeline->addClonedForm(tree_series,this->browse_figure->form("gnomonTree"));
// //            this->view_stack->setCurrentWidget(this->browse_figure);
//             this->pipeline->addReader(treeCommand);
        }
    }
    // if (this->menu) {
    //     this->menu->close();
    // }
}

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowser
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QObject *parent) : QObject(parent)
{
    d = new gnomonWorkspaceBrowserPrivate;
    d->q = this;

//    d->pipeline = gnomonPipeline::instance();

    // d->browse_view = new gnomonViewForm(this);
    // d->browse_view->setExportColor(this->color);
    // d->browse_view->setAcceptForm("gnomonCellComplex",true);
    // d->browse_view->setAcceptForm("gnomonCellImage",true);
    // d->browse_view->setAcceptForm("gnomonImage",true);
    // d->browse_view->setAcceptForm("gnomonMesh",true);
    // d->browse_view->setAcceptForm("gnomonPointCloud",true);
    // d->browse_view->setAcceptDrops(true);

//NOTE    connect(d->browse_view, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    // d->browse_figure = new gnomonViewMatplotlib(this);
    // d->browse_figure->setAcceptForm("gnomonTree",true);
    // d->browse_figure->setAcceptForm("gnomonDataFrame",true);
    // d->browse_figure->setAcceptForm("gnomonLString",true);
    // d->browse_figure->setAcceptDrops(true);

//NOTE    connect(d->browse_figure, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    // d->view_message = new gnomonMessageBoard(this);
    // d->view_message->setMessage("Double-click or drop a file");

    // d->view_stack = new QStackedWidget(this);
    // d->view_stack->addWidget(d->view_message);
    // d->view_stack->addWidget(d->browse_view);
    // d->view_stack->addWidget(d->browse_figure);

    // connect(d->browse_view, &gnomonViewForm::formAdded, [=] (const QString&)
    // {
    //     d->view_stack->setCurrentWidget(d->browse_view);
    // });

    // connect(d->browse_figure, &gnomonViewMatplotlib::formAdded, [=] (const QString&)
    // {
    //     d->view_stack->setCurrentWidget(d->browse_figure);
    //     d->browse_figure->updateVisualizations();
    // });

    // QToolButton *view_button = new QToolButton(this);
    // view_button->setIcon(dtkFontAwesome::instance()->icon(fa::cubes));
    // view_button->setToolTip("3D Form Viewer");
    // QToolButton *figure_button = new QToolButton(this);
    // figure_button->setIcon(dtkFontAwesome::instance()->icon(fa::square));
    // figure_button->setToolTip("2D Form Viewer");

    // QHBoxLayout *v_layout = new QHBoxLayout;
    // v_layout->addWidget(view_button);
    // v_layout->addWidget(figure_button);
    // v_layout->addStretch();

    // QVBoxLayout *viewers_layout = new QVBoxLayout;
    // viewers_layout->setContentsMargins(0, 0, 0, 0);
    // viewers_layout->setSpacing(0);
    // viewers_layout->addLayout(v_layout);
    // viewers_layout->addWidget(d->view_stack);

    // QWidget *viewers = new QWidget(this);
    // viewers->setLayout(viewers_layout);

    // connect(view_button, &QToolButton::clicked, [=] (void) -> void
    // {
    //     d->view_stack->setCurrentWidget(d->browse_view);
    // });

    // connect(figure_button, &QToolButton::clicked, [=] (void) -> void
    // {
    //     d->view_stack->setCurrentWidget(d->browse_figure);
    // });

/////////////////////////////////////////////////////////////////////////////

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

//     QLineEdit *path = new QLineEdit(settings.value("path").toString(), this);

//     QToolButton *up = new QToolButton(this); up->setIcon(dtkFontAwesome::instance()->icon(fa::arrowleft));
//     QToolButton *ls = new QToolButton(this); ls->setIcon(dtkFontAwesome::instance()->icon(fa::th));
//     QToolButton *tr = new QToolButton(this); tr->setIcon(dtkFontAwesome::instance()->icon(fa::list));

//     QHBoxLayout *t_layout = new QHBoxLayout;
//     t_layout->addWidget(up);
//     t_layout->addWidget(ls);
//     t_layout->addWidget(tr);
//     t_layout->addWidget(path);

//     QFileSystemModel *model = new QFileSystemModel(this);

//     gnomonFinderListView *l_browser = new gnomonFinderListView(this);
//     l_browser->setModel(model);
//     l_browser->setRootIndex(model->setRootPath(settings.value("path").toString()));

//     gnomonFinderTreeView *t_browser = new gnomonFinderTreeView(this);
//     t_browser->setModel(model);
//     t_browser->setRootIndex(model->setRootPath(settings.value("path").toString()));

//     QStackedWidget *browser = new QStackedWidget(this);
//     browser->addWidget(t_browser);
//     browser->addWidget(l_browser);

//     QVBoxLayout *r_layout = new QVBoxLayout;
//     r_layout->setContentsMargins(0, 0, 0, 0);
//     r_layout->setSpacing(0);
//     r_layout->addLayout(t_layout);
//     r_layout->addWidget(browser);

//     QWidget *finder = new QWidget(this);
//     finder->setLayout(r_layout);

//     d->splitter = new QSplitter(this);
//     d->splitter->addWidget(viewers);
//     d->splitter->addWidget(finder);

// /////////////////////////////////////////////////////////////////////////////

//     QHBoxLayout *layout = new QHBoxLayout(this);
//     layout->setContentsMargins(0, 0, 0, 0);
//     layout->setSpacing(0);
//     layout->addWidget(d->splitter);

// /////////////////////////////////////////////////////////////////////////////
//     connect(d->browse_view, &gnomonViewForm::fileDropped, [=] (const QString& filename)
//     {
//         d->addFormFromFile(filename);
//     });

//     connect(d->browse_figure, &gnomonViewMatplotlib::fileDropped, [=] (const QString& filename)
//     {
//         d->addFormFromFile(filename);
//     });

//     connect(d->view_message, &gnomonMessageBoard::fileDropped, [=] (const QString& filename)
//     {
//         d->addFormFromFile(filename);
//     });

//     connect(l_browser, &gnomonFinderListView::opened, [=] (const QString& filename) -> void
//     {
//         d->addFormFromFile(filename);
//     });

//     connect(t_browser, &gnomonFinderTreeView::opened, [=] (const QString& filename) -> void
//     {
//         d->addFormFromFile(filename);
//     });

//     connect(l_browser, &gnomonFinderListView::changed, [=] (const QString& value) -> void
//     {
//         QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
//         settings.setValue("path", value);

//         t_browser->setRootIndex(model->setRootPath(value));

//         path->setText(value);
//     });

//     connect(t_browser, &gnomonFinderTreeView::changed, [=] (const QString& value) -> void
//     {
//         QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
//         settings.setValue("path", value);

//         l_browser->setRootIndex(model->setRootPath(value));

//         path->setText(value);
//     });

//     connect(path, &QLineEdit::editingFinished, [=] (void) -> void
//     {
//         QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
//         settings.setValue("path", path->text());

//         l_browser->setRootIndex(model->setRootPath(path->text()));
//         t_browser->setRootIndex(model->setRootPath(path->text()));
//     });

//     connect(up, &QToolButton::clicked, [=] (void) -> void
//     {
//         QDir dir = QDir(model->filePath(l_browser->rootIndex()));
//         dir.cdUp();

//         l_browser->setRootIndex(model->index(dir.absolutePath()));
//         t_browser->setRootIndex(model->index(dir.absolutePath()));

//         path->setText(dir.absolutePath());

//         QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
//         settings.setValue("path", dir.absolutePath());
//     });

//     connect(ls, &QToolButton::clicked, [=] (void) -> void
//     {
//         browser->setCurrentWidget(l_browser);
//     });

//     connect(tr, &QToolButton::clicked, [=] (void) -> void
//     {
//         browser->setCurrentWidget(t_browser);
//     });

// /////////////////////////////////////////////////////////////////////////////

//     d->splitter->restoreState(settings.value("browser/splitter").toByteArray());

//     t_browser->header()->resizeSection(0,300);
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    // QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    // settings.setValue("browser/splitter", d->splitter->saveState());

    delete d;
}

void gnomonWorkspaceBrowser::read(const QString& path)
{
    d->addFormFromFile(path);
}

void gnomonWorkspaceBrowser::readWith(const QString& reader)
{
    d->readForm(reader);
}

// /////////////////////////////////////////////////////////////////////////////

#include "gnomonWorkspaceBrowser.moc"

//
// gnomonWorkspaceBrowser.cpp ends here
