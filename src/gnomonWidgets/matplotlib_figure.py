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

import sys

import gnomoncore
from gnomonWidgets import *

import matplotlib.pyplot as plt

try:
    from Models.gnomonMplView import gnomonMplView
except:
    print "Could not import mpl view class", sys.exc_info()
else:

    from PyQt5.QtWidgets import qApp, QMainWindow
    from PyQt5.QtGui import *
    from PyQt5.QtCore import *

    mpl_view = gnomonMplView(plt.figure(0),4,10,dpi=40)

    toplevels = qApp.topLevelWidgets()

    for widget in toplevels:
        if isinstance(widget, QMainWindow):
            sublevels = widget.children()
            for subwidget in sublevels:
                if (subwidget.objectName() == "QuantificationWorkspace"):
                    print("replace terminal with matplotlib figure")
                    widget = gnomonWorkspaceCellImageQuantification.downcast(subwidget)
                    widget.addView(mpl_view.widget())


#
# matplotlib_figure.py ends here
