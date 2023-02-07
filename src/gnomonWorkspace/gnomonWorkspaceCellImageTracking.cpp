#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

int listItemsNumber(QString& cells_str) {
    QList<QList<int> > converted_list;
    QStringList result;
    QRegularExpression rx("^\\[(.*)\\]$");
    auto match = rx.match(cells_str);
    QString matched_string = match.capturedTexts()[1];
    QChar open_bracket = '[';
    QChar close_bracket = ']';
    QChar comma = ',';
    bool bracket_opened = false;
    bool bracket_closed = false;
    int counter = 0;
    for(auto ms: matched_string) {

        if(ms == open_bracket) {
            bracket_opened = true;
        }

        if(ms == close_bracket) {
            bracket_closed = true;
        }

        if((bracket_opened && bracket_closed) || (comma == ms && !bracket_closed)){
            counter++;
            if(bracket_closed) {
                bracket_opened = false;
                bracket_closed = false;
            }
        }
    }

    return counter;
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageTrackingPrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceCellImageTrackingPrivate
{

public:
     gnomonWorkspaceCellImageTrackingPrivate(void);
    ~gnomonWorkspaceCellImageTrackingPrivate(void);

public:
    gnomonViewData *source_dict = nullptr;
};

gnomonWorkspaceCellImageTrackingPrivate::gnomonWorkspaceCellImageTrackingPrivate(void)
{
}

gnomonWorkspaceCellImageTrackingPrivate::~gnomonWorkspaceCellImageTrackingPrivate(void)
{
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageTracking
// /////////////////////////////////////////////////////////////////////////////


gnomonWorkspaceCellImageTracking::gnomonWorkspaceCellImageTracking(QObject *parent): gnomonAlgorithmWorkspace(parent)
{
    dd = new gnomonWorkspaceCellImageTrackingPrivate;

    loadPluginGroup("cellImageTracking");

    d->workspace = "Cellular Tracking";
    d->command   = new gnomonCellImageTrackingCommand;
    d->keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    d->algorithmsData = gnomonCore::cellImageTracking::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();
    dd->source_dict = new gnomonViewData();
    dd->source_dict->setInputView(true);
    dd->source_dict->setAcceptForm("gnomonDataDict", true);

    emit parametersChanged();

    d->updatePool();

    connect(this->target(), &gnomonViewForm::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
    connect(this->source(), &gnomonViewForm::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
    connect(this->target(), &gnomonViewForm::formAdded, [=](const QString &name) {
        const QString plugin_name = "visualizationCellImageMarchingCubes";
        if(name == "gnomonCellImage" &&
        gnomonVisualization::visualizationCellImage::pluginFactory().keys().contains(plugin_name))
        {
            this->target()->setFormVisuName(name, plugin_name);
            this->target()->setFormVisuParameter("gnomonCellImage", "property_name", "ancestor");
            this->target()->update();
        }
    });
    connect(this->source(), &gnomonViewForm::formAdded, [=](const QString &name) {
        const QString plugin_name = "visualizationCellImageMarchingCubes";
        if(name == "gnomonCellImage" &&
           gnomonVisualization::visualizationCellImage::pluginFactory().keys().contains(plugin_name))
        {
            this->source()->setFormVisuName(name, plugin_name);
            if (this->target()->empty()) {
                this->target()->setForm("gnomonCellImage", this->source()->cellImage()->clone());
                if (!this->target()->synced()) {
                    this->target()->tryLinking();
                }
            }
        }
    });

}

gnomonWorkspaceCellImageTracking::~gnomonWorkspaceCellImageTracking(void)
{

}

gnomonViewData *gnomonWorkspaceCellImageTracking::sourceDict(void) const
{
    return dd->source_dict;
}

void gnomonWorkspaceCellImageTracking::setInputs(void)
{
    gnomonAlgorithmWorkspace::setInputs();

    QString manual_lineage;
    QVariant source_lineage = this->source()->formVisuParameter("gnomonCellImage", "manual_lineage");
    if (source_lineage.canConvert<QString>()) {
        manual_lineage += source_lineage.toString();
    } else {
        manual_lineage += "[]";
    }
    if(listItemsNumber(manual_lineage) < 4) {
        // generate signal to warn the user
    }
    manual_lineage += ", ";
    QVariant target_lineage = this->target()->formVisuParameter("gnomonCellImage", "manual_lineage");
    if (target_lineage.canConvert<QString>()) {
        manual_lineage += target_lineage.toString();
    } else {
        manual_lineage += "[]";
    }

    std::shared_ptr<gnomonDataDictSeries> input_dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(dd->source_dict->form("gnomonDataDict"));
    if (!input_dict) {
        QStringList data_dict_plugins = gnomonCore::dataDictData::pluginFactory().keys();
        if (data_dict_plugins.size() > 0) {
            gnomonAbstractDataDictData *data_dict_data = gnomonCore::dataDictData::pluginFactory().create(data_dict_plugins[0]);
            std::shared_ptr<gnomonDataDict> data_dict = std::make_shared<gnomonDataDict>();
            data_dict->setData(data_dict_data);

            input_dict = std::make_shared<gnomonDataDictSeries>();
            input_dict->insert(0, data_dict);
        }
    }
    if (input_dict) {
        input_dict->current()->set("manual_lineage", manual_lineage);
    }

    d->command->setInputForm("transformation", input_dict);
}

void gnomonWorkspaceCellImageTracking::viewOutputs()
{
    gnomonCellImageTrackingCommand * command = dynamic_cast<gnomonCellImageTrackingCommand *>(d->command);

    if(command->cellImage()) {
        this->target()->removeForm("gnomonCellImage");
        auto cellImage = command->cellImage();
        int count = gnomonFormManager::instance()->formCount(cellImage->formName());
        cellImage->metadata()->set("name", cellImage->formName() + QString::number(count+1));
        cellImage->metadata()->set("source", d->algorithm);
        this->target()->setForm("gnomonCellImage", cellImage);
        this->target()->render();
    }

    if (command->cellImage() != nullptr) {
        d->registerPipeline();
        if(!this->target()->synced()) {
            this->target()->tryLinking();
        } else {
            this->target()->setCurrentTime(this->source()->currentTime()+1.0);
        }
    }
}
