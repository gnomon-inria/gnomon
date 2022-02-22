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
        self.editor = None
        self.texture = None
        self.zoom_factor = 0.8

        self.setAcceptedMouseButtons(Qt.AllButtons)
        self.setFlag(QQuickItem.ItemAcceptsDrops, True)
        self.setFlag(QQuickItem.ItemHasContents, True)
        self.setFlag(QQuickItem.ItemIsFocusScope, True)
        self.setFlag(QQuickItem.ItemAcceptsInputMethod, True)

        self.initInterpreter()

    @Slot(QEvent)
    def focusInEvent(self, event):
        print("focusInEvent")
        self.forceActiveFocus()

    @Slot()
    def initInterpreter(self):
        print("initInterpreter")
        self.updateEditorSize()
        self.editor = InProcessJupyterWidget()
        self.editor.setObjectName("InProcessJupyterWidget")

        # Connect signals
        self.widthChanged.connect(self.updateEditorSize)
        self.heightChanged.connect(self.updateEditorSize)

    def mouseMoveEvent(self, event):
        print("mouseMoveEvent")
        self.routeMouseEvents(event)

    def mousePressEvent(self, event):
        print("mousePressEvent")
        self.routeMouseEvents(event)

    def mouseReleaseEvent(self, event):
        print("mouseReleaseEvent")
        self.routeMouseEvents(event)

    def mouseDoubleClickEvent(self, event):
        self.routeMouseEvents(event)

    # TODO: Support drag events
    def dragEnterEvent(self, event):
        event.accept()
        return
        self.editor._control.dragEnterEvent(event)

    def dragLeaveEvent(self, event):
        event.accept()
        return
        self.editor._control.dragLeaveEvent(event)

    def dragMoveEvent(self, event):
        event.accept()
        return
        self.editor._control.dragMoveEvent(event)

    def routeMouseEvents(self, event):
        print("routeMouseEvents")
        self.forceActiveFocus()
        if self.editor is not None:
            newEvent = QMouseEvent(event.type(), event.localPos(), event.button(), event.buttons(), event.modifiers())
            if newEvent.type() == QEvent.MouseMove:
                self.editor._control.mouseMoveEvent(newEvent)
            elif newEvent.type() == QEvent.MouseButtonPress:
                self.editor._control.mousePressEvent(newEvent)
            elif newEvent.type() == QEvent.MouseButtonRelease:
                self.editor._control.mouseReleaseEvent(newEvent)
            elif newEvent.type() == QEvent.MouseButtonDblClick:
                self.editor._control.mouseDoubleClickEvent(newEvent)
            event.accept()
            # TODO: Consider adding a MouseArea to catch scroll events
            self.update()

    def routeKeyEvents(self, event):
        print("routeKeyEvents")
        if self.editor is not None:
            newEvent = QKeyEvent(event.type(), event.key(), event.modifiers())
            if event.type() == QKeyEvent.KeyPress:
                if (len(event.text()) > 0):
                    ascii_key = ord(event.text())
                else:
                    ascii_key = 0
                keysym = ascii_to_key_sym(ascii_key)
                if keysym is not None:
                    curs = self.editor._control.textCursor()
                    curs.insertText(keysym)
                else:
                    QCoreApplication.postEvent(self.editor._control, newEvent)
            event.accept()
            self.update()

    def keyPressEvent(self, event):
        print("keyPressEvent")
        self.routeKeyEvents(event)

    def keyReleaseEvent(self, event):
        print("keyReleaseEvent")
        self.routeKeyEvents(event)

    def updateEditorSize(self):
        if self.editor is not None:
            self.editor.setGeometry(0, 0, int(self.width()/self.zoom_factor), int(self.height()/self.zoom_factor))

    def onCustomReplot(self):
        self.update()

    def paint(self, painter):
        print("paintEvent")
        if (self.editor is not None):
            print("paintEvent 2")
            self.setRenderTarget(QQuickPaintedItem.FramebufferObject)
            rect = QRect(0, 0, int(self.width()/self.zoom_factor), int(self.height()/self.zoom_factor))
            picture = QPixmap(rect.size())
            picture.fill(QColor(0, 0, 0, 255))
            #picture = self.editor.grab(QRect(QPoint(0, 0), QSize(-1, -1)))
            self.editor.render(picture, QPoint(), QRegion(rect))
            picture_resized = picture.scaled(int(self.width()), int(self.height()),
                                             aspectRatioMode=Qt.KeepAspectRatio,
                                             mode=Qt.SmoothTransformation)
            painter.drawPixmap(QPoint(0, 0), picture_resized)

    def getScreenshot(self, map):
        painter = QPainter(map)
        offset = 0
        block = self.editor._control.document().firstBlock()
        while (block.isValid()):
            r = self.editor._control.blockBoundingRect(block) # QRectF
            layout = block.layout()
            if (not block.isVisible()):
                offset = offset + r.height()
                block = block.next();
                continue
            else:
                layout.draw(painter, QPoint(0,offset))
            offset = offset + r.height();
            block = block.next();


QtQml.qmlRegisterType(JupyterConsole, "gnomon.Jupyter", 1, 0, "JupyterConsole")
