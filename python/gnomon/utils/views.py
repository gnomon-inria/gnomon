import vtk
import matplotlib.pyplot as plt

from gnomon.visualization import gnomonAbstractView, gnomonVtkView, gnomonMplView
from gnomon.utils.matplotlib_tools.backend_qtquickagg import manager_instance, MplCanvasZoomDrag


class gnomonStandaloneVtkView(gnomonVtkView):
    """
    Standalone VtkView object to be used outside the Gnomon application

    """

    def __init__(self, parent=None, size=(1000, 1000), offscreen=False):
        super().__init__(parent)

        self._render_window = vtk.vtkRenderWindow()
        if offscreen:
            self._render_window.SetOffscreenRendering(True)
        self._render_window.SetSize(*size)

        self._render_window_interactor = vtk.vtkRenderWindowInteractor()
        self._render_window_interactor.SetRenderWindow(self._render_window)

        self.associate(self._render_window)

        self._render_window_interactor.Initialize()
        self._render_window_interactor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())

    def show(self):
        self._render_window_interactor.Start()


class gnomonStandaloneMplView(gnomonMplView):
    """
    Standalone MplView object to be used outside the Gnomon application

    """

    def __init__(self, parent=None, size=(1000, 1000)):
        super().__init__(parent, True)

        num = manager_instance.num
        figsize = [s/plt.rcParams['figure.dpi'] for s in size]
        self._figure = plt.figure(num, figsize=figsize)
        self.setFigureNumber(num)

        manager_instance._canvas[num] = self._figure.canvas
        manager_instance._figures[num] = self._figure
        manager_instance._connects[self.num] = MplCanvasZoomDrag(self._figure)
        manager_instance._connects[self.num].connect()
        manager_instance.num += 1

    def render(self):
        self._figure.canvas.draw()

    def clear(self):
        self._figure.clf()
        self._figure.canvas.draw()

    def saveScreenshot(self, filename):
        self._figure.savefig(filename)

    def show(self):
        self._figure.show()
