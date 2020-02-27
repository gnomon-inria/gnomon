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

from dtkthemes import dtkThemesEngine, dtkThemesEngineCallBack

# class lpyThemesEngineCallBack(dtkThemesEngineCallBack):
#     def __init__(self):
#         super(dtkThemesEngineCallBack, self).__init__()

#     # def setWorkspace(self, workspace):
#     #     self.workspace = workspace

#     def execute(self):
#         background_color = QColor(dtkThemesEngine.instance().value("@base1"))

#         Viewer.frameGL.setBgColor(background_color.red(), background_color.green(), background_color.blue())
#         Viewer.frameGL.update()

#         # self.workspace.codeeditor.setStyleSheet(
#         #     ""
#         #     "background-color: " + dtkThemesEngine.instance().value("@base1") + ";"
#         #     "color: " + dtkThemesEngine.instance().value("@fg") + ";")


base1 = QColor(dtkThemesEngine.instance().value("@base1"))

Viewer.show()
Viewer.frameGL.setBgColor(base1.red(), base1.green(), base1.blue())

#workspace = LPyWindow(withinterpreter=False) # To avoid redirection in the lpy shell
workspace = LPyWindow()

def set_theme_to_code_editor():
        workspace.codeeditor.setStyleSheet(
           ""
           "background-color: " + dtkThemesEngine.instance().value("@base1") + ";"
           "color: " + dtkThemesEngine.instance().value("@fg") + ";")

        syntaxhighlighter = workspace.codeeditor.syntaxhighlighter

        propertyname = ['lpykeyword', 'pykeyword', 'prod', 'delimiter', 'func', 'string', 'space','number','comment']
        propertycolor = ["@violet",  "@blue",     "@fg",   "@darkblue", "@magenta", "@grey", "@bdalt", "@red", "@green"]

        translation = { 'pykeyword' : 'keyword'}

        for i, (name, color) in enumerate(zip(propertyname, propertycolor)):
            name = translation.get(name,name)
            if hasattr(syntaxhighlighter, name+'Format'):
                oformat = getattr(syntaxhighlighter, name+'Format')

                if name != 'space':
                   oformat.setForeground(QColor(dtkThemesEngine.instance().value(color)))
                else:
                   oformat.setBackground(QColor(dtkThemesEngine.instance().value(color)))
            else:
                print('No format',name)

        # To update the syntax highlighting on all the document.
        workspace.codeeditor.syntaxhighlighter.setActivation(False)
        workspace.codeeditor.syntaxhighlighter.setActivation(True)

class lpyThemesEngineCallBack(dtkThemesEngineCallBack):
    def __init__(self):
        super(dtkThemesEngineCallBack, self).__init__()


    def execute(self):
        background_color = QColor(dtkThemesEngine.instance().value("@base1"))

        Viewer.frameGL.setBgColor(background_color.red(), background_color.green(), background_color.blue())
        Viewer.frameGL.update()

        set_theme_to_code_editor()


cb1 = lpyThemesEngineCallBack()
# cb1.setWorkspace(workspace)

axiomviewer = LpyView3D(workspace)
axiomviewer.setObjectName('LPYAxiomViewer')
axiomviewer.show()

workspace.frame.setObjectName("LPYCodeEditor")

set_theme_to_code_editor()

workspace.shellwidget.setObjectName("LPYShell")
workspace.shellwidget.setStyleSheet(
    ""
    "background-color: " + dtkThemesEngine.instance().value("@base1") + ";"
    "color: " + dtkThemesEngine.instance().value("@fg") + ";")

workspace.shellwidget._display_banner = False
workspace.shellwidget.style_sheet  = ""
workspace.shellwidget.style_sheet += ".error { color: orange; }"
workspace.shellwidget.style_sheet += ".in-prompt { color: " + dtkThemesEngine.instance().value("@green") + "; }"
workspace.shellwidget.style_sheet += ".in-prompt-number { color: " + dtkThemesEngine.instance().value("@green") + "; font-weight: bold; }"
workspace.shellwidget.style_sheet += ".out-prompt { color: " + dtkThemesEngine.instance().value("@red") + "; }"
workspace.shellwidget.style_sheet += ".out-prompt-number { color: " + dtkThemesEngine.instance().value("@red") + "; font-weight: bold; }"

workspace.debugDock.setObjectName("LPYDebug")
# workspace.parameterDock.toggleViewAction().toggle()# 

workspace.scalarDock.setObjectName("LPYScalars")
workspace.materialDock.setObjectName("LPYMaterials")
workspace.parametersDock.setObjectName("LPYParameters")
panels = workspace.panelmanager.getObjectPanels()
if len(panels) == 0:
    workspace.panelmanager.createNewPanel()
    panels = workspace.panelmanager.getObjectPanels()
panels[0].setObjectName("LPYCurves")
# workspace.viewer.setObjectName("PGLViewer")

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

# red = QColor(dtkThemesEngine.instance().value("@red"))
# ... Same goes for:
# @base0:
# @base1:
# @base2:
# @base3:
# @base4:
# @base5:
# @base6:
# @base7:
# @base8:
#
# @bg:
# @bgalt:
# @fg:
# @fgalt:
# @bd:
# @bdalt:
# @hl:
# @hlalt:
#
# @grey:
# @red:
# @orange:
# @green:
# @teal:
# @yellow:
# @blue:
# @darkblue:
# @magenta:
# @violet:
# @cyan:
# @darkcyan:


