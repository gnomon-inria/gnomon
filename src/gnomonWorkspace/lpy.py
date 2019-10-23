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

from openalea.lpy.gui.lpystudio import LPyWindow, Viewer

workspace = LPyWindow()

setupLPY(sip.unwrapinstance(workspace))
setupLPY(sip.unwrapinstance(Viewer))

# 
# lpy.py ends here
