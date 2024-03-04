#include <gnomonPipeline>
#include <gnomonVisualization>
#include <gnomonWorkspace>
#include <memory>

#include <QtQml>

#include "gnomonSessionManager.h"
#include "gnomonProject"
#include "gnomonForm/gnomonDynamicFormFactory.h"


// /////////////////////////////////////////////////////////////////
// gnomonSessionManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonSessionManagerPrivate : public QObject
{
public:
    gnomonSessionManagerPrivate(QObject *parent = nullptr);
    ~gnomonSessionManagerPrivate(void);
public:
    bool runPipeline(std::shared_ptr<gnomonPipeline> pipeline, bool write_outputs=false);
    bool runNodes(QStringList scheduled_nodes, std::shared_ptr<gnomonPipeline> pipeline, double progress_increment, bool write_outputs=false);

public:
    gnomonSessionManager *q = nullptr;

public:
    QString file_path;
    bool res = false;

    QMap<QString, QJsonObject> workspace_properties;

    QMap<QString, QString> workspace_sources;

    QObject *window = nullptr;
    QQmlApplicationEngine* engine = nullptr;

    bool alive = true;
    bool init = false;
    bool loading_session = false;
    int active_workspace_id = -1;

private:
    QMetaObject::Connection callbackConnection;   
};

gnomonSessionManagerPrivate::gnomonSessionManagerPrivate(QObject *parent) : QObject(parent)
{

}
gnomonSessionManagerPrivate::~gnomonSessionManagerPrivate(void)
{

}

bool gnomonSessionManagerPrivate::runPipeline(std::shared_ptr<gnomonPipeline> pipeline, bool write_outputs)
{
    gnomonPipelineManager::instance()->pipeline()->setName(pipeline->name());
    gnomonPipelineManager::instance()->pipeline()->setDescription(pipeline->description());
    auto scheduled_nodes = pipeline->scheduledNodeNames(true);
    return this->runNodes(scheduled_nodes, pipeline, -1, write_outputs);
}

bool gnomonSessionManagerPrivate::runNodes(QStringList scheduled_nodes, std::shared_ptr<gnomonPipeline> pipeline,
                                           double progress_increment, bool write_outputs)
{
    res = true;

    if(progress_increment == -1.) {
        progress_increment = 1. / scheduled_nodes.size();  
    }

    if(scheduled_nodes.isEmpty()) {
        emit q->finished();
        return true;
    }
    QString n_name = scheduled_nodes.takeFirst();
    gnomonPipelineNode *node = pipeline->node(n_name);

    if(node->algorithmClass().contains("Reader", Qt::CaseInsensitive)) {
        qInfo() << "Reader node : " << node->algorithmClass();
        gnomonWorkspaceBrowser *browser;
        QMetaObject::invokeMethod(window, "workspaceBrowser",
            Q_RETURN_ARG(gnomonWorkspaceBrowser *, browser));

        if(!browser) {
            dtkWarn() << Q_FUNC_INFO << " cannot create worksace browser";
            emit q->failed(file_path);
            return false;
        }
        browser->view()->clear();

        auto tmp = QDir::currentPath();
        // QDir::setCurrent(gnomonQMLUtils::instance()->dataPath());
        QString read_path = node->path();
        QStringList paths = read_path.split(",");
        QDir::setCurrent(tmp);

        callbackConnection = QObject::connect(browser, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(browser->view()->empty()) {
                emit q->failed(file_path);
                res = false;
                return;
            } else {
                browser->view()->transmit();
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
        });

        browser->setReaderPath(paths.join(","));
        browser->readWith(node->algorithmPlugin());

    } else if(node->algorithmClass().contains("morphonetCellImage", Qt::CaseInsensitive)) {
        qInfo() << "Moprphonet node : " << node->algorithmClass();
        gnomonWorkspaceMorphonet * morphonet = nullptr;
        QVariant morphonet_data_var = node->morphonet_data().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspaceMorphonet",
                Q_RETURN_ARG(gnomonWorkspaceMorphonet *, morphonet),
                Q_ARG(QVariant, morphonet_data_var));

        if(!morphonet || !ok) {
            emit q->failed(file_path);
            return false;
        }
        callbackConnection = QObject::connect(morphonet, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(morphonet->view()->empty()) {
                emit q->failed(file_path);
                res = false;
                return;
            } else {
                morphonet->view()->transmit();
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
        });

    } else if(node->algorithmClass().contains("Writer", Qt::CaseInsensitive)) {
        dtkInfo() << "Writer node : " << node->algorithmClass();
        if (write_outputs) {
            auto inputPortsNames = node->inputPortsNames();
            int index = node->inputPort(inputPortsNames.first())->formIndex();
            QString write_path = node->path();
            gnomonFormManager::instance()->saveAs(index, write_path);
        } else {
            dtkInfo() << "Not writing anything";
        }
        q->setProgress(q->progress() + progress_increment);
        res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
    } else if(node->algorithmClass() == "task") {
        qInfo() << "Task node : " << node->algorithmPlugin();
        // auto node = dynamic_cast<gnomonPipelineNodeTask>(node);

        if(node->algorithmPlugin() == "compose") {
            auto inputPortsNames = node->inputPortsNames();
            int first = node->inputPort(inputPortsNames.first())->formIndex();
            if(node->inputPort(inputPortsNames[1])->formIndex() >=0) {
                int second = node->inputPort(inputPortsNames[1])->formIndex();
                qDebug() << "COMPOSE " << first << " -> " << second;
                gnomonFormManager::instance()->compose(first, second);
            }
        }

        q->setProgress(q->progress() + progress_increment);
        res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
    } else if(node->algorithmClass().contains("formAlgorithm", Qt::CaseInsensitive)) {
        dtkInfo() << "Node : Python Workspace";
        gnomonWorkspacePythonAlgorithm * w_p = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspacePython",
                Q_RETURN_ARG(gnomonWorkspacePythonAlgorithm *, w_p),
                Q_ARG(QVariant, p_var));

        if(!w_p || !ok) {
            emit q->failed(file_path);
            return false;
        }

        QList<gnomonVtkView *> views = w_p->sources()->views();
        for(auto *view: views)
            view->clear();

        //TOCHECK how to put into different views? now always view[0]
        for(QString in : node->inputPortsNames()) {
            if(node->inputPort(in)->formIndex() >=0) {
                views[0]->drop(node->inputPort(in)->formIndex());
            }
        }

        callbackConnection = QObject::connect(w_p, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            //TOCHECK only transmit from one target!
            if((*w_p->targets())[0]->empty()) {
                res = false;
                emit q->failed(file_path);
                return;
            } else {
                (*w_p->targets())[0]->transmit();
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
        });

        w_p->run();
    }  else if(node->algorithmClass().contains("lStringEvolutionModel", Qt::CaseInsensitive)) {
        gnomonWorkspaceLSystemModel * w_p = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspaceLSystem",
                                            Q_RETURN_ARG(gnomonWorkspaceLSystemModel *, w_p),
                                            Q_ARG(QVariant, p_var));
        if(!w_p || !ok) {
            emit q->failed(file_path);
            return false;
        }

        callbackConnection = connect(w_p, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(w_p->view()->empty()) {
                res = false;
                emit q->failed(file_path);
                return;
            } else {
                w_p->view()->transmit();
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
        });

        w_p->run();
    } else {
        qInfo() << "Node : " << node->algorithmClass();
        gnomonAlgorithmWorkspace * w_d = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "loadWorkspace",
                Q_RETURN_ARG(gnomonAlgorithmWorkspace *, w_d),
                Q_ARG(QString, node->algorithmPlugin()),
                Q_ARG(QVariant, p_var));

        if(!w_d || !ok) {
            emit q->failed(file_path);
            return false;
        }

        QList<gnomonVtkView *> views = w_d->sources()->views();
        for(auto *view: views)
            view->clear();

        //if nodePortNames is not empty use it to check if it's the good view,
        //if it's not set then try to use the view!
        for(QString in : node->inputPortsNames()) {
            if(node->inputPort(in)->formIndex() >=0) {
                for(auto *view: views){
                    if(view->nodePortNames().isEmpty() || view->nodePortNames().contains(in)) {
                        view->drop(node->inputPort(in)->formIndex());
                    }
                }
            }
        }

        if (node->algorithmClass().contains("imageRegistration", Qt::CaseInsensitive)) {
            if(node->inputPortsNames().contains("initialTransformation") && node->inputPort("initialTransformation")->formIndex() >=0) {
                w_d->textView()->drop(node->inputPort("initialTransformation")->formIndex());
            }
        }

        callbackConnection = QObject::connect(w_d, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            QStringList form_types;
            gnomonVtkViewList *targets = w_d->targets();
            bool has_target = targets && !targets->views().empty();
            if (has_target) {
                form_types.append(targets->views()[0]->acceptedForms());
            }
            gnomonMplView *figure = w_d->figure();
            bool has_figure = figure;
            if (has_figure) {
                form_types.append(figure->acceptedForms());
            }
            gnomonQmlView *text_view = w_d->textView();
            bool has_text_view = text_view;
            if (has_text_view) {
                form_types.append(text_view->acceptedForms());
            }

            form_types.removeDuplicates();
            form_types.sort();

            bool transmit = false;
            for (auto form_type : form_types) {
                //qDebug()<<Q_FUNC_INFO<<form_type;
                if (has_target) {
                    if (targets->views()[0]->contains(form_type)) {
                        targets->views()[0]->transmitForm(form_type);
                        transmit = true;
                    }
                }
                if (has_figure) {
                    if (figure->contains(form_type)) {
                        figure->transmitForm(form_type);
                        transmit = true;
                    }
                }
                if (has_text_view) {
                    if (text_view->contains(form_type)) {
                        text_view->transmitForm(form_type);
                        transmit = true;
                    }
                }
            }

            // Do not delete the next line as it seems, somehow, to prevent a crash caused by the GIL when calling nucleiDetectionTimagetk
            qInfo()<<"";

            if (!transmit) {
                res = false;
                emit q->failed(file_path);
                return;
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment, write_outputs);
        });

        w_d->run();
    }
    return res;    
}
// /////////////////////////////////////////////////////////////////
// gnomonSessionManager
// /////////////////////////////////////////////////////////////////

gnomonSessionManager::gnomonSessionManager(QObject *parent) : gnomonAbstractSessionManager(parent)
{
    d = new gnomonSessionManagerPrivate;
    d->q = this;

    connect(gnomonFormManager::instance(), &gnomonFormManager::added,
            this, &gnomonAbstractSessionManager::sync);
    connect(gnomonPipelineManager::instance()->pipeline(), &gnomonPipeline::nodeAdded,
            this, &gnomonAbstractSessionManager::sync);
    connect(gnomonPipelineManager::instance()->pipeline(), &gnomonPipeline::nodeRemoved,
            this, &gnomonAbstractSessionManager::sync);

    startTimer(1000); // doesn't work :(
}

gnomonSessionManager::~gnomonSessionManager(void)
{
    gnomonSessionManager::sync();
    delete d;
}

bool gnomonSessionManager::save(const QString &id, const QString &path)
{
    /* save a session into a map,
    the session is retrivable wit it's Id
    */
   // d->session_process->save_session(d->session);
   qWarning()<<Q_FUNC_INFO<<"Not implemented.";
   return false;
}

void gnomonSessionManager::load(const QString &id, const QString &path)
{
    /* Should return a session */
    // d->session_process->load_session(id, path)
    qWarning()<<Q_FUNC_INFO<<"Not implemented.";
}

void gnomonSessionManager::initialize() {
    gnomonAbstractSessionManager::registerInstance(new gnomonSessionManager(nullptr));
}

bool gnomonSessionManager::loadFromPipelineFile(const QString &path) {
    if(!d->alive) {
        return false;
    }
    auto pipeline = std::make_shared<gnomonPipeline>();
    d->file_path = path;
    pipeline->readFromJson(path, true);
    return d->runPipeline(pipeline);
}

bool gnomonSessionManager::loadFromPipeline(gnomonPipeline *pipeline, bool write_outputs) {
    if(!d->alive) {
        return false;
    }
    // TODO: QML pipeline will be cleared, so copy is required: is there a better way ?
    auto _pipeline = std::make_shared<gnomonPipeline>();
    _pipeline->fromJson(pipeline->toJson());
    return d->runPipeline(_pipeline, write_outputs);
}

bool gnomonSessionManager::loading(void) {
    return d->loading_session;
}

bool gnomonSessionManager::loading(void) {
    return d->loading_session;
}
void gnomonSessionManager::setEngine(QQmlApplicationEngine *engine) {
    d->engine = engine;
}

void gnomonSessionManager::setWindow(QObject *window) {
    d->window = window;
}

int gnomonSessionManager::loadWorkspace(const QString &source, const QString &uuid) {
    int index = 0;
    auto success = QMetaObject::invokeMethod(d->window, "add_workspace",
                                        Q_RETURN_ARG(int, index),
                                        Q_ARG(QString, source),
                                        Q_ARG(QString, uuid),
                                        Q_ARG(bool, false));
    if(success) {
        d->workspace_sources[uuid] = source;
    }
    return index;
}

int gnomonSessionManager::newWorkspace(const QString &source) {
    int index = 0;
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    auto success = QMetaObject::invokeMethod(d->window, "add_workspace",
                              Q_RETURN_ARG(int, index),
                              Q_ARG(QString, source),
                              Q_ARG(QString, uuid),
                              Q_ARG(bool, true));
    if(success) {
        d->workspace_sources[uuid] = source;
        this->sync();
    }
    return index;
}

void gnomonSessionManager::setActiveWorkspace(int id) {
    if(!d->loading_session) {
        d->active_workspace_id = id;
        this->sync();
    }
}

QJsonObject *gnomonSessionManager::getStorageForWorkspace(const QString &uuid) {
    if(!d->workspace_properties.contains(uuid)) {
        d->workspace_properties[uuid] = QJsonObject();
    }
    return &d->workspace_properties[uuid];
}

void gnomonSessionManager::sync() {
    if(!d->init) {
        return;
    }

    qDebug() << "===========" << "saving session";
    QSettings settings(PROJECT_SESSION_FILE, QSettings::IniFormat);
    QDir dir(GNOMON_PROJECT->projectDir());

    // building json object for properties
    QJsonObject session_json;
    QJsonObject workspace_properties;
    QJsonObject workspace_sources;
    QJsonArray workspace_order;
    auto workspaces = qmlContext(d->window)->objectForName("workspaces");
    for(auto child: workspaces->children()) {
        if(child && child->property("d").isValid() && child->property("uuid").isValid()) {
            auto uuid = child->property("uuid").toString();
            workspace_order.append(uuid);
            workspace_properties.insert(uuid, d->workspace_properties.value(uuid));
            workspace_sources.insert(uuid, d->workspace_sources.value(uuid));
        }
    }
    session_json.insert("current_index", workspaces->property("currentIndex").toInt());
    session_json.insert("workspace_order", workspace_order);
    session_json.insert("workspace_properties", workspace_properties);
    session_json.insert("workspace_sources", workspace_sources);
    session_json.insert("active_workspace_id", d->active_workspace_id);

    settings.setValue("workspaces", session_json);

    // pipeline
    auto url = QUrl::fromLocalFile(dir.absoluteFilePath(PROJECT_PIPELINE_FILE));
    gnomonPipelineManager::instance()->pipeline()->exportToJson(url.toString());
    

    //TODO: forms
    cleanExpiredForms();
    settings.beginGroup("forms");
    settings.setValue("form_manager_state", gnomonFormManager::instance()->serialize());

    settings.setValue("owned_form_ids", m_owned_forms.keys());
    QStringList form_ids;
    for(auto it = m_tracked_forms.keyValueBegin(); it != m_tracked_forms.keyValueEnd(); it++) {
        if(!it->second.expired()) {
            auto form = it->second.lock();
            settings.setValue(it->first, form->serialize());
            form_ids.append(it->first);
        }
    }
    settings.setValue("form_ids", form_ids);
    settings.endGroup();

    settings.beginGroup("pipeline");
    settings.setValue("pipeline_manager_state", gnomonPipelineManager::instance()->serialize());
    settings.endGroup();


}

bool gnomonSessionManager::load() {
    qDebug() << "===========" << "loading session";
    this->setLoadingSessionProgress(0, "Loading Session");
    d->loading_session = true;

    auto setSessionLoader = [=](double progress, const QString& message){
        this->setLoadingSessionProgress(this->loadingSessionProgress() + progress, message);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 200);
    };

    double progress_increment = 0.05;
    setSessionLoader(progress_increment, "Project Directory");

    QDir dir(GNOMON_PROJECT->projectDir());

    QSettings settings(PROJECT_SESSION_FILE, QSettings::IniFormat);
    QJsonObject workspaces_info = settings.value("workspaces").toJsonObject();

    // forms
    settings.beginGroup("forms");
    QStringList form_ids = settings.value("form_ids").toStringList();
    QStringList owned_form_ids = settings.value("owned_form_ids").toStringList();

    // thirty five percent of total time
    progress_increment = 0.35 / form_ids.size();
    int counter = 0;
    QString message;

    // hold a reference to every form until load is finished, every unused form should be cleaned up
    QList<std::shared_ptr<gnomonAbstractDynamicForm>> form_holder;
    for(const auto &uuid: form_ids) {
        message = " Form " + QString::number(++counter) + "/" + QString::number(form_ids.size());
        setSessionLoader(progress_increment, message);

        auto form = gnomonForm::createDynamicForm(settings.value(uuid).toJsonObject());
        m_tracked_forms.insert(uuid, form);
        form_holder.append(form);
        if(owned_form_ids.contains(uuid)) {
            m_owned_forms.insert(uuid, form);
        }
    }

    QJsonObject form_manager_state = settings.value("form_manager_state").toJsonObject();
    gnomonFormManager::instance()->deserialize(form_manager_state);

    settings.endGroup();

    //workspaces
    if(!workspaces_info.isEmpty()) {

        QJsonObject workspace_properties = workspaces_info["workspace_properties"].toObject();
        QJsonArray workspace_order = workspaces_info["workspace_order"].toArray();
        QJsonObject workspace_sources = workspaces_info["workspace_sources"].toObject();
        // fifty percent of total time
        progress_increment = 0.5 / workspace_order.count();
        counter = 0;
        for(const auto &id_: workspace_order) {
            message = " Workspaces " + QString::number(++counter) + "/" + QString::number(workspace_order.count());
            setSessionLoader(progress_increment, message);

            auto id = id_.toString();
            d->workspace_properties[id] = workspace_properties[id].toObject();

            loadWorkspace(workspace_sources[id].toString(), id);
        }


        //TODO: fix pipeline reloading
        // pipeline reloading is broken because pipelines are build upon the memory addresses of various component
        // and memory addresses are not transferred when reloading
        auto pipeline_path = dir.absoluteFilePath(PROJECT_PIPELINE_FILE);
        auto pipeline = std::make_shared<gnomonPipeline>();
        if(dir.exists(pipeline_path)) {
            pipeline->readFromJson(pipeline_path);
        }

        settings.beginGroup("pipeline");
        QJsonObject pipeline_manager_state = settings.value("pipeline_manager_state").toJsonObject();
        gnomonPipelineManager::instance()->deserialize(pipeline_manager_state, pipeline);
        settings.endGroup();

        progress_increment = 0.1;
        setSessionLoader(progress_increment, "Active Workspace");

        d->active_workspace_id = workspaces_info["active_workspace_id"].toInt();
        QMetaObject::invokeMethod(d->window, "switch_workspace",
                                  Q_ARG(int, d->active_workspace_id));

        d->init = true;
        d->loading_session = false;

        return true;
    } else {

        d->loading_session = false;
        return false;
    }
}

bool gnomonSessionManager::newSession(const QString &source) {
    QDir project_dir(GNOMON_PROJECT->projectDir());
    gnomonProject::recursiveRemoveDir(project_dir.filePath(PROJECT_SESSION_DIRECTORY));
    m_owned_forms.clear();
    project_dir.mkpath(PROJECT_SESSION_DIRECTORY);
    auto res = QMetaObject::invokeMethod(d->window, "switch_from_launcher",
                                     Q_ARG(QString, source));
    if(res)
        d->init = true;
    return res;
}

void gnomonSessionManager::timerEvent(QTimerEvent *event) {
    sync();
}




//
// gnomonSessionManager.cpp ends here
