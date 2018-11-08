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




        



    
