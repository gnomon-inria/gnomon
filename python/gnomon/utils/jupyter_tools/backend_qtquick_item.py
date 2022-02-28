import logging

from qtpy import QtQml, QtGui
from qtpy.QtCore import Qt, Slot, QEvent, QRect, QPoint
from qtpy.QtQuick import QQuickItem, QQuickPaintedItem
from qtpy.QtGui import QPainter, QDragEnterEvent, QDragLeaveEvent, QDragMoveEvent, QDropEvent

from .backend_qtquick_widget import InProcessJupyterWidget


class JupyterConsole(QQuickPaintedItem):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.widget = None

        self.setAcceptedMouseButtons(Qt.AllButtons)
        self.setFlag(QQuickItem.ItemAcceptsDrops, True)
        self.setFlag(QQuickItem.ItemHasContents, True)
        self.setFlag(QQuickItem.ItemIsFocusScope, True)
        self.setFlag(QQuickItem.ItemAcceptsInputMethod, True)

        self.init_widget()

    @Slot(QEvent)
    def focusInEvent(self, event):
        logging.debug("focusInEvent")
        self.forceActiveFocus()

    @Slot()
    def init_widget(self):
        logging.debug("init_widget")
        self.update_widget_size()
        self.widget = InProcessJupyterWidget()
        self.widget.setObjectName("InProcessJupyterWidget")

        # Connect signals
        self.widthChanged.connect(self.update_widget_size)
        self.heightChanged.connect(self.update_widget_size)

    @Slot(str)
    def set_style_sheet(self, bg_color):
        self.widget.style_sheet = ""
        self.widget.style_sheet = """QPlainTextEdit, QTextEdit {{
    background-color: {0};
    background-clip: padding;
    color: #888888;
    selection-background-color: transparent;
}}
        """.format(bg_color)
        self.widget.style_sheet += ".error { color: orange; }"
        self.widget.style_sheet += ".in-prompt { color: lightgreen; }"
        self.widget.style_sheet += ".in-prompt-number { color: lightgreen; font-weight: bold; }"
        self.widget.style_sheet += ".out-prompt { color: orange; }"
        self.widget.style_sheet += ".out-prompt-number { color: orange; font-weight: bold; }"
        self.widget.font = QtGui.QFont("Source Code Pro", 11)
        self.update()

    def mouseMoveEvent(self, event):
        logging.debug("mouseMoveEvent")
        self.route_mouse_events(event)

    def mousePressEvent(self, event):
        logging.debug("mousePressEvent")
        self.route_mouse_events(event)

    def mouseReleaseEvent(self, event):
        logging.debug("mouseReleaseEvent")
        self.route_mouse_events(event)

    def mouseDoubleClickEvent(self, event):
        self.route_mouse_events(event)

    def wheelEvent(self, event):
        logging.debug("wheelEvent")
        if self.widget is not None:
            self.widget._control.wheelEvent(event)
            event.accept()
            self.update()

    def keyPressEvent(self, event):
        logging.debug("keyPressEvent")
        self.route_key_events(event)

    def keyReleaseEvent(self, event):
        logging.debug("keyReleaseEvent")
        self.route_key_events(event)

    def dragEnterEvent(self, event):
        self.route_drag_events(event)

    def dragLeaveEvent(self, event):
        self.route_drag_events(event)

    def dragMoveEvent(self, event):
        self.route_drag_events(event)

    def dropEvent(self, event):
        self.route_drag_events(event)

    def route_mouse_events(self, event):
        logging.debug("route_mouse_events")
        self.forceActiveFocus()
        if self.widget is not None:
            if event.type() == QEvent.MouseMove:
                self.widget._control.mouseMoveEvent(event)
            elif event.type() == QEvent.MouseButtonPress:
                self.widget._control.mousePressEvent(event)
            elif event.type() == QEvent.MouseButtonRelease:
                self.widget._control.mouseReleaseEvent(event)
            elif event.type() == QEvent.MouseButtonDblClick:
                self.widget._control.mouseDoubleClickEvent(event)
            event.accept()
            self.update()

    def route_key_events(self, event):
        logging.debug("route_key_events")
        if self.widget is not None:
            intercepted = self.widget.eventFilter(self.widget._control, event)
            if not intercepted:
                if event.type() == QEvent.KeyPress:
                    self.widget._control.keyPressEvent(event)
                elif event.type() == QEvent.KeyRelease:
                    self.widget._control.keyReleaseEvent(event)
        event.accept()
        self.update()
        
    def route_drag_events(self, event):
        logging.debug("route_drag_events")
        self.forceActiveFocus()
        if self.widget is not None:
            if isinstance(event, QDragEnterEvent):
                self.widget._control.dragEnterEvent(event)
            elif isinstance(event, QDragMoveEvent):
                self.widget._control.dragMoveEvent(event)
            elif isinstance(event, QDragLeaveEvent):
                self.widget._control.dragLeaveEvent(event)
            elif isinstance(event, QDropEvent):
                self.widget._control.dropEvent(event)
            event.accept()
            self.update()

    def update_widget_size(self):
        if self.widget is not None:
            self.widget.setGeometry(0, 0, int(self.width()), int(self.height()))

    def onCustomReplot(self):
        self.update()

    def paint(self, painter):
        logging.debug("paintEvent")
        if (self.widget is not None):
            rect = QRect(0, 0, int(self.width()), int(self.height()))
            self.widget.render(painter, QPoint(), rect)

    def getScreenshot(self, map):
        painter = QPainter(map)
        offset = 0
        block = self.widget._control.document().firstBlock()
        while (block.isValid()):
            r = self.widget._control.blockBoundingRect(block) # QRectF
            layout = block.layout()
            if (not block.isVisible()):
                offset = offset + r.height()
                block = block.next()
                continue
            else:
                layout.draw(painter, QPoint(0, offset))
            offset = offset + r.height()
            block = block.next()


QtQml.qmlRegisterType(JupyterConsole, "gnomon.Jupyter", 1, 0, "JupyterConsole")
