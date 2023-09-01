#include "gnomonWorkspaceBrowser.h"

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
#include <gnomonVisualization/gnomonView/gnomonVtkView.h>

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCommand/gnomonBinaryImage/gnomonBinaryImageReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageReader.h>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageReader.h>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexReader.h>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameReader.h>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageReader.h>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringReader.h>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshReader.h>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudReader.h>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeReaderCommand>
#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeReader.h>
#include <zip.h>

#include <memory>

class gnomonWorkspaceBrowserPrivate: public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowserPrivate(gnomonWorkspaceBrowser *);
    ~gnomonWorkspaceBrowserPrivate(void);

public:
    void findReaders(const QString &default_plugin);
    bool viewOutputs(gnomonAbstractReaderCommand* command);

public slots:
    bool readForm(const QString& reader_plugin);

public:
    gnomonPipelineManager *pipeline_manager;
    gnomonVtkView *browse_view;
    gnomonWorkspaceBrowser *q;
    QMap<QString, QMap<QString, gnomonAbstractReaderCommand *> > fileReaderCommands;
    QMap<QString, QMap<QString, QString> > fileReaderDescriptions;
    QMap<QString, QMap<QString, QVariant> > fileReaderMetadata;
    QString filename;
    QString ext;
    QMap<QString, QMap<QString, QString> > fileReaderImagePath;
    QList<gnomonAbstractReaderCommand *> commands;
    int progress = 0;
};

/////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceBrowserPrivate
/////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceBrowserPrivate::gnomonWorkspaceBrowserPrivate(gnomonWorkspaceBrowser *q)
{
    this->q = q;
    QVariantList pluginsMetadata;

    commands << new gnomonBinaryImageReaderCommand;
    pluginsMetadata += gnomonCore::binaryImageReader::pluginFactory().dataList();
    commands << new gnomonCellImageReaderCommand;
    pluginsMetadata += gnomonCore::cellImageReader::pluginFactory().dataList();
    commands << new gnomonCellComplexReaderCommand;
    pluginsMetadata += gnomonCore::cellComplexReader::pluginFactory().dataList();
    commands << new gnomonDataFrameReaderCommand;
    pluginsMetadata += gnomonCore::dataFrameReader::pluginFactory().dataList();
    commands << new gnomonImageReaderCommand;
    pluginsMetadata += gnomonCore::imageReader::pluginFactory().dataList();
    commands << new gnomonLStringReaderCommand;
    pluginsMetadata += gnomonCore::lStringReader::pluginFactory().dataList();
    commands << new gnomonMeshReaderCommand;
    pluginsMetadata += gnomonCore::meshReader::pluginFactory().dataList();
    commands << new gnomonPointCloudReaderCommand;
    pluginsMetadata += gnomonCore::pointCloudReader::pluginFactory().dataList();
    commands << new gnomonTreeReaderCommand;
    pluginsMetadata += gnomonCore::treeReader::pluginFactory().dataList();

    QMap<QString, QVariant> pluginsMetadata2; // key -> variant
    for(const auto & var: pluginsMetadata) {
        pluginsMetadata2.insert(var.toMap()["key"].toString(), var);
    }

    for (auto command: commands) {
        QMap<QString, QStringList> extensions = command->extensions();
        auto descriptions = command->descriptions();
        auto preview = command->preview();
        for (const auto& algo_name : command->algorithmNames()) {
            for (QString ext : extensions[algo_name]) {

                if (!this->fileReaderCommands.contains(ext)) {
                    QMap<QString, QString> empty_desc;
                    fileReaderDescriptions[ext] = empty_desc;
                    QMap<QString, gnomonAbstractReaderCommand *> empty_list;
                    fileReaderCommands[ext] = empty_list;
                    fileReaderImagePath[ext] = empty_desc;
                }

                if(descriptions[algo_name].split("\n").size() >=1)
                    fileReaderDescriptions[ext][algo_name] = descriptions[algo_name].split("\n")[1];
                else
                    fileReaderDescriptions[ext][algo_name] = descriptions[algo_name];
                fileReaderCommands[ext][algo_name] = command;
                fileReaderImagePath[ext][algo_name] = preview[algo_name];
                fileReaderMetadata[ext][algo_name] = pluginsMetadata2[algo_name];
            }
        }
        connect(command, &gnomonAbstractCommand::finished, [this, q, command]() {
            this->viewOutputs(command);
            emit q->finished();
        });
    }
}

gnomonWorkspaceBrowserPrivate::~gnomonWorkspaceBrowserPrivate(void)
{
    for(auto c : commands) {
        delete c;
    }
    commands.clear();
}

void gnomonWorkspaceBrowserPrivate::findReaders(const QString &default_plugin)
{
    if (this->fileReaderCommands.contains(this->ext)) {

        auto available_plugins = this->fileReaderCommands[this->ext].keys();

        QJsonObject readers;
        QJsonObject reader_descs;
        if(available_plugins.contains(default_plugin)) {
            reader_descs.insert("description", fileReaderDescriptions[ext][default_plugin]);
            reader_descs.insert("preview", fileReaderImagePath[ext][default_plugin]);
            reader_descs.insert("metadata", QJsonValue::fromVariant(fileReaderMetadata[ext][default_plugin]));
            readers.insert(default_plugin, reader_descs);
        } else {
            for (const auto &plugin_name : available_plugins) {
                reader_descs.insert("description", fileReaderDescriptions[ext][plugin_name]);
                reader_descs.insert("preview", fileReaderImagePath[ext][plugin_name]);
                reader_descs.insert("metadata", QJsonValue::fromVariant(fileReaderMetadata[ext][plugin_name]));
                readers.insert(plugin_name, reader_descs);
            }
        }
        emit q->available(readers);
    } else {
        emit q->noReaderAvailable(this->ext);
        dtkWarn() << Q_FUNC_INFO << "File format "<<this->ext<<" is not supported.";
    }
}

bool gnomonWorkspaceBrowserPrivate::readForm(const QString& reader_plugin)
{
    gnomonAbstractReaderCommand *readerCommand = this->fileReaderCommands[this->ext][reader_plugin];
    if(!readerCommand) {
        dtkWarn() << Q_FUNC_INFO << "cannot create readerCommand " << reader_plugin << " for extension " << this->ext;
        return false;
    }
    readerCommand->setAlgorithmName(reader_plugin);

    QStringList paths;
    QStringList sources;
    for (auto file : this->filename.split(",")) {
        if(file.startsWith("file://")) {
            QString file_path = file.remove("file://");
            if(!QFile::exists(file_path)) {
                dtkWarn() << Q_FUNC_INFO << "file " << file_path << "doesn't exist";
            } else {
                paths.append(file_path);
                sources.append(QFileInfo(file_path).fileName());
            }
        } else {
            //it's not a file, most likely a url
            //
            paths.append(file);
            sources.append(QUrl(file).fileName());
        }
    }
    if (paths.size() == 0) {
        return false;
    }
    QString path = paths.join(",");
    QString source = sources.join(",");

    readerCommand->setPath(path);
    readerCommand->setSource(source);
    readerCommand->redo();

    return true;
}

bool gnomonWorkspaceBrowserPrivate::viewOutputs(gnomonAbstractReaderCommand* command)
{
    const auto &source = command->source();
    if (gnomonImageReaderCommand *imageCommand = dynamic_cast<gnomonImageReaderCommand *>(command))
    {
        std::shared_ptr<gnomonImageSeries> image_series = imageCommand->image();
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
    } else if (gnomonCellImageReaderCommand *cellImageCommand = dynamic_cast<gnomonCellImageReaderCommand *>(command))
    {
        std::shared_ptr<gnomonCellImageSeries> cellImage_series = cellImageCommand->cellImage();
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
    } else if (gnomonCellComplexReaderCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexReaderCommand *>(command))
    {
        std::shared_ptr<gnomonCellComplexSeries> cellComplex_series = cellComplexCommand->cellComplex();
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
    } else if (gnomonBinaryImageReaderCommand *binaryImageCommand = dynamic_cast<gnomonBinaryImageReaderCommand *>(command))
    {
        std::shared_ptr<gnomonBinaryImageSeries> binaryImage_series = binaryImageCommand->binaryImage();
        if (!binaryImage_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting binaryImage series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(binaryImage_series->formName());
            binaryImage_series->metadata()->set("name", binaryImage_series->formName().remove("gnomon") + QString::number(form_count+1));
            binaryImage_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonBinaryImage",binaryImage_series);
            gnomonPipelineManager::instance()->addForm(binaryImage_series);
            //this->pipeline_manager->addClonedForm(binaryImage_series, this->browse_view->binaryImage());
            this->pipeline_manager->addReader(binaryImageCommand);
        }
    } else if (gnomonDataFrameReaderCommand *dataFrameCommand = dynamic_cast<gnomonDataFrameReaderCommand *>(command))
    {
        std::shared_ptr<gnomonDataFrameSeries> dataFrame_series = dataFrameCommand->dataFrame();
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
    } else if (gnomonLStringReaderCommand *lStringCommand = dynamic_cast<gnomonLStringReaderCommand *>(command))
    {
        std::shared_ptr<gnomonLStringSeries> lString_series = lStringCommand->lString();
        if (!lString_series) {
            dtkWarn() << Q_FUNC_INFO << "Resulting lString series is void.";
            return false;
        } else {
            int form_count = gnomonFormManager::instance()->formCount(lString_series->formName());
            lString_series->metadata()->set("name", lString_series->formName().remove("gnomon") + QString::number(form_count+1));
            lString_series->metadata()->set("source", source);
            this->browse_view->setForm("gnomonLString",lString_series);
            gnomonPipelineManager::instance()->addForm(lString_series);
            //this->pipeline_manager->addClonedForm(lString_series,this->browse_view->lString());
            this->pipeline_manager->addReader(lStringCommand);
        }
    } else if (gnomonMeshReaderCommand *meshCommand = dynamic_cast<gnomonMeshReaderCommand *>(command))
    {
        std::shared_ptr<gnomonMeshSeries> mesh_series = meshCommand->mesh();
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
    } else if (gnomonPointCloudReaderCommand *pointCloudCommand = dynamic_cast<gnomonPointCloudReaderCommand *>(command))
    {
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
    } else if (gnomonTreeReaderCommand *treeCommand = dynamic_cast<gnomonTreeReaderCommand *>(command))
    {
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

    d->browse_view = new gnomonVtkView(this);
    d->browse_view->setNodePortNames({});
    // d->browse_view->setExportColor(this->color);
    d->browse_view->setAcceptForm("gnomonBinaryImage", true);
    d->browse_view->setAcceptForm("gnomonCellComplex",true);
    d->browse_view->setAcceptForm("gnomonCellImage",true);
    d->browse_view->setAcceptForm("gnomonImage",true);
    d->browse_view->setAcceptForm("gnomonLString",true);
    d->browse_view->setAcceptForm("gnomonMesh",true);
    d->browse_view->setAcceptForm("gnomonPointCloud",true);
    // d->browse_view->setAcceptDrops(true);

    connect(d->browse_view, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        d->pipeline_manager->addForm(f);
        this->m_can_be_destroyed = false;
        emit canBeDestroyedChanged(false);
    });
}

gnomonWorkspaceBrowser::~gnomonWorkspaceBrowser(void)
{
    delete d;
}

int gnomonWorkspaceBrowser::progress(void) {
    return d->progress;
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

        QString filename = filenames[0].split(".").join(".").toLower();
        QString ext = "";

        QMap<QString, QMap<QString, gnomonAbstractReaderCommand *> > ::iterator i;
        for (i = d->fileReaderCommands.begin(); i != d->fileReaderCommands.end(); ++i)
        {
            if( filename.endsWith(i.key()))
            {
                ext = i.key();
            }
        }



        if(filename.endsWith(".zip")) {
            // reading the manifest
            int err = 0;
            zip *z = zip_open(filenames[0].remove("file://").toStdString().c_str(), 0, &err);

            //Search for the file of given name
            const char *name = "manifest.json";
            auto index = zip_name_locate(z, name, 0);
            if(index < 0) {
                dtkWarn() << Q_FUNC_INFO << "Invalid time series container. Containers doesn't have a manifest.json file.";
                d->ext = "";
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
                d->ext = "";
                return;
            }
            ext = manifest["extension"].toString();
        }

        if(ext.isEmpty()) {
            dtkWarn() << Q_FUNC_INFO << "Extension not recognized";
            d->ext = "";
            return;
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
    emit started();
    return d->readForm(reader);
}

gnomonVtkView *gnomonWorkspaceBrowser::view(void) const
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

void gnomonWorkspaceBrowser::export_outputs(void) {
    d->browse_view->transmit();
}

// /////////////////////////////////////////////////////////////////////////////

#include "gnomonWorkspaceBrowser.moc"

//
// gnomonWorkspaceBrowser.cpp ends here
