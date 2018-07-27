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
from gnomoncore import *

class sphereExpansionScenario(gnomonAbstractSystemScenario):
    def __init__(self, sphere=None, wall=None):
        super(sphereExpansionScenario, self).__init__()
        self.sphere = sphere
        self.wall = wall
        self.wall_pos0 = vec3_t([2., 0., 0.])
        self.wall_pos1 = vec3_t([4., 0., 0.])
        self.growth_rate = 1.
        self.growth_rate_decay = 0.99
        self.time_1 = 10

    def run(self, time, dt):
        if(time < self.time_1):
            self.wall.setPosition(self.wall_pos0)
        else:
            self.wall.setPosition(self.wall_pos1)

        self.growth_rate *= self.growth_rate_decay

        self.sphere.setRadius(self.sphere.radius() + self.growth_rate*dt)
        if(abs(self.sphere.center()[0] - self.wall.position()[0]) <= self.sphere.radius() ):
            new_center = self.sphere.center()
            if(self.sphere.center()[0] > self.wall.position()[0]):
                new_center[0] = self.wall.position()[0] + self.sphere.radius()
            else:
                new_center[0] = self.wall.position()[0] - self.sphere.radius()

            self.sphere.setCenter(new_center)

class TestSystemScenario(unittest.TestCase):

    def setUp(self):
        self.sphere = gnomonSphereForm()
        self.sphere.setRadius(1)
        self.wall = gnomonWallForm()
        self.system_scenario = sphereExpansionScenario(self.sphere, self.wall)

    def tearDown(self):
        pass

    def test_gnomonSphereExpansion(self):
        system = gnomonSystem(self.system_scenario)
        dt = 1
        for t in xrange(0, 20):
            system.run(t, dt)

        eps = 1e-4
        assert abs(self.sphere.radius() - 19.0272) < eps
        assert abs(self.sphere.center()[0] + 15.0272) < eps
        assert abs(self.wall.position()[0] - 4.) < eps
