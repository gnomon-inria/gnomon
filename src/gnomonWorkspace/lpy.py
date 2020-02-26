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

from openalea.lpy.gui.lpystudio import LPyWindow, LpyPlotter, Viewer
from openalea.lpy.gui.lpycodeeditor import LpyCodeEditor
from openalea.lpy.gui.lpyview3d import LpyView3D

from dtkthemes import dtkThemesEngine

base1 = QColor(dtkThemesEngine.instance().value("@base1"))

Viewer.show()
Viewer.frameGL.setBgColor(base1.red(), base1.green(), base1.blue())

workspace = LPyWindow()

axiomviewer = LpyView3D(workspace)
axiomviewer.setObjectName('LPYAxiomViewer')
axiomviewer.show()

workspace.frame.setObjectName("LPYCodeEditor")

workspace.codeeditor.setStyleSheet(
    ""
    "background-color: " + dtkThemesEngine.instance().value("@base1") + ";"
    "color: " + dtkThemesEngine.instance().value("@fg") + ";")

workspace.shellwidget.setObjectName("LPYShell")
workspace.shellwidget.setStyleSheet(
    ""
    "background-color: " + dtkThemesEngine.instance().value("@base1") + ";"
    "color: " + dtkThemesEngine.instance().value("@fg") + ";")

workspace.shellwidget._display_banner = False
workspace.shellwidget.style_sheet  = ""
workspace.shellwidget.style_sheet += ".error { color: orange; }"
workspace.shellwidget.style_sheet += ".in-prompt { color: lightgreen; }"
workspace.shellwidget.style_sheet += ".in-prompt-number { color: lightgreen; font-weight: bold; }"
workspace.shellwidget.style_sheet += ".out-prompt { color: orange; }"
workspace.shellwidget.style_sheet += ".out-prompt-number { color: orange; font-weight: bold; }"

workspace.debugDock.setObjectName("LPYDebug")
# workspace.parameterDock.toggleViewAction().toggle()
# workspace.parameterDock.setObjectName("LPYParameters")
workspace.scalarDock.setObjectName("LPYScalars")
workspace.materialDock.setObjectName("LPYMaterials")
workspace.setObjectName("LPYMainWindow")
workspace.show()
workspace.hide()

# dtkThemesEngine.instance().changed.connect(lambda: print("Oh yeah"))

#
# lpy.py ends here

# ####################################################################

# def retrieve_widgets(parentwidget):
#     in_widgets = [workspace.codeeditor, LpyPlotter(parentwidget)]
#     # LpyPlotter is a simple class to display 3D scene.

#     # type of qt objects are indicated by their name action, menu, Dock, widget

#     workspace.menuView.removeAction(workspace.actionView3D)
#     in_layoutmenu = [workspace.menuEdit, workspace.menuView]

#     out_widgets = [ QWidget.find(Viewer.winId()), workspace.shellwidget, workspace.debugDock]
#     # In theory, Viewer can be retrieve from its winId. Does not seems to work in Python.

#     param_widgets = [workspace.parameterDock,workspace.scalarDock, workspace.materialDock, ]

#     workspace.LsystemBar.removeAction(workspace.actionView3D)
#     run_actions = [action for action in workspace.LsystemBar.actions()]

#     workspace.menuFile.removeAction(workspace.actionExit)
#     leftbar_actions = [workspace.menuFile, workspace.menuHelp]

#     return in_widgets, in_layoutmenu, out_widgets, param_widgets, run_actions, leftbar_actions

# def get_codeeditor_theme(propertyname):
#     # Current version do not have lpykeyword and pykeyword yet.
#     assert propertyname in ['lpykeyword', 'pykeyword', 'prod', 'delimiter', 'func', 'string', 'tab','space','number','comment']
#     syntaxhighlighter = codeeditor.syntaxhighlighter
#     return getattr(syntaxhighlighter, propertyname+'Format')
