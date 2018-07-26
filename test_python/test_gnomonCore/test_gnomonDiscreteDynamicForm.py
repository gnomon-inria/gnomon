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
from gnomoncore import gnomonDiscreteDynamicForm, gnomonTime, gnomonAbstractForm, gnomonSphereForm


# class Sphere(gnomonAbstractForm):

#     def __init__(self,radius=1,center=[0,0,0]):
#         super(gnomonAbstractForm, self).__init__()
#         self._radius = radius
#         self._center = center

#     @property
#     def radius(self):
#         return self._radius

#     @radius.setter
#     def set_radius(self, radius):
#         assert(radius>0)
#         self._radius = radius

#     @property
#     def center(self):
#         return self._center

#     @radius.setter
#     def set_center(self, center):
#         assert(len(center)==3)
#         self._center = center



class TestDynamicSphere(unittest.TestCase):
    '''
    Tests the gnomonDiscreteDynamicForm class.
    '''

    def setUp(self):
        # self.sphere1 = Sphere(1)
        self.sphere1 = gnomonSphereForm()
        self.sphere1.setRadius(1)
        # self.sphere2 = Sphere(2)
        self.sphere2 = gnomonSphereForm()
        self.sphere2.setRadius(2)

        self.dynamic_form = gnomonDiscreteDynamicForm()

    def tearDown(self):
        pass

    def test_gnomonDiscreteDynamicForm_append(self):
        self.dynamic_form.insert(self.sphere1,gnomonTime(1))

    def test_gnomonDiscreteDynamicForm_atTime(self):
        self.dynamic_form.insert(self.sphere1,gnomonTime(1))
        assert self.dynamic_form.atTime(gnomonTime(1)) is not None
        # assert self.dynamic_form.atTime(gnomonTime(2)) is None
        # assert self.dynamic_form.atTime(gnomonTime(1)).radius == 1



#
# test_gnomonDiscreteDynamicForm.py ends here.