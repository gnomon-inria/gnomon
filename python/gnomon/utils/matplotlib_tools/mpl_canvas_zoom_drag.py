class MplCanvasZoomDrag(object):

    def __init__(self, figure):
        super().__init__()

        self.figure = figure
        self.shift = False
        self.drag = False

        self.drag_initial_point = None

        self.connects = {}

    def __del__(self):
        self.clear()

    def connect(self):
        self.connects['press'] = self.figure.canvas.mpl_connect('button_press_event', self.press)
        self.connects['move'] = self.figure.canvas.mpl_connect('motion_notify_event', self.move)
        self.connects['release'] = self.figure.canvas.mpl_connect('button_release_event', self.release)
        self.connects['key_press'] = self.figure.canvas.mpl_connect('key_press_event', self.on_key_press)
        self.connects['key_release'] = self.figure.canvas.mpl_connect('key_release_event', self.on_key_release)
        self.connects['scroll'] = self.figure.canvas.mpl_connect('scroll_event', self.scroll)

    def clear(self):
        for c in self.connects.keys():
            self.figure.canvas.mpl_disconnect(self.connects[c])
        self.connects = {}
        self.refresh()

    def refresh(self):
        self.figure.canvas.draw()

    def press(self, event):
        if self.shift:
            self.drag = True
            self.drag_initial_point = (event.xdata, event.ydata)
        self.refresh()

    def move(self, event):
        if self.drag:
            point = (event.xdata, event.ydata)
            xlim = list(self.figure.gca().get_xlim())
            xlim[0] -= point[0] - self.drag_initial_point[0]
            xlim[1] -= point[0] - self.drag_initial_point[0]
            ylim = list(self.figure.gca().get_ylim())
            ylim[0] -= point[1] - self.drag_initial_point[1]
            ylim[1] -= point[1] - self.drag_initial_point[1]
            self.figure.gca().set_xlim(*xlim)
            self.figure.gca().set_ylim(*ylim)
        self.refresh()

    def release(self, event):
        self.drag = False
        self.drag_initial_point = None
        self.refresh()

    def on_key_press(self, event):
        if event.key == 'shift':
            self.shift = True

    def on_key_release(self, event):
        if event.key == 'shift':
            self.shift = False

    def scroll(self, event):
        step = event.step
        point = [event.xdata, event.ydata]
        xlim = list(self.figure.gca().get_xlim())
        point_x = (point[0] - xlim[0]) / (xlim[1] - xlim[0])
        x_size = xlim[1]-xlim[0]
        xlim[0] += step * point_x * x_size/100.
        xlim[1] -= step * (1-point_x) * x_size/100.
        self.figure.gca().set_xlim(*xlim)
        ylim = list(self.figure.gca().get_ylim())
        point_y = (point[1] - ylim[0]) / (ylim[1] - ylim[0])
        y_size = ylim[1]-ylim[0]
        ylim[0] += step * point_y * y_size/100.
        ylim[1] -= step * (1-point_y) * y_size/100.
        self.figure.gca().set_ylim(*ylim)
        self.refresh()
