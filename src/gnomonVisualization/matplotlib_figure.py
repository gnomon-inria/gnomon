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
    from gnomon_utils.gnomonMplTabWidget import all_widgets
except:
    print("Could not import mpl view class", sys.exc_info())
else:
    figure = plt.figure(num)
    widget = all_widgets[num]
    print(widget)

    setupMatplotlib(sip.unwrapinstance(widget),num)

#
# matplotlib_figure.py ends here
