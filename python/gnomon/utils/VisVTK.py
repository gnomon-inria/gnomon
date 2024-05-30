"""
.. module:: VisVTK
    :platform: Unix, Windows
    :synopsis: VTK visualization component for NURBS-Python

.. moduleauthor:: Onur Rauf Bingol <orbingol@gmail.com>
.. moduleauthor:: Karamoko Samassa <karamoko.samassa@inria.fr>

"""

from random import random
from geomdl import vis
import numpy as np
import vtk
from vtk.util.numpy_support import numpy_to_vtk
from vtk import VTK_FLOAT
from matplotlib import cm

def create_actor_pts(pts, color, **kwargs):
    """Creates a VTK actor for rendering scatter plots.

    :param pts: points
    :type pts: vtkFloatArray
    :param color: actor color
    :type color: list
    :return: a VTK actor
    :rtype: vtkActor
    """
    # Keyword arguments
    array_name = kwargs.get('name', "")
    array_index = kwargs.get('index', 0)
    point_size = kwargs.get('size', 0.05)

    sphere = vtk.vtkSphereSource()
    sphere.SetRadius(point_size)
    sphere.SetThetaResolution(12)
    sphere.SetPhiResolution(12)
    sphere.Update()

    # Create points
    points = vtk.vtkPoints()
    points.SetData(pts)

    # Create a PolyData object and add points
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)

    # Run glyph 3D on the points array
    glyph = vtk.vtkGlyph3D()
    glyph.SetInputData(polydata)
    glyph.SetSourceData(sphere.GetOutput())
    glyph.SetScaleModeToDataScalingOff()

    # Map polydata to the graphics primitives
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(glyph.GetOutputPort())
    mapper.SetArrayName(array_name)
    mapper.SetArrayId(array_index)

    # Create an actor and set its properties
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)

    # Return the actor
    return actor


def create_actor_polygon(pts, color, **kwargs):
    """ Creates a VTK actor for rendering polygons.

    :param pts: points
    :type pts: vtkFloatArray
    :param color: actor color
    :type color: list
    :return: a VTK actor
    :rtype: vtkActor
    """
    # Keyword arguments
    array_name = kwargs.get('name', "")
    array_index = kwargs.get('index', 0)
    line_width = kwargs.get('size', 1.0)

    # Create points
    points = vtk.vtkPoints()
    points.SetData(pts)

    # Number of points
    num_points = points.GetNumberOfPoints()

    # Create lines
    cells = vtk.vtkCellArray()
    for i in range(num_points - 1):
        line = vtk.vtkLine()
        line.GetPointIds().SetId(0, i)
        line.GetPointIds().SetId(1, i + 1)
        cells.InsertNextCell(line)

    # Create a PolyData object and add points & lines
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)
    polydata.SetLines(cells)

    # Map poly data to the graphics primitives
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputDataObject(polydata)
    mapper.SetArrayName(array_name)
    mapper.SetArrayId(array_index)

    # Create an actor and set its properties
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)
    actor.GetProperty().SetLineWidth(line_width)

    # Return the actor
    return actor

def create_color(color):
    """ Creates VTK-compatible RGB color from a color string.

    :param color: color
    :type color: str
    :return: RGB color values
    :rtype: list
    """
    if color[0] == "#":
        # Convert hex string to RGB
        return [int(color[i:i + 2], 16) / 255 for i in range(1, 7, 2)]
    else:
        # Create a named colors instance
        nc = vtk.vtkNamedColors()
        return nc.GetColor3d(color)

def create_actor_mesh(pts, lines, color, **kwargs):
    """ Creates a VTK actor for rendering quadrilateral plots.

    :param pts: points
    :type pts: vtkFloatArray
    :param lines: point connectivity information
    :type lines: vtkIntArray
    :param color: actor color
    :type color: list
    :return: a VTK actor
    :rtype: vtkActor
    """
    # Keyword arguments
    array_name = kwargs.get('name', "")
    array_index = kwargs.get('index', 0)
    line_width = kwargs.get('size', 0.5)

    # Create points
    points = vtk.vtkPoints()
    points.SetData(pts)

    # Create lines
    cells = vtk.vtkCellArray()
    for line in lines:
        pline = vtk.vtkPolyLine()
        pline.GetPointIds().SetNumberOfIds(5)
        for i in range(len(line)):
            pline.GetPointIds().SetId(i, line[i])
        pline.GetPointIds().SetId(4, line[0])
        cells.InsertNextCell(pline)

    # Create a PolyData object and add points & lines
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)
    polydata.SetLines(cells)

    # Map poly data to the graphics primitives
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputDataObject(polydata)
    mapper.SetArrayName(array_name)
    mapper.SetArrayId(array_index)

    # Create an actor and set its properties
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)
    actor.GetProperty().SetLineWidth(line_width)

    # Return the actor
    return actor

def create_actor_tri(pts, tris, color, **kwargs):
    """ Creates a VTK actor for rendering triangulated surface plots.

    :param pts: points
    :type pts: vtkFloatArray
    :param tris: list of triangle indices
    :type tris: ndarray
    :param color: actor color
    :type color: list
    :return: a VTK actor
    :rtype: vtkActor
    """
    # Keyword arguments
    array_name = kwargs.get('name', "")
    array_index = kwargs.get('index', 0)

    # Create points
    points = vtk.vtkPoints()
    points.SetData(pts)

    # Create triangles
    triangles = vtk.vtkCellArray()
    for tri in tris:
        tmp = vtk.vtkTriangle()
        for i, v in enumerate(tri):
            tmp.GetPointIds().SetId(i, v)
        triangles.InsertNextCell(tmp)

    # Create a PolyData object and add points & triangles
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)
    polydata.SetPolys(triangles)

    # Map poly data to the graphics primitives
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputDataObject(polydata)
    mapper.SetArrayName(array_name)
    mapper.SetArrayId(array_index)

    # Create an actor and set its properties
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)

    # Return the actor
    return actor

def create_actor_hexahedron(grid, color, **kwargs):
    """ Creates a VTK actor for rendering voxels using hexahedron elements.

    :param grid: grid
    :type grid: ndarray
    :param color: actor color
    :type color: list
    :return: a VTK actor
    :rtype: vtkActor
    """
    # Keyword arguments
    array_name = kwargs.get('name', "")
    array_index = kwargs.get('index', 0)

    # Create hexahedron elements
    points = vtk.vtkPoints()
    hexarray = vtk.vtkCellArray()
    for j, pt in enumerate(grid):
        tmp = vtk.vtkHexahedron()
        fb = pt[0]
        for i, v in enumerate(fb):
            points.InsertNextPoint(v)
            tmp.GetPointIds().SetId(i, i + (j * 8))
        ft = pt[-1]
        for i, v in enumerate(ft):
            points.InsertNextPoint(v)
            tmp.GetPointIds().SetId(i + 4, i + 4 + (j * 8))
        hexarray.InsertNextCell(tmp)

    # Create an unstructured grid object and add points & hexahedron elements
    ugrid = vtk.vtkUnstructuredGrid()
    ugrid.SetPoints(points)
    ugrid.SetCells(tmp.GetCellType(), hexarray)
    # ugrid.InsertNextCell(tmp.GetCellType(), tmp.GetPointIds())

    # Map unstructured grid to the graphics primitives
    mapper = vtk.vtkDataSetMapper()
    mapper.SetInputDataObject(ugrid)
    mapper.SetArrayName(array_name)
    mapper.SetArrayId(array_index)

    # Create an actor and set its properties
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)

    # Return the actor
    return actor

def create_render_window(render_window, vtk_actors, callbacks):
    figure_size = (800, 600)
    camera_position = (0, 0, 100)
    display_plot = True

    # Create renderer
    # renderer = vtk.vtkRenderer()
    renderer = render_window.GetRenderers().GetFirstRenderer()
    renderer.setBackground(1.0, 1.0, 1.0)

    # Add actors to the scene
    for actor in vtk_actors:
        renderer.AddActor(actor)

    # Render window
    render_window.AddRenderer(renderer)
    render_window.SetSize(*figure_size)
    render_window.SetOffScreenRendering(not display_plot)

    # Render actors
    render_window.Render()

    # Render window interactor
    window_interactor = vtk.vtkRenderWindowInteractor()
    window_interactor.SetRenderWindow(render_window)

    # Add event observers
    for cb in callbacks:
        window_interactor.AddObserver(cb, callbacks[cb][0], callbacks[cb][1])  # cb name, cb function ref, cb priority

    # Use trackball camera
    interactor_style = vtk.vtkInteractorStyleTrackballCamera()
    window_interactor.SetInteractorStyle(interactor_style)

    # Start interactor
    window_interactor.Start()
class VisConfig(vis.VisConfigAbstract):
    """ Configuration class for VTK visualization module.

    This class is only required when you would like to change the visual defaults of the plots and the figure.

    The ``VisVTK`` module has the following configuration variables:

    * ``ctrlpts`` (bool): Control points polygon/grid visibility. *Default: True*
    * ``evalpts`` (bool): Curve/surface points visibility. *Default: True*
    * ``trims`` (bool): Trim curve visibility. *Default: True*
    * ``trim_size`` (int): Size of the trim curves. *Default: 4*
    * ``figure_size`` (list): Size of the figure in (x, y). *Default: (800, 600)*
    * ``line_width`` (int): Thickness of the lines on the figure. *Default: 1.0*
    """
    def __init__(self, **kwargs):
        super(VisConfig, self).__init__(**kwargs)
        self._bg = (  # background colors
            (0.5, 0.5, 0.5), (0.2, 0.2, 0.2), (0.25, 0.5, 0.75), (1.0, 1.0, 0.0),
            (1.0, 0.5, 0.0), (0.5, 0.0, 1.0), (0.0, 0.0, 0.0), (1.0, 1.0, 1.0)
        )
        self._bg_id = 0  # used for keeping track of the background numbering
        self.display_ctrlpts = kwargs.get('ctrlpts', True)
        self.display_evalpts = kwargs.get('evalpts', True)
        self.display_bbox = kwargs.get('bbox', False)
        self.display_trims = kwargs.get('trims', True)
        self.trim_size = kwargs.get('trim_size', 4)
        self.figure_size = kwargs.get('figure_size', (800, 600))  # size of the render window
        self.line_width = kwargs.get('line_width', 1.0)
        self.figure_image_filename = "temp-figure.png"

    def keypress_callback(self, obj, ev):
        """ VTK callback for keypress events.

        Keypress events:
            * ``e``: exit the application
            * ``p``: pick object (hover the mouse and then press to pick)
            * ``f``: fly to point (click somewhere in the window and press to fly)
            * ``r``: reset the camera
            * ``s`` and ``w``: switch between solid and wireframe modes
            * ``b``: change background color
            * ``m``: change color of the picked object
            * ``d``: print debug information (of picked object, point, etc.)
            * ``h``: change object visibility
            * ``n``: reset object visibility
            * ``arrow keys``: pan the model

        Please refer to `vtkInteractorStyle <https://vtk.org/doc/nightly/html/classvtkInteractorStyle.html>`_ class
        reference for more details.

        :param obj: render window interactor
        :type obj: vtkRenderWindowInteractor
        :param ev: event name
        :type ev: str
        """
        key = obj.GetKeySym()  # pressed key (as str)
        render_window = obj.GetRenderWindow()  # vtkRenderWindow
        renderer = render_window.GetRenderers().GetFirstRenderer()  # vtkRenderer
        picker = obj.GetPicker()  # vtkPropPicker
        actor = picker.GetActor()  # vtkActor

        # Custom keypress events
        if key == 'Up':
            camera = renderer.GetActiveCamera()  # vtkCamera
            camera.Pitch(2.5)
        if key == 'Down':
            camera = renderer.GetActiveCamera()  # vtkCamera
            camera.Pitch(-2.5)
        if key == 'Left':
            camera = renderer.GetActiveCamera()  # vtkCamera
            camera.Yaw(-2.5)
        if key == 'Right':
            camera = renderer.GetActiveCamera()  # vtkCamera
            camera.Yaw(2.5)
        if key == 'b':
            if self._bg_id >= len(self._bg):
                self._bg_id = 0
            renderer.SetBackground(*self._bg[self._bg_id])
            self._bg_id += 1
        if key == 'm':
            if actor is not None:
                actor.GetProperty().SetColor(random(), random(), random())
        if key == 'd':
            if actor is not None:
                print("Name:", actor.GetMapper().GetArrayName())
                print("Index:", actor.GetMapper().GetArrayId())
            print("Selected point:", picker.GetSelectionPoint()[0:2])
            print("# of visible actors:", renderer.VisibleActorCount())
        if key == 'h':
            if actor is not None:
                actor.SetVisibility(not actor.GetVisibility())
        if key == 'n':
            actors = renderer.GetActors()  # vtkActorCollection
            for actor in actors:
                actor.VisibilityOn()

        # Update render window
        render_window.Render()


class VisCurve3D(vis.VisAbstract):
    """ VTK visualization module for curves. """
    def __init__(self, config=VisConfig(), **kwargs):
        super(VisCurve3D, self).__init__(config, **kwargs)
        self.vtk_actors = []

    def render(self, render_window, **kwargs):
        """ Plots the curve and the control points polygon. """
        # Calling parent function
        super(VisCurve3D, self).render(**kwargs)

        # Initialize a list to store VTK actors
        self.vtk_actors = []

        # Start plotting
        for plot in self._plots:
            # Plot control points
            if plot['type'] == 'ctrlpts' and self.vconf.display_ctrlpts:
                # Points as spheres
                pts = np.array(plot['ptsarr'], dtype=np.float)
                # Handle 2-dimensional data
                if pts.shape[1] == 2:
                    pts = np.c_[pts, np.zeros(pts.shape[0], dtype=np.float)]
                vtkpts = numpy_to_vtk(pts, deep=False, array_type=VTK_FLOAT)
                vtkpts.SetName(plot['name'])
                actor1 = create_actor_pts(pts=vtkpts, color=create_color(plot['color']),
                                               name=plot['name'], idx=plot['idx'])
                self.vtk_actors.append(actor1)
                # Lines
                actor2 = create_actor_polygon(pts=vtkpts, color=create_color(plot['color']),
                                                   name=plot['name'], index=plot['idx'], size=self.vconf.line_width)
                self.vtk_actors.append(actor2)

            # Plot evaluated points
            if plot['type'] == 'evalpts' and self.vconf.display_evalpts:
                pts = np.array(plot['ptsarr'], dtype=np.float)
                # Handle 2-dimensional data
                if pts.shape[1] == 2:
                    pts = np.c_[pts, np.zeros(pts.shape[0], dtype=np.float)]
                vtkpts = numpy_to_vtk(pts, deep=False, array_type=VTK_FLOAT)
                vtkpts.SetName(plot['name'])
                actor1 = create_actor_polygon(pts=vtkpts, color=create_color(plot['color']),
                                                   name=plot['name'], index=plot['idx'], size=self.vconf.line_width * 2)
                self.vtk_actors.append(actor1)

    def set_renderer_window(self, render_window):
        self.render()
        create_render_window(render_window, self.vtk_actors, dict(KeyPressEvent=(self.vconf.keypress_callback, 1.0)))


# It is easier to plot 2-dimensional curves with VisCurve3D
VisCurve2D = VisCurve3D


class MoveCtrlPointsInteractor(vtk.vtkInteractorStyleTrackballCamera):

    def __init__(self):
        super().__init__()
        self.AddObserver("LeftButtonPressEvent", self.leftButtonPressEvent)

    def leftButtonPressEvent(self, obj, event):
        print("##### Inside leftButtonPressEvent ######")
        clickPos = obj.GetInteractor().GetEventPosition()
        picker = vtk.vtkPropPicker()
        picker.Pick(clickPos[0], clickPos[1], 0, obj.GetDefaultRenderer())

    def OnLeftButtonDown(self):
        super().OnLeftButtonDown()
        print("##### Inside OnLeftButtonDown ######")


class VisSurface(vis.VisAbstract):
    """ VTK visualization module for surfaces. """
    def __init__(self, surface, config=VisConfig(), **kwargs):
        super(VisSurface, self).__init__(config, **kwargs)
        self._module_config['ctrlpts'] = "quads"
        self._module_config['evalpts'] = "triangles"
        self.surface = surface

        self.render_window = None
        self.interactor_style = None

    def render(self, colormap=cm.cool, **kwargs):
        """ Plots the surface and the control points grid. """
        # Calling parent function
        super(VisSurface, self).render(**kwargs)

        # Initialize a list to store VTK actors
        self.vtk_actors = []

        # Start plotting
        for plot in self._plots:
            # Plot control points
            if plot['type'] == 'ctrlpts' and self.vconf.display_ctrlpts:
                vertices = [v.data for v in plot['ptsarr'][0]]
                faces = [q.data for q in plot['ptsarr'][1]]
                # Points as spheres
                pts = np.array(vertices, dtype=np.float)
                vtkpts = numpy_to_vtk(pts, deep=False, array_type=VTK_FLOAT)
                vtkpts.SetName(plot['name'])
                actor1 = create_actor_pts(pts=vtkpts, color=create_color(plot['color']),
                                               name=plot['name'], index=plot['idx'], size=0.5)
                self.vtk_actors.append(actor1)
                # Quad mesh
                lines = np.array(faces, dtype=np.int)
                actor2 = create_actor_mesh(pts=vtkpts, lines=lines, color=create_color(plot['color']),
                                                name=plot['name'], index=plot['idx'], size=self.vconf.line_width)
                self.vtk_actors.append(actor2)

            # Plot evaluated points
            if plot['type'] == 'evalpts' and self.vconf.display_evalpts:
                vertices = [v.data for v in plot['ptsarr'][0]]
                vtkpts = numpy_to_vtk(vertices, deep=False, array_type=VTK_FLOAT)
                vtkpts.SetName(plot['name'])
                faces = [t.data for t in plot['ptsarr'][1]]
                tris = np.array(faces, dtype=np.int)
                actor1 = create_actor_tri(pts=vtkpts, tris=tris, color=create_color(plot['color']),
                                               name=plot['name'], index=plot['idx'])
                self.vtk_actors.append(actor1)

            # Plot trim curves
            if self.vconf.display_trims:
                if plot['type'] == 'trimcurve':
                    pts = np.array(plot['ptsarr'], dtype=np.float)
                    vtkpts = numpy_to_vtk(pts, deep=False, array_type=VTK_FLOAT)
                    vtkpts.SetName(plot['name'])
                    actor1 = create_actor_polygon(pts=vtkpts, color=create_color(plot['color']),
                                                       name=plot['name'], index=plot['idx'], size=self.vconf.trim_size)
                    self.vtk_actors.append(actor1)

    def set_render_window(self, render_window):
        self.render_window = render_window

        renderer = self.render_window.GetRenderers().GetFirstRenderer()
        renderer.SetBackground(1.0, 1.0, 1.0)

        # Add actors to the scene
        print(self.vtk_actors)
        for actor in self.vtk_actors:
            renderer.AddActor(actor)
        interactor = self.render_window.GetInteractor()

        # Use trackball camera
        self.interactor_style = MoveCtrlPointsInteractor()
        self.interactor_style.SetDefaultRenderer(renderer)
        interactor.SetInteractorStyle(self.interactor_style)

        interactor.Render()
        # create_render_window(render_window, self.vtk_actors, dict(KeyPressEvent=(self.vconf.keypress_callback, 1.0)))


class VisVolume(vis.VisAbstract):
    """ VTK visualization module for volumes. """
    def __init__(self, config=VisConfig(), **kwargs):
        super(VisVolume, self).__init__(config, **kwargs)
        self._module_config['ctrlpts'] = "points"
        self._module_config['evalpts'] = "points"
        self.vtk_actors = []

    def render(self, render_window, **kwargs):
        """ Plots the volume and the control points. """
        # Calling parent function
        super(VisVolume, self).render(**kwargs)

        # Initialize a list to store VTK actors
        self.vtk_actors = []

        # Start plotting
        for plot in self._plots:
            # Plot control points
            if plot['type'] == 'ctrlpts' and self.vconf.display_ctrlpts:
                # Points as spheres
                pts = np.array(plot['ptsarr'], dtype=np.float)
                temp_actor = create_actor_pts(pts=vtkpts, color=create_color(plot['color']),
                                                   name=plot['name'], index=plot['idx'])
                self.vtk_actors.append(temp_actor)

            # Plot evaluated points
            if plot['type'] == 'evalpts' and self.vconf.display_evalpts:
                pts = np.array(plot['ptsarr'], dtype=np.float)
                temp_actor = create_actor_pts(pts=vtkpts, color=create_color(plot['color']),
                                                   name=plot['name'], index=plot['idx'])
                self.vtk_actors.append(temp_actor)

    def set_renderer_window(self, render_window):
        self.render()
        create_render_window(render_window, self.vtk_actors, dict(KeyPressEvent=(self.vconf.keypress_callback, 1.0)))

class VisVoxel(vis.VisAbstract):
    """ VTK visualization module for voxel representation of the volumes. """
    def __init__(self, config=VisConfig(), **kwargs):
        super(VisVoxel, self).__init__(config, **kwargs)
        self._module_config['ctrlpts'] = "points"
        self._module_config['evalpts'] = "voxels"
        self.vtk_actors = []

    def render(self, render_window, **kwargs):
        """ Plots the volume and the control points. """
        # Calling parent function
        super(VisVoxel, self).render(**kwargs)

        # Initialize a list to store VTK actors
        self.vtk_actors = []

        # Start plotting
        for plot in self._plots:
            # Plot control points
            if plot['type'] == 'ctrlpts' and self.vconf.display_ctrlpts:
                # Points as spheres
                pts = np.array(plot['ptsarr'], dtype=np.float)
                temp_actor = create_actor_pts(pts=vtkpts, color=create_color(plot['color']),
                                                   name=plot['name'], index=plot['idx'])
                self.vtk_actors.append(temp_actor)

            # Plot evaluated points
            if plot['type'] == 'evalpts' and self.vconf.display_evalpts:
                faces = np.array(plot['ptsarr'][1], dtype=np.float)
                filled = np.array(plot['ptsarr'][2], dtype=np.int)
                grid_filled = faces[filled == 1]
                temp_actor = create_actor_hexahedron(grid=grid_filled, color=create_color(plot['color']),
                                                          name=plot['name'], index=plot['idx'])
                self.vtk_actors.append(temp_actor)

    def set_renderer_window(self, render_window):
        self.render()
        create_render_window(render_window, self.vtk_actors, dict(KeyPressEvent=(self.vconf.keypress_callback, 1.0)))
