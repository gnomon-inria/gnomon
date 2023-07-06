import vtk
import matplotlib.pyplot as plt

from gnomon.visualization import gnomonAbstractView
from gnomon.visualization import gnomonAbstractVtkVisualization, gnomonAbstractMplVisualization
from gnomon.utils.matplotlib_tools.backend_qtquickagg import manager_instance


class gnomonLightVtkView(gnomonAbstractView):
    """
    Standalone view object mimicking the behaviour of a gnomonVtkView.

    """

    def __init__(self, parent=None):
        super().__init__(parent)

        self._renderer = vtk.vtkRenderer()

        self._render_window = vtk.vtkRenderWindow()
        self._render_window.AddRenderer(self._renderer)
        self._render_window.SetSize(1000, 1000)

        self._render_window_interactor = vtk.vtkRenderWindowInteractor()
        self._render_window_interactor.Initialize()
        self._render_window_interactor.SetRenderWindow(self._render_window)
        self._render_window_interactor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())

    def renderer3D(self):
        return self._renderer

    def renderer2D(self):
        return self._renderer

    def interactor(self):
        return self._render_window_interactor

    def setBounds(self, *args):
        self._renderer.ResetCamera()

    def currentTime(self):
        return 0

    def render(self):
        self._render_window_interactor.Render()

    def show(self):
        self._render_window_interactor.Start()

    def resetCamera(self):
        self._renderer.ResetCamera()

    def setCameraXY(self, flip=False, turn=False):
        cam = self._renderer.GetActiveCamera()
        x_min, x_max, y_min, y_max, z_min, z_max = self._renderer.ComputeVisiblePropBounds()
        cam.SetFocalPoint((x_min + x_max)/2, (y_min + y_max)/2, (z_min + z_max)/2)
        cam.SetPosition((x_min + x_max)/2, (y_min + y_max)/2, z_min if flip else z_max)
        cam.SetViewUp(0, -1 if turn else 1, 0)
        self.resetCamera()
        self.render()

    def setCameraXZ(self, flip=False, turn=False):
        cam = self._renderer.GetActiveCamera()
        x_min, x_max, y_min, y_max, z_min, z_max = self._renderer.ComputeVisiblePropBounds()
        cam.SetFocalPoint((x_min + x_max)/2, (y_min + y_max)/2, (z_min + z_max)/2)
        cam.SetPosition((x_min + x_max)/2, y_min if flip else y_max, (z_min + z_max)/2)
        cam.SetViewUp(0, 0, -1 if turn else 1)
        self.resetCamera()
        self.render()

    def setCameraYZ(self, flip=False, turn=False):
        cam = self._renderer.GetActiveCamera()
        x_min, x_max, y_min, y_max, z_min, z_max = self._renderer.ComputeVisiblePropBounds()
        cam.SetFocalPoint((x_min + x_max)/2, (y_min + y_max)/2, (z_min + z_max)/2)
        cam.SetPosition(x_min if flip else x_max, (y_min + y_max)/2, (z_min + z_max)/2)
        cam.SetViewUp(0, 0, -1 if turn else 1)
        self.resetCamera()
        self.render()

    def saveScreenshot(self, filename):
        render_window = vtk.vtkRenderWindow()
        render_window.AddRenderer(self._renderer)
        render_window.SetSize(1000, 1000)

        window_to_image_filter = vtk.vtkWindowToImageFilter()
        window_to_image_filter.SetInput(render_window)
        window_to_image_filter.SetInputBufferTypeToRGBA()
        window_to_image_filter.ReadFrontBufferOff()

        writer = None
        if filename.endswith(".png"):
            writer = vtk.vtkPNGWriter()
        if writer is not None:
            writer.SetFileName(filename)
            writer.SetInputConnection(window_to_image_filter.GetOutputPort())
            writer.Write()


class gnomonLightMplView(gnomonAbstractView):
    """
    Standalone view object mimicking the behaviour of a gnomonMplView.

    """

    def __init__(self, parent=None, figsize=(10, 10)):
        super().__init__(parent)

        num = manager_instance.num
        self._figure = plt.figure(num, figsize=figsize)
        print(f"Created figure {self._figure.number}")
        manager_instance._canvas[num]=self._figure.canvas
        manager_instance._figures[num]=self._figure
        manager_instance.num += 1

    def figureNumber(self):
        return self._figure.number

    def render(self):
        self._figure.canvas.draw()

    def clear(self):
        self._figure.clf()

    def saveScreenshot(self, filename):
        self._figure.savefig(filename)

    def show(self):
        self._figure.show()


def setView(visu, view):
    """
    Pass a standalone view to a visualization object so that it can display

    Parameters
    ----------
    visu: gnomonAbstractVisualization
        The visualization instance
    view: gnomonAbstractView
        The standalone view


    """
    if isinstance(view, gnomonLightVtkView):
        if isinstance(visu, gnomonAbstractVtkVisualization):
            visu.setView(view)

            def _view(*args, **kwargs):
                return view
            visu.vtkView = _view

    elif isinstance(view, gnomonLightMplView):
        if isinstance(visu, gnomonAbstractMplVisualization):
            visu.setView(view)

            def _view(*args, **kwargs):
                return view
            visu.mplView = _view

            def _figureNumber(*args, **kwargs):
                return view.figureNumber()
            visu.figureNumber = _figureNumber
