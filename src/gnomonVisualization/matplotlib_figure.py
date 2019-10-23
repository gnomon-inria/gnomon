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
    # from gnomonplugins.treegeneration.visualization.utils.gnomonMatplotlib.gnomonMplTabWidget import gnomonMplTabWidget
    # from gnomonplugins.treegeneration.visualization.utils.gnomonMatplotlib.gnomonMplTabWidget import MplCanvas, MplFigureTabWidget
    from gnomonplugins.treegeneration.visualization.utils.gnomonMatplotlib.gnomonMplTabWidget import all_widgets
    #from gnomonplugins.treegeneration.visualization.utils.gnomonMatplotlib.gnomonMplView import gnomonMplView
except:
    print("Could not import mpl view class", sys.exc_info())
else:

    # from PyQt5.QtWidgets import qApp, QMainWindow
    # from PyQt5.QtGui import *
    # from PyQt5.QtCore import *

    # mpl_tabs = gnomonMplTabWidget()
    # mpl_tabs = gnomonMplView()

    print(num)

    figure = plt.figure(num)
    widget = all_widgets[num]
    print(widget)

    setupMatplotlib(sip.unwrapinstance(widget),num)

#
# matplotlib_figure.py ends here
