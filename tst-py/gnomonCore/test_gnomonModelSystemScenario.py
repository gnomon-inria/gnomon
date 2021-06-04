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
# import sys
# from PyQt5.QtCore import QSettings

# settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","dtk-script")
# settings.beginGroup("modules");
# paths = settings.value("path")
# settings.endGroup()

# for path in paths.split(":"):
#     sys.path.append(path)

import numpy as np

from gnomoncore import gnomonTime #, gnomonDiscreteDynamicForm
from gnomoncore import gnomonAbstractSystemScenario, gnomonSystem
from gnomoncore import gnomonAbstractModel
from gnomoncore import gnomonAbstractForm, gnomonSphereForm


class gnomonWallForm(gnomonAbstractForm):
    def __init__(self):
        self.x = 0

    def name(self):
        return "WallForm"


class myWallEvolutionModel(gnomonAbstractModel):
    def __init__(self):
        self.wall = None
        self.parameters = {'wall_x_0':2., 'wall_x_1':4., 'time_1':10}

    def setWall(self, wall):
        self.wall = wall

    def step(self, time, dt):
        if(time < self.parameters['time_1']):
            self.wall.x = self.parameters['wall_x_0']
        else:
            self.wall.x = self.parameters['wall_x_1']


class mySphereEvolutionModel(gnomonAbstractModel):
    def __init__(self):
        self.sphere = None
        self.parameters = {'growth_rate':1., 'growth_rate_decay':0.99}

    def setSphere(self, sphere):
        self.sphere = sphere

    def step(self, time, dt):
        self.parameters['growth_rate'] *= self.parameters['growth_rate_decay']
        new_radius = self.sphere.radius() + self.parameters['growth_rate']*dt
        self.sphere.setRadius(new_radius)


class mySphereWallInteractionEvolutionModel(gnomonAbstractModel):
    def __init__(self):
        self.sphere = None
        self.wall = None
        self.parameters = {}

    def setSphere(self, sphere):
        self.sphere = sphere

    def setWall(self, wall):
        self.wall = wall

    def step(self, time, dt):
        if(abs(self.sphere.center()[0] - self.wall.x) <= self.sphere.radius() ):
            new_center = self.sphere.center()
            if(self.sphere.center()[0] > self.wall.x):
                new_center[0] = self.wall.x + self.sphere.radius()
            else:
                new_center[0] = self.wall.x - self.sphere.radius()
            self.sphere.setCenter(new_center)


class mySphereExpansionAgainstWallScenario(gnomonAbstractSystemScenario):
    def __init__(self, sphere=None, wall=None):
        super(mySphereExpansionAgainstWallScenario, self).__init__()
        self.sphere = sphere
        self.wall = wall

        self.wall_model = myWallEvolutionModel()
        self.wall_model.setWall(self.wall)

        self.sphere_model = mySphereEvolutionModel()
        self.sphere_model.setSphere(self.sphere)

        self.interaction_model = mySphereWallInteractionEvolutionModel()
        self.interaction_model.setSphere(self.sphere)
        self.interaction_model.setWall(self.wall)

    def step(self, time, dt):
        self.wall_model.step(time,dt)
        self.sphere_model.step(time,dt)
        self.interaction_model.step(time,dt)


class TestModelSystemScenario(unittest.TestCase):

    def setUp(self):
        self.sphere = gnomonSphereForm()
        self.sphere.setRadius(1)

        self.wall = gnomonWallForm()

        self.system_scenario = mySphereExpansionAgainstWallScenario(self.sphere, self.wall)

        self.dynamic_sphere = gnomonDiscreteDynamicForm()
        self.dynamic_sphere.insert(self.system_scenario.sphere,gnomonTime(0))

        # self.dynamic_wall = gnomonDiscreteDynamicForm()
        # self.dynamic_wall.insert(self.system_scenario.wall,gnomonTime(0))


    def tearDown(self):
        self.dynamic_sphere.this.disown()

    def test_gnomonSphereExpansion(self):
        system = gnomonSystem(self.system_scenario)
        dt = 1
        for t in range(0, 20):
            system.step(t, dt)
            self.dynamic_sphere.insert(self.system_scenario.sphere,gnomonTime(t+dt))
            # self.dynamic_wall.insert(self.system_scenario.wall,gnomonTime(t+dt))

        print(self.dynamic_sphere.availableTimes())

        eps = 1e-4
        assert abs(self.sphere.radius() - 19.0272) < eps
        assert abs(self.sphere.center()[0] + 15.0272) < eps
        assert abs(self.wall.x - 4.) < eps
