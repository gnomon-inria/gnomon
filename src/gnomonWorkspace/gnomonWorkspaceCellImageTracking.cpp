#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

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
    emit algorithmsLoaded();

    d->workspace = "Cellular Tracking";
    d->command   = new gnomonCellImageTrackingCommand;
    d->keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    d->algorithm = d->command->algorithmName();

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
        }
    });
    connect(this->source(), &gnomonViewForm::formAdded, [=](const QString &name) {
        const QString plugin_name = "visualizationCellImageMarchingCubes";
        if(name == "gnomonCellImage" &&
           gnomonVisualization::visualizationCellImage::pluginFactory().keys().contains(plugin_name))
        {
            this->source()->setFormVisuName(name, plugin_name);
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

    QStringList cells;
    for(double d: this->source()->pickedCells())
        cells.append(QString::number(d));
    QString picked_cells = cells.join(",");

    std::shared_ptr<gnomonDataDictSeries> input_dict = std::dynamic_pointer_cast<gnomonDataDictSeries>(dd->source_dict->form("gnomonDataDict"));
    input_dict->current()->set("picked_cells", picked_cells);
    d->command->setInputForm("transformation", input_dict);
}
