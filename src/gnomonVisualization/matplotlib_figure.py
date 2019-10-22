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
import sys

import gnomoncore

from gnomonvisualization import setupMatplotlib

import matplotlib.pyplot as plt

try:
    from gnomonplugins.treegeneration.visualization.utils.gnomonMatplotlib.gnomonMplTabWidget import gnomonMplTabWidget
except:
    print("Could not import mpl view class", sys.exc_info())
else:

    from PyQt5.QtWidgets import qApp, QMainWindow
    from PyQt5.QtGui import *
    from PyQt5.QtCore import *

    mpl_tabs = gnomonMplTabWidget()

    setupMatplotlib(sip.unwrapinstance(mpl_tabs))

#
# matplotlib_figure.py ends here
