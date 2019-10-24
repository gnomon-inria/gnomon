#!/usr/bin/env python3

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

import sip
import gnomoncore

from gnomonwidgets import *
from gnomonworkspace import *

from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from openalea.plantscan3d import main_window

workspace = main_window.MainWindow()
workspace.setObjectName("PS3DMainWindow")
workspace.show();

# setupPS3D(sip.unwrapinstance(workspace))

#
# ps3D.py ends here
