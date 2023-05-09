#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractCellImageVtkVisualization"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

int listItemsNumber(const QString& cells_str) {
    QRegularExpression rx("^\\[(.*)\\]$");
    auto match = rx.match(cells_str);
    if (match.hasMatch()){
        QString matched_string = match.capturedTexts()[1];
        int list_depth = 0;
        int counter = 0;
        for(auto ms: matched_string) {
            if (ms == '[') {
                list_depth += 1;
            } else if (ms == ']') {
                list_depth -= 1;
            } else if (ms == ','){
                if (list_depth == 0) {
                    counter++;
                }
            } else if (ms != ' ' && counter == 0) {
                counter = 1;
            }
        }
        return counter;
    }
    return 0;
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
    gnomonQmlView *source_dict = nullptr;
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

    d->workspace = "Cellular Tracking";
    d->command   = new gnomonCellImageTrackingCommand;
    d->keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    d->algorithmsData = gnomonCore::cellImageTracking::pluginFactory().dataList();
    d->algorithm = d->command->algorithmName();

    emit algorithmsLoaded();

    //create the views
    this->addInputView();
    this->addOutputView();
    dd->source_dict = new gnomonQmlView();
    dd->source_dict->setInputView(true);
    dd->source_dict->setAcceptForm("gnomonDataDict", true);

    emit parametersChanged();

    d->updatePool();

    connect(this->target(), &gnomonVtkView::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
    connect(this->source(), &gnomonVtkView::syncedChanged, [=]() {
        this->target()->disconnectTime();
        this->source()->disconnectTime();
        this->target()->setCurrentTime(this->source()->currentTime()+1.0);
    });
    connect(this->target(), &gnomonVtkView::formAdded, [=](const QString &name) {
        const QString plugin_name = "cellImageVtkVisualizationMarchingCubes";
        if(name == "gnomonCellImage" &&
        gnomonVisualization::cellImageVtkVisualization::pluginFactory().keys().contains(plugin_name))
        {
            this->target()->setFormVisuName(name, plugin_name);
            this->target()->setFormVisuParameter("gnomonCellImage", "property_name", "ancestor");
            this->target()->update();
        }
    });
    connect(this->source(), &gnomonVtkView::formAdded, [=](const QString &name) {
        const QString plugin_name = "cellImageVtkVisualizationMarchingCubes";
        if(name == "gnomonCellImage" &&
           gnomonVisualization::cellImageVtkVisualization::pluginFactory().keys().contains(plugin_name))
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

gnomonQmlView *gnomonWorkspaceCellImageTracking::sourceDict(void) const
{
    return dd->source_dict;
}

void gnomonWorkspaceCellImageTracking::setInputs(void)
{
    gnomonAlgorithmWorkspace::setInputs();
    std::shared_ptr<gnomonDataDictSeries> input_dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(dd->source_dict->form("gnomonDataDict"));

    QString manual_lineage;
    QVariant source_lineage = this->source()->formVisuParameter("gnomonCellImage", "manual_lineage");
    if (source_lineage.canConvert<QString>()) {
        manual_lineage += source_lineage.toString();
    } else {
        manual_lineage += "[]";
    }
    if (manual_lineage != "[]") {
        int manual_lineage_size = listItemsNumber(manual_lineage);
        if (manual_lineage_size < 4) {
            emit notEnoughCells(manual_lineage_size);
        } else if (input_dict) {
            emit notInitTrans();
        }
    }
    manual_lineage += ", ";
    QVariant target_lineage = this->target()->formVisuParameter("gnomonCellImage", "manual_lineage");
    if (target_lineage.canConvert<QString>()) {
        manual_lineage += target_lineage.toString();
    } else {
        manual_lineage += "[]";
    }

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
        auto cellImage = command->cellImage();
        int count = gnomonFormManager::instance()->formCount(cellImage->formName());
        cellImage->metadata()->set("name", cellImage->formName() + QString::number(count+1));
        cellImage->metadata()->set("source", d->algorithm);

        this->source()->removeForm("gnomonCellImage");
        this->source()->setForm("gnomonCellImage", cellImage);
        this->source()->render();

        this->target()->removeForm("gnomonCellImage");
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
