# -*- coding: utf-8 -*-
# -*- python -*-
#
#
#       OpenAlea.OALab: Multi-Paradigm GUI
#
#       Copyright 2014-2017 INRIA - CIRAD - INRA
#
#       File author(s):
#            Guillaume Cerutti <guillaume.cerutti@inria.fr>
#            Julien Diener <julien.diener@inria.fr>
#            Guillaume Baty <guillaume.baty@inria.fr>
#
#       File contributor(s):
#
#       Distributed under the Cecill-C License.
#       See accompanying file LICENSE.txt or copy at
#           http://www.cecill.info/licences/Licence_CeCILL-C_V1-en.html
#
#       OpenAlea WebSite : http://openalea.gforge.inria.fr
#
###############################################################################

from PyQt5 import QtGui, QtWidgets, QtCore

import matplotlib
matplotlib.use('Qt5Agg')

import matplotlib.pyplot as plt

from matplotlib.backends.backend_qt5agg import _BackendQT5Agg
from matplotlib.backends.backend_qt5 import _BackendQT5

from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.backend_bases import FigureManagerBase
from matplotlib._pylab_helpers import Gcf

# from matplotlib.backends.qt_compat import _getSaveFileName

import os

all_widgets = {}
figure_containers = []


class MplFigure(Figure):
    pass

class MplCanvas(FigureCanvasQTAgg):

    """Ultimately, this is a QWidget (as well as a FigureCanvasAgg, etc.)."""

    def __init__(self, parent=None):
        fig = MplFigure()
        FigureCanvasQTAgg.__init__(self, fig)
#         self.figure.add_axobserver(self._on_axes_changed)
#
#     def _on_axes_changed(self, *args):
#         self.draw()
#         self.draw_idle()


# class gnomonMplCanvasView(dtkViewWidget):
#     """Ultimately, this is a QWidget (as well as a FigureCanvasAgg, etc.)."""
#
#     def __init__(self, figure=None, width=10, height=8, dpi=100):
#         super(gnomonMplCanvasView, self).__init__()
#
#         if figure is None:
#             self.figure = Figure(figsize=(width, height), dpi=dpi)
#         else:
#             self.figure = figure
#             self.figure.set_figwidth(width)
#             self.figure.set_figheight(height)
#             self.figure.set_dpi(dpi)
#
#         self.setObjectName(self.figure.get_label())
#         # self.setObjectName("Figure "+str(self.figure.number))
#         self.canvas = None
#         self.set_canvas()
#
#     def set_canvas(self):
#         self.canvas = FigureCanvasQTAgg(self.figure)
#         self.canvas.axes = self.figure.gca()
#
#         # self.canvas.__init__(self.figure)
#         self.canvas.setParent(self)
#         self.canvas.setSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
#         self.canvas.updateGeometry()
#
#     def set_figure(self, figure):
#         self.figure = figure
#         self.setObjectName(self.figure.get_label())
#         # self.setObjectName("Figure "+str(self.figure.number))
#         self.set_canvas()
#
#     def widget(self):
#         return self.canvas



    # def __init__(self, parent=None):
    #     fig = MplFigure()
    #     FigureCanvasQTAgg.__init__(self, fig)
#         self.figure.add_axobserver(self._on_axes_changed)
#
#     def _on_axes_changed(self, *args):
#         self.draw()
#         self.draw_idle()


class FigureManagerQT(FigureManagerBase):
    """
    Public attributes

    canvas      : The FigureCanvas instance
    num         : The Figure number
    window      : The qt.QMainWindow
    """

    def __getattribute__(self, *args, **kwargs):
        return FigureManagerBase.__getattribute__(self, *args, **kwargs)

    def __init__(self, canvas, num):
        FigureManagerBase.__init__(self, canvas, num)
        self.canvas = canvas
        self.canvas.setFocusPolicy(QtCore.Qt.StrongFocus)

    def show(self):
        #print 'pylab.plot'
        pass


class MplFigureTabWidget(QtWidgets.QFrame):
    tabCreated = QtCore.pyqtSignal(bool)
    
    def __init__(self, canvas=None, num=None):
        QtWidgets.QFrame.__init__(self)

        c = self.palette().color(self.backgroundRole())
        self._default_color = str((c.red(), c.green(), c.blue()))

        num = 0 if num is None else num

        self.canvas = MplCanvas()
        self.manager = FigureManagerQT(self.canvas, num)

        # self.mpl_toolbar = NavigationToolbar2QT(self.canvas, None)
        # self.mpl_toolbar.setStyleSheet("background-color: rgb%s;" % self._default_color)
        # self.mpl_toolbar.hide()

        Gcf.figs[num] = self.manager
        all_widgets[num] = self

        self.setToolTip("Figure %d" % self.manager.num)

        self.setFrameShape(QtWidgets.QFrame.Box)
        self.setFrameShadow(QtWidgets.QFrame.Plain)
        self.setContentsMargins(1, 1, 1, 1)

        self._layout = QtWidgets.QVBoxLayout(self)
        self._layout.addWidget(self.canvas)
        self._layout.setContentsMargins(1, 1, 1, 1)


    def show_active(self):
        self.setFrameShape(QtWidgets.QFrame.Box)
        self.setStyleSheet("background-color: rgb(0, 150, 0);")

    def show_inactive(self):
        self.setStyleSheet("")
        self.setFrameShape(QtWidgets.QFrame.NoFrame)

    def hold(self, state=True):
        for axe in self.canvas.figure.axes:
            axe.hold(state)

    # def toolbar_actions(self):
        # return [['', '', action, 0] for action in self.mpl_toolbar.actions()]


class gnomonMplTabWidget(QtWidgets.QWidget):

    def __init__(self, parent=None):
        
        QtWidgets.QWidget.__init__(self, parent=parent)

        c = self.palette().color(self.backgroundRole())
        self._default_color = str((c.red(), c.green(), c.blue()))

        self.tabs = QtWidgets.QTabWidget()

        self._layout = QtWidgets.QVBoxLayout(self)
        self._layout.addWidget(self.tabs)

        # self.mpl_toolbar = NavigationToolbar2QT(gnomonMplCanvasView(), None)
        # self.mpl_toolbar.setStyleSheet("background-color: rgb%s;" % self._default_color)

        self.tabs.setTabsClosable(True)
        self.tabs.tabCloseRequested.connect(self.tab_closed)
        self.tabs.currentChanged.connect(self.activate_figure)

        self.tabs.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.tabs.currentChanged.connect(self.update_name)
        # self._create_actions()
        # self._create_connections()

        # add_drop_callback(self, 'pandas/dataframe', self.drop_dataframe)

        self.initialize()

        self.setObjectName("MplTabWidget")

    def update_name(self,tab_index):
        widget = self.tabs.widget(tab_index)
        if widget is not None:
            self.setObjectName("MplTabWidget Figure "+str(widget.manager.num))

    # def _create_actions(self):
    #     self.action_save_figure = QtGui.QAction(qicon("matplotlib_filesave.png"), 'Save Figure', self)

    # def _create_connections(self):
    #     self.action_save_figure.triggered.connect(self.save_figure)

    def refresh_tab_widgets(self):
        for num in all_widgets.keys():
            widget = all_widgets[num]
            figure_label = widget.canvas.figure.get_label()

            if len(figure_label) == 0:
                figure_label = "Figure "+str(num)
            else:
                figure_label += " ["+str(num)+"]"
            widget.setToolTip(figure_label)
            tab_num = self.tabs.addTab(widget, figure_label)
        self.tabs.setCurrentIndex(tab_num)

    def tab_closed(self, tab_index):
        widget = self.tabs.widget(tab_index)
        num = widget.manager.num
        self.tabs.removeTab(tab_index)
        del Gcf.figs[num]
        del widget.manager
        del widget.canvas
        del all_widgets[num]

    def activate_figure(self, tab_index):
        widget = self.tabs.widget(tab_index)

        # self.mpl_toolbar = NavigationToolbar2QT(widget.canvas, None)
        # widget.canvas.draw()

        # cid = widget.manager.canvas.mpl_connect('button_press_event', make_active)
        # widget.manager._cidgcf = cid
        # _pylab_helpers.Gcf.set_active(widget.manager)

    def initialize(self):
        figure_containers.append(self)

    def toolbars(self):
        toolbar = QtWidgets.QToolBar("Matplotlib Figures")
        # toolbar.addActions([self.action_save_figure])
        return [toolbar]

    def toolbar_actions(self):
        return []
        # return [self.action_save_figure]

    def save_figure(self, filename):
        widget = self.tabs.currentWidget()
        if widget:
            widget.canvas.figure.savefig(filename)


    # def toolbar_actions(self):
    #     #print self.mpl_toolbar
    #     return [['', '', action, 0] for action in self.mpl_toolbar.actions()]



@classmethod
def gnomon_new_figure_manager_given_figure(cls, num, figure):
    """
    Create a new figure manager instance for the given figure.
    """
    # canvas = gnomonMplCanvasView(figure).widget()
    canvas = MplCanvas(figure)
    widget = MplFigureTabWidget(canvas, num)
    #print all_widgets
    #print figure_containers
    # return FigureManagerQT(canvas, num)

    for f in figure_containers:
        f.refresh_tab_widgets()
    return widget.manager


@classmethod
def gnomon_draw_if_interactive(cls):
    """
    Is called after every pylab drawing command
    """
    # if matplotlib.is_interactive():


    figManager = Gcf.get_active()
    #print("Draw figures :",figManager)
    if figManager is not None:
        figManager.canvas.draw_idle()


def activate():
    plt.switch_backend('qt5agg')
    plt.ion()

    _BackendQT5.draw_if_interactive = gnomon_draw_if_interactive
    _BackendQT5Agg.draw_if_interactive = gnomon_draw_if_interactive

    _BackendQT5.new_figure_manager_given_figure = gnomon_new_figure_manager_given_figure
    _BackendQT5Agg.new_figure_manager_given_figure = gnomon_new_figure_manager_given_figure

    _BackendQT5Agg.show = lambda: None

    #print("Gnomon matplotlib interaction activated!")

activate()
