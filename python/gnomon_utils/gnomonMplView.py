# Version: $Id$
#
#

# Commentary:
#
# $ git clone git://github.com/matplotlib/matplotlib.git
# $ cd matplotlib
# $ python setup.py build
# ... [qt5agg: yes [installing, Qt: 5.9.1, PyQt: 5.9; PySide2 not found]]
# $ sudo python setup.py install
# ... [goes into e.g. /Library/Python/2.7/site-packages/]

# Change Log:
#
#

# Code:

import sys
import os
import matplotlib

matplotlib.use('Qt5Agg')

from PyQt5 import QtCore
from PyQt5 import QtGui
from PyQt5 import QtWidgets

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class gnomonMplView(QtWidgets.QWidget):

    def __init__(self, figure=None, width=10, height=8, dpi=100):
        super(gnomonMplView, self).__init__()

        if figure is None:  
            self.figure = Figure(figsize=(width, height), dpi=dpi)
        else:
            self.figure = figure
            self.figure.set_figwidth(width)
            self.figure.set_figheight(height)
            self.figure.set_dpi(dpi)

        self.setObjectName("Figure "+str(self.figure.number))
        self.canvas = None
        self.set_canvas()
        # self.thisown = False
        # dtkViewController.instance().insert(self)

    def set_canvas(self):
        self.canvas = FigureCanvas(self.figure)
        self.canvas.axes = self.figure.gca()

        self.canvas.__init__(self.figure)
        self.canvas.setParent(self)
        self.canvas.setSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        self.canvas.updateGeometry()

    def set_figure(self, figure):
        self.figure = figure
        self.setObjectName("Figure "+str(self.figure.number))
        self.set_canvas()

    def widget(self):
        return self.canvas


#
# matplotlib-integration.py ends here
