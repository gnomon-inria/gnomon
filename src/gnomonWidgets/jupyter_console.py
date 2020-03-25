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

from gnomonwidgets import setupConsole

try:
    import qtconsole
except:
    print("Install Jupyter to get QtConsole as Python Interpreter")
else:
    from qtconsole.rich_jupyter_widget import RichJupyterWidget
    from qtconsole.inprocess import QtInProcessKernelManager

    from PyQt5 import QtWidgets, QtGui, QtCore

    print("Try to replace terminal with jupyter console")

    kernel_manager = QtInProcessKernelManager()
    kernel_manager.start_kernel()

    kernel = kernel_manager.kernel
    kernel.gui = 'qt'

    kernel_client = kernel_manager.client()
    kernel_client.start_channels()

    console = RichJupyterWidget()
    console.enable_calltips = False
    console._display_banner = False
    console.style_sheet  = ""
    console.style_sheet += ".error { color: orange; }"
    console.style_sheet += ".in-prompt { color: lightgreen; }"
    console.style_sheet += ".in-prompt-number { color: lightgreen; font-weight: bold; }"
    console.style_sheet += ".out-prompt { color: orange; }"
    console.style_sheet += ".out-prompt-number { color: orange; font-weight: bold; }"
    console.font = QtGui.QFont("Source Code Pro", 12)
    console.kernel_manager = kernel_manager
    console.kernel_client = kernel_client

    setupConsole(sip.unwrapinstance(console))


#
# create_jupyter_console.py ends here
