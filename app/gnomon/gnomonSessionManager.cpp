#include <gnomonPipeline>
#include <gnomonVisualization>
#include <gnomonWorkspace>
#include <memory>

#include <QtQml>

#include "gnomonSessionManager.h"

// /////////////////////////////////////////////////////////////////
// gnomonSessionManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonSessionManagerPrivate : public QObject
{
public:
    gnomonSessionManagerPrivate(QObject *parent = nullptr);
    ~gnomonSessionManagerPrivate(void);
public:
    bool runNodes(QStringList scheduled_nodes, std::shared_ptr<gnomonPipeline> pipeline, double progress_increment);

public:
    gnomonSessionManager *q = nullptr;

public:
    QString file_path;
    bool res = false;


    QObject *window = nullptr;
    QQmlApplicationEngine* engine = nullptr;

private:
    QMetaObject::Connection callbackConnection;   
};

gnomonSessionManagerPrivate::gnomonSessionManagerPrivate(QObject *parent) : QObject(parent)
{

}
gnomonSessionManagerPrivate::~gnomonSessionManagerPrivate(void)
{

}

bool gnomonSessionManagerPrivate::runNodes(QStringList scheduled_nodes, std::shared_ptr<gnomonPipeline> pipeline,
                                           double progress_increment)
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
        for (size_t i = 0; i < paths.size(); i++) {
            QDir path(paths[i]);
            if(path.isRelative()) {
                paths[i] = path.absolutePath();
            }
        }
        QDir::setCurrent(tmp);

        callbackConnection = QObject::connect(browser, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(browser->view()->empty()) {
                emit q->failed(file_path);
                return false;
            } else {
                browser->view()->transmit();
            }
            q->setProgress(q->progress() + progress_increment);
            res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
            res = runNodes(scheduled_nodes, pipeline, progress_increment);
        });

    } else if(node->algorithmClass().contains("Writer", Qt::CaseInsensitive)) {
        dtkInfo() << "Writer node (not creating anything) : " << node->algorithmClass();
        q->setProgress(q->progress() + progress_increment);
        res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
        res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
            res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
            res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
            res = runNodes(scheduled_nodes, pipeline, progress_increment);
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
}

gnomonSessionManager::~gnomonSessionManager(void)
{
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

bool gnomonSessionManager::loadFromPipeline(const QString &path) {
    if(!d->alive) {
        return false;
    }
    auto pipeline = std::make_shared<gnomonPipeline>();
    d->file_path = path;
    pipeline->readFromJson(path, true);
    gnomonPipelineManager::instance()->pipeline()->setName(pipeline->name());
    gnomonPipelineManager::instance()->pipeline()->setDescription(pipeline->description());
    auto scheduled_nodes = pipeline->scheduledNodeNames(true);
    return d->runNodes(scheduled_nodes, pipeline, 0);
}

void gnomonSessionManager::setEngine(QQmlApplicationEngine *engine) {
    d->engine = engine;
}

void gnomonSessionManager::setWindow(QObject *window) {
    d->window = window;
}

int gnomonSessionManager::loadWorkspace(const QString &source, const QString &uuid) {
    int index = 0;
    QMetaObject::invokeMethod(d->window, "add_workspace",
                                        Q_RETURN_ARG(int, index),
                                        Q_ARG(QString, source),
                                        Q_ARG(QString, uuid));
    return index;
}

int gnomonSessionManager::newWorkspace(const QString &source) {
    int index = 0;
    auto uuid = QUuid::createUuid().toString();
    QMetaObject::invokeMethod(d->window, "add_workspace",
                              Q_RETURN_ARG(int, index),
                              Q_ARG(QString, source),
                              Q_ARG(QString, uuid));
    return index;
}

}


//
// gnomonSessionManager.cpp ends here
