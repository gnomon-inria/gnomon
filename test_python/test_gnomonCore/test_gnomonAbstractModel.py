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
import sys
from PyQt5.QtCore import QSettings

settings = QSettings(QSettings.IniFormat,QSettings.UserScope,"inria","dtk-script")
settings.beginGroup("modules");
paths = settings.value("path")
settings.endGroup()

for path in paths.split(":"):
    sys.path.append(path)
    
from gnomoncore import gnomonAbstractModel, gnomonAbstractForm
from gnomoncore import gnomonTime


class Sphere(gnomonAbstractForm):

    def __init__(self,radius=1,center=[0,0,0]):
        super(gnomonAbstractForm, self).__init__()
        self._radius = radius
        self._center = center

    @property
    def radius(self):
        return self._radius

    @radius.setter
    def set_radius(self, radius):
        assert(radius>0)
        self._radius = radius

    @property
    def center(self):
        return self._center

    @radius.setter
    def set_center(self, center):
        assert(len(center)==3)
        self._center = center


class SphereGrowthModel(gnomonAbstractModel):

    def __init__(self):
        # super(gnomonAbstractModel, self).__init__()

        self.sphere = None
        # self.dynamic_form = None

        self.time = gnomonTime(0,gnomonTime.ArbitraryTime)
        self.dt = gnomonTime(1,gnomonTime.ArbitraryTime)

        self.growth_rate = 1.

        self.next_sphere = None
        # self.next_dynamic_form = None
        self.next_time = None

    # def setDynamicForm(self, dynamic_form):
    #     self.dynamic_form = dynamic_form

    def setForm(self, form):
        self.sphere = form

    def setTime(self, time):
        self.time = time

    def setDeltaT(self, dt):
        self.dt = dt

    def setGrowthRateParameter(self, growth_rate):
        self.growth_rate = growth_rate

    def nextForm(self):
        return self.next_sphere

    def nextTime(self):
        return self.next_time

    def step(self):
        sphere = self.sphere
        # sphere = self.dynamic_form.atTime(self.time)
        next_radius = sphere.radius+self.growth_rate*self.dt.getTimeStamp()
        next_center = sphere.center
        next_sphere = Sphere(next_radius,next_center)

        self.next_sphere = next_sphere
        # self.next_dynamic_form = self.dynamic_form
        # self.next_dynamic_form.appendForm(next_sphere,self.time+self.dt)
        # self.next_time = self.time+self.dt
        self.next_time = gnomonTime(self.time.getTimeStamp()+self.dt.getTimeStamp())


class TestSphereModel(unittest.TestCase):

    def setUp(self):
        initial_sphere = Sphere(1)

        self.model = SphereGrowthModel()
        self.model.setForm(initial_sphere)
        self.model.setTime(gnomonTime(1))
        self.model.setDeltaT(gnomonTime(1))
        self.model.setGrowthRateParameter(1)

    def tearDown(self):
        pass

    def test_gnomonAbstractModel_step(self):
        self.model.step()
        assert self.model.nextTime().getTimeStamp() == 2
        assert self.model.nextForm().radius == 2

    def test_gnomonAbstractModel_loop(self):
        self.model.step()
        for step in xrange(8):
            self.model.setForm(self.model.nextForm())
            self.model.setTime(self.model.nextTime())
            self.model.step()

        assert self.model.nextTime().getTimeStamp() == 10
        assert self.model.nextForm().radius == 10
