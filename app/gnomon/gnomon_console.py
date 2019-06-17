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

import gnomoncore

try:
    import qtconsole
except:
    print "Install Jupyter to get QtConsole as Python Interpreter"
else:
    from qtconsole.rich_jupyter_widget import RichJupyterWidget
    from qtconsole.inprocess import QtInProcessKernelManager

    from PyQt5.QtWidgets import *
    from PyQt5.QtGui import *

    kernel_manager = QtInProcessKernelManager()
    kernel_manager.start_kernel()

    kernel = kernel_manager.kernel
    kernel.gui = 'qt'

    kernel_client = kernel_manager.client()
    kernel_client.start_channels()

    console = RichJupyterWidget()
    console._display_banner = False
    console.style_sheet  = ""
    console.style_sheet += ".error { color: orange; }"
    console.style_sheet += ".in-prompt { color: lightgreen; }"
    console.style_sheet += ".in-prompt-number { color: lightgreen; font-weight: bold; }"
    console.style_sheet += ".out-prompt { color: orange; }"
    console.style_sheet += ".out-prompt-number { color: orange; font-weight: bold; }"
    console.font = QFont("Source Code Pro", 12)
    console.kernel_manager = kernel_manager
    console.kernel_client = kernel_client
    console.show()

import importlib

plugin_names = []

plugin_names += ['gnomonImageReader']
plugin_names += ['gnomonImageWriter']
plugin_names += ['gnomonImageFilter']
plugin_names += ['gnomonCellComplexData']
plugin_names += ['gnomonCellComplexReader']
plugin_names += ['gnomonCellComplexFromImage']
plugin_names += ['gnomonCellGraphData']
plugin_names += ['gnomonCellGraphFromImage']
plugin_names += ['gnomonCellImageData']
plugin_names += ['gnomonCellImageFromImage']

for plugin_name in plugin_names:
    try:
        importlib.import_module(plugin_name)
    except Exception as e:
        print e
        print "Error while loading ",plugin_name," Python plugin!"

#
# create_jupyter_console.py ends here
