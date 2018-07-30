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

import numpy as np

from gnomoncore import gnomonTime
from gnomoncore import gnomonAbstractSystemScenario, gnomonSystem
from gnomoncore import gnomonAbstractModel
from gnomoncore import gnomonAbstractForm, gnomonSphereForm


class gnomonWallForm(gnomonAbstractForm):
    def __init__(self):
        self.x = 0

    def name(self):
        return "WallForm"


class gnomonWallMotionModel(gnomonAbstractModel):
    def __init__(self):
        self.wall = None
        self.wall_x_0 = 2.
        self.wall_x_1 = 4.
        self.time_1 = 10

    def setWall(self, wall):
        self.wall = wall

    def run(self, time, dt):
        if(time < self.time_1):
            self.wall.x = self.wall_x_0
        else:
            self.wall.x = self.wall_x_1


class gnomonSphereExpansionModel(gnomonAbstractModel):
    def __init__(self):
        self.sphere = None
        self.wall = None
        self.growth_rate = 1.
        self.growth_rate_decay = 0.99

    def setSphere(self, sphere):
        self.sphere = sphere

    def setWall(self, wall):
        self.wall = wall

    def run(self, time, dt):
        self.growth_rate *= self.growth_rate_decay

        self.sphere.setRadius(self.sphere.radius() + self.growth_rate*dt)
        if(abs(self.sphere.center()[0] - self.wall.x) <= self.sphere.radius() ):
            new_center = self.sphere.center()
            if(self.sphere.center()[0] > self.wall.x):
                new_center[0] = self.wall.x + self.sphere.radius()
            else:
                new_center[0] = self.wall.x - self.sphere.radius()
            self.sphere.setCenter(new_center)


class sphereExpansionScenario(gnomonAbstractSystemScenario):
    def __init__(self, sphere=None, wall=None):
        super(sphereExpansionScenario, self).__init__()
        self.sphere = sphere
        self.wall = wall

        self.wall_model = gnomonWallMotionModel()
        self.wall_model.setWall(self.wall)

        self.sphere_model = gnomonSphereExpansionModel()
        self.sphere_model.setWall(self.wall)
        self.sphere_model.setSphere(self.sphere)

    def run(self, time, dt):
        self.wall_model.run(time,dt)
        self.sphere_model.run(time,dt)


class TestModelSystemScenario(unittest.TestCase):

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
        assert abs(self.wall.x - 4.) < eps


