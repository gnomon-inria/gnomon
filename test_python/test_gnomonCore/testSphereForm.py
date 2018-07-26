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


class TestSphereFrom(unittest.TestCase):

    def setUp(self):
        self.sphere = gnomonSphereForm()
        self.sphere.setRadius(1)

    def tearDown(self):
        pass

    def test_gnomonSphereForm_radius(self):
        assert self.sphere.radius() == 1

        self.sphere.setRadius(2)
        assert self.sphere.radius() == 2




        



    
