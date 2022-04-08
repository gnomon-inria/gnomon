#include "gnomonWorkspaceCellImageTracking.h"
#include "gnomonAlgorithmWorkspace_p.h"

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

    connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));
    /*
    connect(this->source(), &gnomonViewForm::timeChanged, [=](double time) {
        auto times = this->source()->times();
        auto index = times.indexOf(time);
        this->target()->setCurrentTime(times[std::min(index+1, times.size()-1)]);
    });

     */
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

    gnomonDataDictSeries *input_dict = dynamic_cast<gnomonDataDictSeries *>(dd->source_dict->form("gnomonDataDict"));
    d->command->setInputForm("transformation", input_dict);
}
