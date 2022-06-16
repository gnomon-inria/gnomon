#include "gnomonWorkspaceBrowser.h"

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
#include <gnomonVisualization/gnomonView/gnomonViewForm.h>

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCommand/gnomonBinaryImage/gnomonBinaryImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeReaderCommand>
#include <zip.h>

class gnomonWorkspaceBrowserPrivate: public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowserPrivate(gnomonWorkspaceBrowser *);
    ~gnomonWorkspaceBrowserPrivate(void);

public:
    void findReaders(const QString &default_plugin);

public slots:
    bool readForm(const QString& reader_plugin);

public:
    gnomonPipelineManager *pipeline_manager;
    gnomonViewForm *browse_view;
    gnomonWorkspaceBrowser *q;
    QMap<QString, QMap<QString, gnomonAbstractCommand *> > fileReaderCommands;
    QMap<QString, QMap<QString, QString> > fileReaderDescriptions;
    QString filename;
    QString ext;

};

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowserPrivate
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowserPrivate::gnomonWorkspaceBrowserPrivate(gnomonWorkspaceBrowser *q)
{
    this->q = q;

    QList<gnomonAbstractReaderCommand*> commands;
    commands << new gnomonBinaryImageReaderCommand;
    commands << new gnomonCellImageReaderCommand;
    commands << new gnomonCellComplexReaderCommand;
    commands << new gnomonDataFrameReaderCommand;
    commands << new gnomonImageReaderCommand;
    commands << new gnomonMeshReaderCommand;
    commands << new gnomonPointCloudReaderCommand;
    commands << new gnomonTreeReaderCommand;

    for (auto command: commands) {
        QMap<QString, QStringList> extensions = command->extensions();
        auto descriptions = command->descriptions();
        for (const auto& algo_name : command->algorithmNames()) {
            for (QString ext : extensions[algo_name]) {

                if (!this->fileReaderCommands.contains(ext)) {
                    QMap<QString, QString> empty_desc;
                    fileReaderDescriptions[ext] = empty_desc;
                    QMap<QString, gnomonAbstractCommand *> empty_list;
                    fileReaderCommands[ext] = empty_list;
                }

                fileReaderDescriptions[ext][algo_name] = descriptions[algo_name].split("\n")[1];
                fileReaderCommands[ext][algo_name] = command;
            }
        }
        QObject::connect(command, SIGNAL(finished()), q, SIGNAL(finished()));
    }
}

gnomonWorkspaceBrowserPrivate::~gnomonWorkspaceBrowserPrivate(void)
{
}

void gnomonWorkspaceBrowserPrivate::findReaders(const QString &default_plugin)
{
    if (this->fileReaderCommands.contains(this->ext))
    {
        auto available_plugins = this->fileReaderCommands[this->ext].keys();

        QVariantMap reader_descs;
        if(available_plugins.contains(default_plugin)) {
            reader_descs[default_plugin] = fileReaderDescriptions[ext][default_plugin];
        } else {
            for (const auto &plugin_name : available_plugins) {
                reader_descs[plugin_name] = fileReaderDescriptions[ext][plugin_name];
            }
        }
        emit q->available(reader_descs);
    } else {
        dtkWarn() << Q_FUNC_INFO << "File format"<<this->ext<<"is not supported.";
    }
    
    return;
}

bool gnomonWorkspaceBrowserPrivate::readForm(const QString& reader_plugin)
{
    gnomonAbstractCommand *readerCommand = this->fileReaderCommands[this->ext][reader_plugin];
    if(!readerCommand) {
        dtkWarn() << Q_FUNC_INFO << "cannot create readerCommand " << reader_plugin << " for extension " << this->ext;
        return false;
    }
    readerCommand->setAlgorithmName(reader_plugin);

    QStringList paths;
    QStringList sources;
    for (auto file : this->filename.split(",")) {
        QString file_path = file.remove("file://");
        if(!QFile::exists(file_path)) {
            dtkWarn() << Q_FUNC_INFO << "file " << file_path << "doesn't exist";
        } else {
            paths.append(file_path);
        }
    }
    if (paths.size() == 0) {
        return false;
    }
    for (auto file_path: paths) {
        sources.append(QFileInfo(file_path).fileName());
    }
    QString path = paths.join(",");
    QString source = sources.join(",");

    if (gnomonImageReaderCommand *imageCommand = dynamic_cast<gnomonImageReaderCommand *>(readerCommand))
    {
        imageCommand->setPath(path);
        imageCommand->redo();
        std::shared_ptr<gnomonImageSeries>  image_series = imageCommand->image();
        if (!image_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting image series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(image_series->formName());
            image_series->metadata()->set("name", image_series->formName().remove("gnomon") + QString::number(form_count+1));
            image_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonImage",image_series);
            //this->pipeline_manager->addClonedForm(image_series,this->browse_view->image());
            gnomonPipelineManager::instance()->addForm(image_series);
            this->pipeline_manager->addReader(imageCommand);
        }
    } else if (gnomonCellImageReaderCommand *cellImageCommand = dynamic_cast<gnomonCellImageReaderCommand *>(readerCommand))
    {
        cellImageCommand->setPath(path);
        cellImageCommand->redo();
        std::shared_ptr<gnomonCellImageSeries>  cellImage_series = cellImageCommand->cellImage();
        if (!cellImage_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting cellImage series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(cellImage_series->formName());
            cellImage_series->metadata()->set("name", cellImage_series->formName().remove("gnomon") + QString::number(form_count+1));
            cellImage_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonCellImage",cellImage_series);
            gnomonPipelineManager::instance()->addForm(cellImage_series);
            //this->pipeline_manager->addClonedForm(cellImage_series,this->browse_view->cellImage());
            this->pipeline_manager->addReader(cellImageCommand);
        }
    } else if (gnomonCellComplexReaderCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexReaderCommand *>(readerCommand))
    {
        cellComplexCommand->setPath(path);
        cellComplexCommand->redo();
        std::shared_ptr<gnomonCellComplexSeries>  cellComplex_series = cellComplexCommand->cellComplex();
        if (!cellComplex_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting cellComplex series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(cellComplex_series->formName());
            cellComplex_series->metadata()->set("name", cellComplex_series->formName().remove("gnomon") + QString::number(form_count+1));
            cellComplex_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonCellComplex",cellComplex_series);
            gnomonPipelineManager::instance()->addForm(cellComplex_series);
            //this->pipeline_manager->addClonedForm(cellComplex_series,this->browse_view->cellComplex());
            this->pipeline_manager->addReader(cellComplexCommand);
        }
    } else if (gnomonBinaryImageReaderCommand *binaryImageCommand = dynamic_cast<gnomonBinaryImageReaderCommand *>(readerCommand))
    {
        binaryImageCommand->setPath(path);
        binaryImageCommand->redo();
        std::shared_ptr<gnomonBinaryImageSeries>  binaryImage_series = binaryImageCommand->binaryImage();
        if (!binaryImage_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting binaryImage series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(binaryImage_series->formName());
            binaryImage_series->metadata()->set("name", binaryImage_series->formName().remove("gnomon") + QString::number(form_count+1));        QStringList sources;
            binaryImage_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonBinaryImage",binaryImage_series);
            gnomonPipelineManager::instance()->addForm(binaryImage_series);
            //this->pipeline_manager->addClonedForm(binaryImage_series, this->browse_view->binaryImage());
            this->pipeline_manager->addReader(binaryImageCommand);
        }
    } else if (gnomonDataFrameReaderCommand *dataFrameCommand = dynamic_cast<gnomonDataFrameReaderCommand *>(readerCommand))
    {
        dataFrameCommand->setPath(path);
        dataFrameCommand->redo();
        std::shared_ptr<gnomonDataFrameSeries>  dataFrame_series = dataFrameCommand->dataFrame();
        if (!dataFrame_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting dataFrame series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(dataFrame_series->formName());
            dataFrame_series->metadata()->set("name", dataFrame_series->formName().remove("gnomon") + QString::number(form_count+1));
            dataFrame_series->metadata()->set("source", source);
//            this->browse_figure->setForm("gnomonDataFrame",dataFrame_series);
//            this->pipeline_manager->addClonedForm(dataFrame_series,this->browse_figure->form("gnomonDataFrame"));
            gnomonPipelineManager::instance()->addForm(dataFrame_series);
            this->pipeline_manager->addReader(dataFrameCommand);
        }
    } else if (gnomonMeshReaderCommand *meshCommand = dynamic_cast<gnomonMeshReaderCommand *>(readerCommand))
    {
        meshCommand->setPath(path);
        meshCommand->redo();
        std::shared_ptr<gnomonMeshSeries>  mesh_series = meshCommand->mesh();
        if (!mesh_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting mesh series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(mesh_series->formName());
            mesh_series->metadata()->set("name", mesh_series->formName().remove("gnomon") + QString::number(form_count+1));
            mesh_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonMesh",mesh_series);
            gnomonPipelineManager::instance()->addForm(mesh_series);
            //this->pipeline_manager->addClonedForm(mesh_series,this->browse_view->mesh());
            this->pipeline_manager->addReader(meshCommand);
        }
    } else if (gnomonPointCloudReaderCommand *pointCloudCommand = dynamic_cast<gnomonPointCloudReaderCommand *>(readerCommand))
    {
        pointCloudCommand->setPath(path);
        pointCloudCommand->redo();
        std::shared_ptr<gnomonPointCloudSeries>  pointCloud_series = pointCloudCommand->pointCloud();
        if (!pointCloud_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting pointCloud series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(pointCloud_series->formName());
            pointCloud_series->metadata()->set("name", pointCloud_series->formName().remove("gnomon") + QString::number(form_count+1));
            pointCloud_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonPointCloud",pointCloud_series);
            gnomonPipelineManager::instance()->addForm(pointCloud_series);
            //this->pipeline_manager->addClonedForm(pointCloud_series,this->browse_view->pointCloud());
            this->pipeline_manager->addReader(pointCloudCommand);
        }
    } else if (gnomonTreeReaderCommand *treeCommand = dynamic_cast<gnomonTreeReaderCommand *>(readerCommand))
    {
        treeCommand->setPath(path);
        treeCommand->redo();
        std::shared_ptr<gnomonTreeSeries>  tree_series = treeCommand->tree();
        if (!tree_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting tree series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(tree_series->formName());
            tree_series->metadata()->set("name", tree_series->formName().remove("gnomon") + QString::number(form_count+1));
            tree_series->metadata()->set("source", source);
            gnomonPipelineManager::instance()->addForm(tree_series);
//            this->browse_figure->setForm("gnomonTree",tree_series);
//            this->pipeline_manager->addClonedForm(tree_series,this->browse_figure->form("gnomonTree"));
            this->pipeline_manager->addReader(treeCommand);
        }
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowser
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowser::gnomonWorkspaceBrowser(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceBrowserPrivate(this);

    d->pipeline_manager = gnomonPipelineManager::instance();

    d->browse_view = new gnomonViewForm(this);
    // d->browse_view->setExportColor(this->color);
    d->browse_view->setAcceptForm("gnomonBinaryImage", true);
    d->browse_view->setAcceptForm("gnomonCellComplex",true);
    d->browse_view->setAcceptForm("gnomonCellImage",true);
    d->browse_view->setAcceptForm("gnomonImage",true);
    d->browse_view->setAcceptForm("gnomonMesh",true);
    d->browse_view->setAcceptForm("gnomonPointCloud",true);
    // d->browse_view->setAcceptDrops(true);

    connect(d->browse_view, &gnomonViewForm::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        d->pipeline_manager->addForm(f);
    });
    // d->browse_figure = new gnomonViewMatplotlib(this);
    // d->browse_figure->setAcceptForm("gnomonTree",true);
    // d->browse_figure->setAcceptForm("gnomonDataFrame",true);
    // d->browse_figure->setAcceptForm("gnomonLString",true);
    // d->browse_figure->setAcceptDrops(true);

    // connect(d->browse_figure, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline_manager, SLOT(addForm(gnomonAbstractDynamicForm *)));

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
//         d->findReaders(filename);
//     });

//     connect(d->browse_figure, &gnomonViewMatplotlib::fileDropped, [=] (const QString& filename)
//     {
//         d->findReaders(filename);
//     });

//     connect(d->view_message, &gnomonMessageBoard::fileDropped, [=] (const QString& filename)
//     {
//         d->findReaders(filename);
//     });

//     connect(l_browser, &gnomonFinderListView::opened, [=] (const QString& filename) -> void
//     {
//         d->findReaders(filename);
//     });

//     connect(t_browser, &gnomonFinderTreeView::opened, [=] (const QString& filename) -> void
//     {
//         d->findReaders(filename);
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


const QString& gnomonWorkspaceBrowser::readerPath(void) const
{
    return d->filename;
}

void gnomonWorkspaceBrowser::setReaderPath(const QString& path)
{
    if (path != d->filename) {
        d->filename = path;

        QStringList filenames = d->filename.split(",");

        // check for conformity
        QString ext = filenames[0].split(".").sliced(1).join(".");
        for(const auto & fname: filenames) {
            if(ext != fname.split(".").sliced(1).join(".")) {
                dtkWarn() << Q_FUNC_INFO << "Selected files don't have the same extension. Please select files with the same extensions.";
                return;
            }
        }

        if(ext.endsWith("zip")) {
            // reading the manifest
            int err = 0;
            zip *z = zip_open(filenames[0].remove("file://").toStdString().c_str(), 0, &err);

            //Search for the file of given name
            const char *name = "manifest.json";
            auto index = zip_name_locate(z, name, 0);
            if(index < 0) {
                dtkWarn() << Q_FUNC_INFO << "Invalid time series container. Containers doesn't have a manifest.json file.";
                return;
            }

            struct zip_stat st{};
            zip_stat_init(&st);
            zip_stat(z, name, 0, &st);

            //Alloc memory for its uncompressed contents
            char *contents = new char[st.size];

            //Read the compressed file
            zip_file *f = zip_fopen(z, name, 0);
            zip_fread(f, contents, st.size);
            zip_fclose(f);

            //And close the archive
            zip_close(z);

            // getting the extension
            auto manifest_doc = QJsonDocument::fromJson(QByteArray::fromRawData(contents, (qsizetype)st.size));
            //delete allocated memory
            delete[] contents;
            auto manifest = manifest_doc.object();
            if(!(manifest.contains("extension") && manifest.contains("series"))) {
                dtkWarn() << Q_FUNC_INFO << "Invalid time series container. manifest.json does not provide both extension and files fields";
                return;
            }
            d->ext = manifest["extension"].toString();


        } else {
            d->ext = ext;
        }

        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("path", QUrl(filenames[0]).adjusted(QUrl::RemoveFilename).toString());
        
        emit readerPathChanged();
    }
}

void gnomonWorkspaceBrowser::requestReaders(QString default_reader="")
{
    d->findReaders(default_reader);
}

bool gnomonWorkspaceBrowser::readWith(const QString& reader)
{
    return d->readForm(reader);
}

gnomonViewForm *gnomonWorkspaceBrowser::view(void)
{
    return d->browse_view;
}

void gnomonWorkspaceBrowser::restoreState(void)
{
    d->browse_view->restoreState();
}

QUrl gnomonWorkspaceBrowser::defaultReadPath()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    return settings.value("path").toString();
}

QStringList gnomonWorkspaceBrowser::readerExtensions(void)
{
    return d->fileReaderCommands.keys();
}

// /////////////////////////////////////////////////////////////////////////////

#include "gnomonWorkspaceBrowser.moc"

//
// gnomonWorkspaceBrowser.cpp ends here
