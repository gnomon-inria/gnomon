import logging

from qtpy import QtQml
from qtpy.QtCore import Qt, QObject, Slot, Signal, QEvent, QTimer, QUrl, qDebug, QRect, QPoint, QCoreApplication
from qtpy.QtQuick import QQuickItem, QQuickPaintedItem
from qtpy.QtGui import QColor, QPalette, QPainter, QBrush, QPixmap, QRegion, QKeyEvent, QTextCursor, QMouseEvent, QDragEnterEvent, QDragLeaveEvent, QDragMoveEvent
from qtpy.QtWidgets import QStyleOption, QStylePainter, QStyle, QWidget
from qtpy.QtCore import QIODevice, QFile, QSize

from .backend_qtquick_widget import InProcessJupyterWidget


AsciiToKeySymTable = [ None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None, # Tab is 9
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None,
                       " ", "!", "\"", "#",
                       "$", "%", "&", "'",
                       "(", ")", "*", "+",
                       ",", "-", ".", "/",
                       "0", "1", "2", "3", "4", "5", "6", "7",
                       "8", "9", ":", ";", "<", "=",
                       ">", "?", "at", "A", "B", "C", "D", "E", "F", "G",
                       "H", "I", "J", "K", "L", "M", "N", "O",
                       "P", "Q", "R", "S", "T", "U", "V", "W",
                       "X", "Y", "Z", "[",
                       "\\", "]", "^", "_",
                       "`", "a", "b", "c", "d", "e", "f", "g",
                       "h", "i", "j", "k", "l", "m", "n", "o",
                       "p", "q", "r", "s", "t", "u", "v", "w",
                       "x", "y", "z", "{", "|", "}", "~", None, # Delete is 127
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None, None, None, None, None, None, None, None,
                       None, None]


def ascii_to_key_sym(i):
    if i >= 0:
        return AsciiToKeySymTable[i]
    else:
        return None


class JupyterConsole(QQuickPaintedItem):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.widget = None

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update)
        self.timer.setInterval(1000/60.)
        self.timer.start()

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

    def mouseMoveEvent(self, event):
        logging.debug("mouseMoveEvent")
        self.routeMouseEvents(event)

    def mousePressEvent(self, event):
        logging.debug("mousePressEvent")
        self.routeMouseEvents(event)

    def mouseReleaseEvent(self, event):
        logging.debug("mouseReleaseEvent")
        self.routeMouseEvents(event)

    def mouseDoubleClickEvent(self, event):
        self.routeMouseEvents(event)

    # TODO: Support drag events
    def dragEnterEvent(self, event):
        event.accept()
        return
        self.widget._control.dragEnterEvent(event)

    def dragLeaveEvent(self, event):
        event.accept()
        return
        self.widget._control.dragLeaveEvent(event)

    def dragMoveEvent(self, event):
        event.accept()
        return
        self.widget._control.dragMoveEvent(event)

    def routeMouseEvents(self, event):
        logging.debug("routeMouseEvents")
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
            # TODO: Consider adding a MouseArea to catch scroll events
            event.accept()
            self.update()

    def routeKeyEvents(self, event):
        logging.debug("routeKeyEvents")
        if self.widget is not None:
            intercepted = self.widget.eventFilter(self.widget._control, event)
            if not intercepted:
                if event.type() == QEvent.KeyPress:
                    self.widget._control.keyPressEvent(event)
                elif event.type() == QEvent.KeyRelease:
                    self.widget._control.keyReleaseEvent(event)
        event.accept()
        self.update()

    def keyPressEvent(self, event):
        logging.debug("keyPressEvent")
        self.routeKeyEvents(event)

    def keyReleaseEvent(self, event):
        logging.debug("keyReleaseEvent")
        self.routeKeyEvents(event)

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
