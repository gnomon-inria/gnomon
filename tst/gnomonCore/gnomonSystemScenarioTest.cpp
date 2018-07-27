#include "gnomonSystemScenarioTest.h"

#include <gnomonCore>
#include <gnomonTest>

#include <dtkCore>
#include <dtkLog>

#include <gnomonCore/gnomonAbstractSystemScenario>
#include <gnomonCore/gnomonSphereForm>
#include <gnomonCore/gnomonWallForm>

class gnomonSphereExpansionScenario : public gnomonAbstractSystemScenario
{

public:
    void run(std::size_t time, std::size_t dt) override
    {
        if(time < time_1) {
            wall.setPosition(wall_pos0);
        } else {
            wall.setPosition(wall_pos1);
        }

        growth_rate *= growth_rate_decay;

        sphere.setRadius(sphere.radius() + growth_rate*dt);
        if(std::abs(sphere.center()[0] - wall.position()[0]) <= sphere.radius() ) {
            gnomon::vec3_t new_center =  sphere.center();
            new_center[0] = sphere.center()[0] > wall.position()[0] ? wall.position()[0] + sphere.radius() : wall.position()[0] - sphere.radius();
            sphere.setCenter(new_center);
        }
    }

public:
    gnomon::vec3_t wall_pos0 = {2., 0., 0.};
    gnomon::vec3_t wall_pos1 = {4., 0., 0.};
    std::size_t time_1 = 10;
    double growth_rate = 1.;
    double growth_rate_decay = 0.99;

    gnomonSphereForm sphere;
    gnomonWallForm wall;
};

void gnomonSystemScenarioTestCase::initTestCase(void)
{
    dtkLogger::instance().attachConsole();
    dtkLogger::instance().setLevel("trace");
}

void gnomonSystemScenarioTestCase::init(void)
{
}

void gnomonSystemScenarioTestCase::sphereExpansionClassTest(void)
{
    gnomonSphereExpansionScenario scenario;

    std::size_t time_max = 20;
    std::size_t dt = 1;
    gnomonSystem system(&scenario);
    for(std::size_t t = 0; t < time_max; t += dt) {
        system.run(t, dt);
    }

    double eps = 1e-4;
    QVERIFY(std::abs(scenario.sphere.radius() - 19.0272) < eps );
    QVERIFY(std::abs(scenario.sphere.center()[0] + 15.0272) < eps);
    QVERIFY(scenario.sphere.center()[1] == 0.);
    QVERIFY(scenario.sphere.center()[2] == 0.);

    QVERIFY(scenario.wall.position()[0] == scenario.wall_pos1[0]);
    QVERIFY(scenario.wall.position()[1] == scenario.wall_pos1[1]);
    QVERIFY(scenario.wall.position()[2] == scenario.wall_pos1[2]);
}

void gnomonSystemScenarioTestCase::sphereExpansionLambdaTest(void)
{
    gnomonSphereForm sphere;
    gnomonWallForm wall;

    gnomon::vec3_t wall_pos0 = {2., 0., 0.};
    gnomon::vec3_t wall_pos1 = {4., 0., 0.};
    std::size_t time_1 = 10;
    double growth_rate = 1.;
    double growth_rate_decay = 0.99;

    auto system_scenario_func = [&](std::size_t time, std::size_t dt) {
        if(time < time_1) {
            wall.setPosition(wall_pos0);
        } else {
            wall.setPosition(wall_pos1);
        }

        growth_rate *= growth_rate_decay;

        sphere.setRadius(sphere.radius() + growth_rate*dt);
        if(std::abs(sphere.center()[0] - wall.position()[0]) <= sphere.radius() ) {
            gnomon::vec3_t new_center =  sphere.center();
            new_center[0] = sphere.center()[0] > wall.position()[0] ? wall.position()[0] + sphere.radius() : wall.position()[0] - sphere.radius();
            sphere.setCenter(new_center);
        }
    };

    std::size_t time_max = 20;
    std::size_t dt = 1;
    gnomonSystem system(system_scenario_func);
    for(std::size_t t = 0; t < time_max; t += dt) {
        system.run(t, dt);
    }

    double eps = 1e-4;
    QVERIFY(std::abs(sphere.radius() - 19.0272) < eps );
    QVERIFY(std::abs(sphere.center()[0] + 15.0272) < eps);
    QVERIFY(sphere.center()[1] == 0.);
    QVERIFY(sphere.center()[2] == 0.);

    QVERIFY(wall.position()[0] == wall_pos1[0]);
    QVERIFY(wall.position()[1] == wall_pos1[1]);
    QVERIFY(wall.position()[2] == wall_pos1[2]);
}

void gnomonSystemScenarioTestCase::cleanup(void)
{

}

void gnomonSystemScenarioTestCase::cleanupTestCase(void)
{

}

GNOMONTEST_MAIN_NOGUI(gnomonSystemScenarioTest, gnomonSystemScenarioTestCase);
