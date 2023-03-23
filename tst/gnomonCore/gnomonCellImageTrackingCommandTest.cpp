#include "gnomonCellImageTrackingCommandTest.h"
#include "gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageTracking"
#include "gnomonCommand/gnomonCellImage/gnomonCellImageTrackingCommand"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkScriptInterpreterPython>
#include <QTestAccessibility>


namespace tracking{
    bool t_run_called = false;
    bool t_set_parameter_called = false;
    bool t_set_cell_image_called = false;
    bool t_set_image_called = false;
}

class dummyCellImageTrackingPlugin : public gnomonAbstractCellImageTracking {
public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override {tracking::t_set_parameter_called = true;};
    dtkCoreParameters parameters(void) const override {return dtkCoreParameters();};
    void updateParameters(void){};

    void run(void) override{ tracking::t_run_called = true;};
    QString documentation(void) override {return "empty";};

    void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage) override {
        tracking::t_set_cell_image_called = true;
    }

    void setImage(std::shared_ptr<gnomonImageSeries> image) override {
        tracking::t_set_image_called = true;
    }

    std::shared_ptr<gnomonCellImageSeries> cellImage() const override {
        return nullptr;
    }

    std::shared_ptr<gnomonTreeSeries> tree() const override {
        return nullptr;
    }
    void clearInputs(void) override {};
    void clearOutputs(void) override {};
};

inline gnomonAbstractCellImageTracking* dummyCellImageTrackingPluginCreator(void)  {
    return new dummyCellImageTrackingPlugin();
}

class gnomonCellImageTrackingCommandTestCasePrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image_series;
    std::shared_ptr<gnomonCellImageSeries> cell_image_series;
    gnomonCellImageTrackingCommand *tracking_command = nullptr;
};

gnomonCellImageTrackingCommandTestCase::gnomonCellImageTrackingCommandTestCase(void):d(new gnomonCellImageTrackingCommandTestCasePrivate) {

}

gnomonCellImageTrackingCommandTestCase::~gnomonCellImageTrackingCommandTestCase(void) {
    delete d;
}

void gnomonCellImageTrackingCommandTestCase::initTestCase(void) {
    dtkScriptInterpreterPython::instance()->init();
    gnomonCore::cellImageTracking::pluginFactory().record("dummyCellImageTracking", dummyCellImageTrackingPluginCreator);
}

void gnomonCellImageTrackingCommandTestCase::init(void) {
    d->tracking_command = new gnomonCellImageTrackingCommand();
    QVERIFY(d->tracking_command);
    d->tracking_command->setAlgorithmName("dummyCellImageTracking");
}

void gnomonCellImageTrackingCommandTestCase::redo(void) {

    auto img_series = std::make_shared<gnomonImageSeries>();
    d->tracking_command->setImage(img_series);
    QVERIFY(!tracking::t_set_image_called); //empty time series so not called

    auto cellimg_series = std::make_shared<gnomonCellImageSeries>();
    auto cellimg = std::make_shared<gnomonCellImage>();
    cellimg_series->insert(0, cellimg);
    d->tracking_command->setCellImage(cellimg_series);
    QVERIFY(tracking::t_set_cell_image_called);

    d->tracking_command->setNoAsync();
    d->tracking_command->redo();
    QVERIFY(tracking::t_run_called);
}

void gnomonCellImageTrackingCommandTestCase::undo(void) {
    d->tracking_command->undo();
    QVERIFY(tracking::t_set_image_called && tracking::t_set_cell_image_called);

}

void gnomonCellImageTrackingCommandTestCase::cleanupTestCase(void) {
    delete d->tracking_command;
    d->tracking_command = nullptr;
}

void gnomonCellImageTrackingCommandTestCase::cleanup(void) {

}

GNOMONTEST_MAIN_NOGUI(gnomonCellImageTrackingCommandTest, gnomonCellImageTrackingCommandTestCase);