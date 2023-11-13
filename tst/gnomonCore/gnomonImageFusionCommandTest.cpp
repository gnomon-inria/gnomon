#include "gnomonImageFusionCommandTest.h"

#include <gnomonCore>
#include <gnomonTest>


#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFusionCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>

#include <dtkScript>

namespace fusion {
bool t_remove_images_called = false;
int t_nb_image_added = 0;
bool t_set_parameter_called = false;
bool t_run_called = false;
bool t_remove_landmarks = false;
}


class dummyImageFusionPlugin : public gnomonAbstractImageFusion {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {fusion::t_set_parameter_called = true;};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};

    int run(void) override{ fusion::t_run_called = true; return 0;};
    QString documentation(void) override {return "empty";};
    void addImage(std::shared_ptr<gnomonImageSeries> ) override { fusion::t_nb_image_added++;};
    void removeImages(void) override { fusion::t_remove_images_called = true; fusion::t_nb_image_added=0;};

    void addLandmarks(const std::vector<gnomonLandmark>&) override {};
    void removeLandmarks(void) override {fusion::t_remove_landmarks = true;};

    std::shared_ptr<gnomonImageSeries> output() override { return nullptr;};
    void clearInputs(void) override {};
    void clearOutputs(void) override {};
};

inline gnomonAbstractImageFusion* dummyImageFusionPluginCreator(void)  {
    return new dummyImageFusionPlugin();
}

class gnomonImageFusionCommandTestCasePrivate
{
public:
    QVector< std::shared_ptr<gnomonImageSeries> > image_series;
    gnomonImageFusionCommand *fusion_command = nullptr;
};

gnomonImageFusionCommandTestCase::gnomonImageFusionCommandTestCase(void) : d(new gnomonImageFusionCommandTestCasePrivate)
{
}

gnomonImageFusionCommandTestCase::~gnomonImageFusionCommandTestCase(void)
{
    delete d;
}

void gnomonImageFusionCommandTestCase::initTestCase(void)
{
    dtkScriptInterpreterPython::instance()->init();
    gnomonCore::imageFusion::pluginFactory().record("dummyImageFusion", dummyImageFusionPluginCreator);
}

void gnomonImageFusionCommandTestCase::init(void)
{
    d->fusion_command = new gnomonImageFusionCommand();
    Q_ASSERT(d->fusion_command);
    d->fusion_command->setAlgorithmName("dummyImageFusion");
}

void gnomonImageFusionCommandTestCase::redo(void)
{
    d->image_series.push_back(std::make_shared<gnomonImageSeries>());
    d->image_series.push_back(std::make_shared<gnomonImageSeries>());
    d->image_series.push_back(std::make_shared<gnomonImageSeries>());

    d->fusion_command->addImage(d->image_series[0]);
    d->fusion_command->addImage(d->image_series[1]);
    d->fusion_command->addImage(d->image_series[2]);

    d->fusion_command->setParameter("nb_iterations", 0);
    d->fusion_command->setParameter("n_job", 1);
    d->fusion_command->redo();

    QVERIFY(fusion::t_nb_image_added==3 && fusion::t_run_called && fusion::t_remove_images_called);
}

void gnomonImageFusionCommandTestCase::undo(void)
{
    d->fusion_command->undo();
    QVERIFY(fusion::t_remove_landmarks);
}

void gnomonImageFusionCommandTestCase::cleanup(void)
{
    delete d->fusion_command;
    d->fusion_command = nullptr;
}

void gnomonImageFusionCommandTestCase::cleanupTestCase(void)
{
}

GNOMONTEST_MAIN_NOGUI(gnomonImageFusionCommandTest, gnomonImageFusionCommandTestCase);
