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

    void run(void) override{ tracking::t_run_called = true;};
    QString documentation(void) override {return "empty";};

    void setCellImage(gnomonCellImageSeries *cellimage) override {
        tracking::t_set_cell_image_called = true;
    }

    void setImage(gnomonImageSeries *image) override {
        tracking::t_set_image_called = true;
    }

    gnomonCellImageSeries *cellImage() const override {
        return nullptr;
    }

    gnomonTreeSeries *tree() const override {
        return nullptr;
    }
};

inline gnomonAbstractCellImageTracking* dummyCellImageTrackingPluginCreator(void)  {
    return new dummyCellImageTrackingPlugin();
}

class gnomonCellImageTrackingCommandTestCasePrivate
{
public:
    gnomonImageSeries *image_series;
    gnomonCellImageSeries *cell_image_series;
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
    d->tracking_command->setAlgorithmName("dummyImageFilter");
}

void gnomonCellImageTrackingCommandTestCase::redo(void) {
    d->tracking_command->setImage(new gnomonImageSeries());
    d->tracking_command->setCellImage(new gnomonCellImageSeries());
    d->tracking_command->redo();

    QVERIFY(tracking::t_run_called && tracking::t_set_image_called && tracking::t_set_cell_image_called);
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