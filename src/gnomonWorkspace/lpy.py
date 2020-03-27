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
import openalea.lpy.gui.settings as lpysettings

from dtkthemes import dtkThemesEngine
from dtkthemes import dtkThemesEngineCallBack

## #############################################################################
##
## #############################################################################

base1 = QColor(dtkThemesEngine.instance().value("@base1"))

## #############################################################################
##
## #############################################################################
def getSettings():
    settings = QSettings(QSettings.IniFormat, QSettings.UserScope,'Gnomon','LPy'+str(LPY_VERSION_MAJOR))
    return settings

lpysettings.getSettings = getSettings

Viewer.show()

old_get_code = LpyCodeEditor.getCode

def getCode(self):
    code = str(self.toPlainText()).encode('iso-8859-1','replace').decode('iso-8859-1')
    if hasattr(self,'axiom'):
        print("Setting new axiom :",self.axiom)
        result = ''
        for line in code.splitlines(True):
            l = line.strip()
            if len(l) > 0:
                firstword = l.split()[0]
                if firstword == 'Axiom:':
                    result += 'Axiom:'+self.axiom
                else:
                    result += line
            else:
                result += line
        # self.setAxiom(None)
        return result
    else:
        return code
LpyCodeEditor.getCode = getCode

def setAxiom(self,axiom=None):
    if axiom is not None:
        self.axiom = axiom
    else:
        if hasattr(self,'axiom'):
            del self.axiom
LpyCodeEditor.setAxiom = setAxiom

workspace = LPyWindow(withinterpreter=True)

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
        super(lpyThemesEngineCallBack, self).__init__()

    def register(self):
        dtkThemesEngine.instance().addCallBack(self)

    def execute(self):
        background_color = QColor(dtkThemesEngine.instance().value("@base1"))

        Viewer.frameGL.setBgColor(background_color.red(), background_color.green(), background_color.blue())
        # Viewer.frameGL.update()

        set_theme_to_code_editor()


cb1 = lpyThemesEngineCallBack()
cb1.register()

from gnomonvisualization import gnomonFormManager

class gnomonLpyView3D(LpyView3D):

    def __init__(self, parent):
        super().__init__(parent)

    def dragEnterEvent(self, event):
        if event.mimeData().hasText():
            event.accept()
        else:
            event.ignore()

    def dragLeaveEvent(self, event):
        event.accept()

    def dragMoveEvent(self, event):
        event.accept()

    def dropEvent(self, event):
        path = event.mimeData().text()
        if path[0] == ":":
            form_index = int(path[1:])
            form = gnomonFormManager.instance().get(form_index)
            print(form)

            if form.current().asLString():
                lstring = form.current().asLString()

                lstring.toString()

            event.accept()
        else:
            event.ignore()


axiomviewer = gnomonLpyView3D(workspace)
axiomviewer.setObjectName('LPYAxiomViewer')
axiomviewer.setAcceptDrops(True)

axiomviewer.show()

workspace.frame.setObjectName("LPYCodeEditor")

set_theme_to_code_editor()

workspace.shellwidget.setObjectName("LPYShell")
workspace.shellwidget.enable_calltips = False
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

set_theme_to_code_editor()

dtkThemesEngine.instance().apply()

#
# lpy.py ends here
