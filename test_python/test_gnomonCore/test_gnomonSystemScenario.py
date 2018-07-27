# Version: $Id$
#
#

# Commentary:
#
#

# Change Log:
#
#

# Code:

import unittest
from gnomoncore import gnomonAbstractForm, gnomonSphereForm, gnomonWallForm, gnomonSystem, gnomonAbstractSystemScenario

class sphereExpansionScenario(gnomonAbstractSystemScenario):
    def run(self, time, dt):
        if(time < time_1):
            wall.setPosition(wall_pos0)
        else:
            wall.setPosition(wall_pos1)

        growth_rate *= growth_rate_decay

        sphere.setRadius(sphere.radius() + growth_rate*dt)
        if(abs(sphere.center()[0] - wall.position()[0]) <= sphere.radius() ):
            new_center = sphere.center()
            if(sphere.center()[0] > wall.position()[0]):
                new_center[0] = wall.position()[0] + sphere.radius()
            else:
                new_center[0] = wall.position()[0] - sphere.radius()

            sphere.setCenter(new_center)

class TestSystemScenario(unittest.TestCase):

    def setUp(self):
        self.sphere = gnomonSphereForm()
        self.sphere.setRadius(1)

        self.wall = gnomonWallForm()

        self.system_scenario_func = sphereExpansionScenario()


    def tearDown(self):
        pass

    def test_gnomonSphereForm_radius(self):
        wall_pos0 = [2., 0, 0]
        wall_pos1 = [4., 0, 0]
        growth_rate = 1.
        growth_rate_decay = 0.99

        system = gnomonSystem(self.system_scenario_func)
        dt = 1
        for t in xrange(0, 20):
            system.run(t, dt)

        eps = 1e-4
        assert abs(self.sphere.radius() - 19.0272) < eps
