import json
import math
import numpy as np
import scipy.ndimage as nd
import traceback

import vtk
from vtk.util.numpy_support import numpy_to_vtk, vtk_to_numpy

from gnomon.core import gnomonCellImage, cellImageData_pluginFactory, setMorphonetHelperCreator, gnomonMorphonetHelper, gnomonMorphonetHelperCreator
from gnomon.utils import load_plugin_group

from gnomon.utils.decorators.form_series import formDictFromSeries
from morphonet import Net, tools

from timagetk import TissueImage3D, LabelledImage, SpatialImage
from timagetk.algorithms.watershed import watershed
from timagetk.algorithms.resample import isometric_resampling

from visu_core.vtk.utils.polydata_tools import vtk_combine_polydatas
from visu_core.vtk.utils.image_tools import image_to_vtk_cell_polydatas

load_plugin_group("cellImageData")


def _obj_cells(cell_polydatas, time=0):
    obj_str = ""
    offset = 1

    for cell, polydata in cell_polydatas.items():
        obj_str += f"g {time},{cell}\n"
        vertex_dict = {}
        points = polydata.GetPoints()
        if points is None:
            continue
        points = vtk_to_numpy(points.GetData())
        for p, point in enumerate(points):
            obj_str += f"v {' '.join(map(str, point))}\n"
            vertex_dict[p] = p + offset
        offset += len(points)
        triangles = vtk_to_numpy(polydata.GetPolys().GetData()).reshape((polydata.GetNumberOfPolys(), 4))[:, 1:]
        for t in triangles:
            obj_str += f"f {vertex_dict[t[0]]} {vertex_dict[t[1]]} {vertex_dict[t[2]]}\n"
    return obj_str


def _info_property(cell_property, time=0, type='float'):
    info_str = ""
    if type in ['float', 'group', 'selection']:
        for label, ppty in cell_property.items():
            info_str += f"{time},{label}:{ppty}\n"
    elif type in ['time']:
        if time>0:
            for label, ppty in cell_property.items():
                info_str += f"{time-1},{ppty}:{time},{label}\n"

    return info_str


class MorphonetHelper(gnomonMorphonetHelper):
    """ Morphonet Helper class
    """

    def __init__(self) :
        super().__init__()
        self._net = None

    def obj_to_tissue_image(self, obj: list[str], voxelsize=0.1):
        """convert a str into a numpy array using a vtk polydata
        1. create a vtkPolydata from obj
        2. a white image of good dimensions
        3. polydataToImageStencil
        4. get np array
        5. make tissueImage
        """

        polydatas = {}
        vtk_points = vtk.vtkPoints() # one vtk_points for every pd

        idx_availables = [i for i in range(65535)]
        idx_availables.remove(1) # remove background
        line_idx=0

        #first line is not a new cell info
        if not obj[0].startswith('g'):
            cell_idx = idx_availables[0]
            idx_availables.remove(cell_idx)

            polydata = vtk.vtkPolyData()
            polydata.Initialize()
            vtk_cells = vtk.vtkCellArray()
            polydata.SetPoints(vtk_points)
            polydata.SetPolys(vtk_cells)

            polydatas[cell_idx] = polydata

        try:
            for line in obj:
                if line.startswith('g'):
                    # new polydata
                    ll = line[1:].strip().split(',')
                    if len(ll) == 1:
                        cell_idx = ll[0]
                    elif len(ll) == 2:
                        #current_time = int(ll[0])
                        cell_idx = ll[1]
                    else:
                        #current_time = int(ll[0])
                        cell_idx = ll[1]
                        #channel = ll[2]

                    # in case cell_idx cannot be casted to int
                    try: 
                        cell_idx= int(cell_idx)

                        # 1 is the background
                        if cell_idx == 1:
                            print("cell idx changed from 1 to ", idx_availables[-1])
                            cell_idx = idx_availables[-1]
                    except: 
                        cell_idx = idx_availables[0]
                    idx_availables.remove(cell_idx)

                    polydata = vtk.vtkPolyData()
                    polydata.Initialize()
                    vtk_cells = vtk.vtkCellArray()
                    polydata.SetPoints(vtk_points)
                    polydata.SetPolys(vtk_cells)

                    polydatas[cell_idx] = polydata

                if line.startswith('v'):
                    point_id = vtk_points.InsertNextPoint([float(v) for v in line[1:].strip().split(' ')])
                    vtk_cells.InsertCellPoint(point_id)
                if line.startswith('f'):
                    tri = vtk.vtkTriangle()
                    for idx,v in zip([0,1,2], line[1:].strip().split(' ')):
                        tri.GetPointIds().SetId(idx, int(v)-1)
                    vtk_cells.InsertNextCell(tri)
                line_idx += 1

        except Exception as e:
            print("error parsing obj file:", e)
            print(obj[max(line_idx-10, 0):line_idx+1])
            return None

        full_polydata = vtk_combine_polydatas(list(polydatas.values()))
        bounds = full_polydata.GetBounds()

        print(len(polydatas), " polydatas created. bounds: ", bounds, " ids: ", list(polydatas.keys()))
        
        final_img = vtk.vtkImageData()
        spacing = [voxelsize]*3
        dim = [int(np.ceil((bounds[ii*2+1] - bounds[ii*2])/voxelsize)) for ii in range(0, 3)]
        print(dim)
        # spacing = [(bounds[ii*2+1] - bounds[ii*2])/dim[ii] for ii in range(0,3)]

        final_img.SetSpacing(spacing)
        final_img.SetDimensions(dim)
        # final_img.SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1)
        # final_img.SetExtent(0, int(bounds[1]-bounds[0]), 0, int(bounds[3]-bounds[2]), 0, int(bounds[5]-bounds[4]))
        origin = [bounds[ii*2] + spacing[ii] / 2 for ii in range(0, 3)]
        final_img.SetOrigin(origin)
        final_img.ComputeBounds()
        
        print("img bounds: ", final_img.GetBounds(), " and dims:", dim)
        print("img  origin:", origin)

        final_img.AllocateScalars(vtk.VTK_UNSIGNED_SHORT, 1)

        # fill the image with foreground voxels:
        count = final_img.GetNumberOfPoints()
        background_value = 1

        for i in range(count):
            final_img.GetPointData().GetScalars().SetTuple1(i, background_value)

        #final_img.GetPointData().GetScalars().Fill(background_value)

        print("final image done")

        for cell_idx, pd in polydatas.items():
            pol2stenc = vtk.vtkPolyDataToImageStencil()
            pol2stenc.SetTolerance(0)  # important if extruder.SetVector(0, 0, 1) !!!
            pol2stenc.SetInputData(pd)
            pol2stenc.SetOutputOrigin(origin)
            pol2stenc.SetOutputSpacing(spacing)
            pol2stenc.SetOutputWholeExtent(final_img.GetExtent())
            pol2stenc.Update()

            imgstenc = vtk.vtkImageStencil()
            imgstenc.SetInputData(final_img)
            imgstenc.SetStencilConnection(pol2stenc.GetOutputPort())
            imgstenc.ReverseStencilOn()
            imgstenc.SetBackgroundValue(cell_idx)
            imgstenc.Update()

            final_img =imgstenc.GetOutput()

        print("image stencil done")
        #visu_debug(img=final_img)
        scalars = imgstenc.GetOutput().GetPointData().GetScalars()
        arr = vtk_to_numpy(scalars)

        # debug purpose
        # to delete later        
        (unique, counts) = np.unique(arr, return_counts=True)
        frequencies = np.asarray((unique, counts)).T
        print(frequencies)

        seg_img = LabelledImage(np.reshape(arr, (dim[2], dim[1], dim[0])).transpose((2,1,0)),
                                not_a_label=0, origin=origin[::-1], voxelsize=spacing[::-1])  # ex spacings

        # TODO : find a way to fill holes between cells without altering shapes
        # background_img = seg_img.get_array()==1
        # eroded_background_img = nd.binary_erosion(background_img, iterations=2)
        # seg_img[np.bitwise_xor(background_img, eroded_background_img)] = 0
        # distance_img = np.round(nd.distance_transform_cdt(background_img)).astype(np.uint16)
        # gradient_img = SpatialImage(distance_img, origin=origin[::-1], voxelsize=spacing[::-1])
        # seg_img = watershed(gradient_img, seg_img)

        tissue = TissueImage3D(seg_img,
                               background=1,
                               not_a_label=0,
                               origin=origin[::-1],
                               voxelsize=spacing[::-1])  # ex spacings

        tissue.cells.volume()
        return tissue

    def is_connected(self):
        """test if connected to Morphoner server

        Returns:
            bool: True if connected
        """
        return self._net and self._net._is_connected()

    def connect(self, login: str, password: str) -> bool:
        """Connect to morphonet server

        Args:
            login (str): _description_
            password (str): _description_

        Returns:
            bool: True if connected, False otherwise
        """        
        self._net = Net(login, password)
        return self.is_connected()


    def datasetsInfo(self, search):
        """Get infos on available datasets in morphoner server

        Args:
            search (str): Not used for now_

        Returns:
            str: a JSON dumps of available datasets info 
        """
        if self.is_connected():
            data=self._net._request({},'/api/userrelatedset/','GET')

            for ds in data:
                ds['owner'] = self._net.get_guy_by_id(ds['id_people'])
                ds['own'] = self._net.id_people==ds['id_people']

            return json.dumps(data)

        return ""

    def selectDataset(self, id: int):
        """Select a dataset

        Args:
            id (int): the id of the dataset to select_

        Returns:
            bool: True if the dataset is selected, False otherwise 
        """
        if not self.is_connected():
            print("not connected to morphonet, nothing done")
            return False

        self._net.select_dataset_by_id(id)
        return self._net.id_dataset != -1


    def startTime(self):
        if self._net:
            return self._net.minTime
        else: 
            return 0

    def endTime(self):
        if self._net:
            return self._net.maxTime
        else:
            return 0

    def loadMnDataAtTime(self, time: int, voxelsize: float) -> gnomonCellImage:
        """Load morphonet data at time and return a gnomonCellImageData serialized

        Args:
            time (int): time to load
            voxelsize (float): cubic voxel size for

        Returns:
            gnomonCellImage: the image or PyNone
        """

        if self._net.id_dataset == -1:
            print("no dataset selected, nothing is done ")
            return None

        cell_img_data = cellImageData_pluginFactory().create("gnomonCellImageDataTissueImage")

        # if self._net.is_image_at(time): 
        #     arr = self._net.get_image_at(time)
        #     tissue = TissueImage3D(arr)
        #     tissue.cells.volume()
        #     cell_img_data.set_tissue_image(tissue)

        # else:
        try:
            obj = self._net.get_mesh_at(time)
            obj = obj.split("\n")
            tissue = self.obj_to_tissue_image(obj, voxelsize=voxelsize)
            if tissue is not None:
                cell_img_data.set_tissue_image(tissue)
                cell_img = gnomonCellImage()
                cell_img.setData(cell_img_data)

                if cell_img.cellCount() > 0:
                    return cell_img
                else:
                    return None
            else:
                return None
        except Exception as e:
            print(e)
            traceback.print_exception(type(e), e, e.__traceback__)

            return None


    def transform_to_mn_mesh(self, seg_img, time, resolution, border):
        """
        Transform a segmentedimage to a morphonet mesh

        Parameters
        ----------
        seg_img: segmented images
            image to transform
        time: int
            time to load
        resolution: float
            voxelsize of the image on which marching cubes are computed
        border: bool
            whether to add a border on margin cells


        """
        if hasattr(seg_img, 'background'):
            background = seg_img.background
        else:
            background = 1

        try:
            tissue = seg_img.data().get_tissue_image()
            labels = tissue.cell_ids()
            seg_img = isometric_resampling(tissue, method=resolution, interpolation='nearest')
            if border:
                seg_img[ 0, :, :] = background
                seg_img[-1, :, :] = background
                seg_img[ :, 0, :] = background
                seg_img[ :,-1, :] = background
                seg_img[ :, :, 0] = background
                seg_img[ :, :,-1] = background
            cell_polydatas = image_to_vtk_cell_polydatas(seg_img, labels=labels, smoothing=10, decimation=25)
            obj_str = _obj_cells(cell_polydatas, time=int(np.round(time)))

            return obj_str

        except Exception as e:
            print(e)
            traceback.print_exception(type(e), e, e.__traceback__)
            return None


    def transform_to_mn_infos(self, time_series):
        """
        Transform a series of segmentedimage to morphonet infos

        Parameters
        ----------
        time_series: dict
            images to transform
        """

        infos = {}
        property_types = {}

        times = np.sort(list(time_series.keys()))

        for i_t, time in enumerate(times):
            tissue = time_series[time].data().get_tissue_image()

            if hasattr(tissue, 'background'):
                background = tissue.background
            else:
                background = 1

            property_names = [p for p in tissue.cells.feature_names() if tissue.cells.feature(p)!={}]
            for property_name in property_names:
                cell_property = tissue.cells.feature(property_name)

                if not property_name in property_types:
                    property_values = np.array(list(cell_property.values()))
                    if property_name == 'ancestor':
                        property_types[property_name] = 'time'
                    elif (property_values.dtype == int) and (property_values.ndim == 1) and (np.all(property_values > 0)) and (np.all(property_values < 256)):
                        property_types[property_name] = 'selection'
                    elif (property_values.ndim == 1) and (property_values.dtype != np.dtype('O')):
                        property_types[property_name] = 'float'
                    else:
                        property_types[property_name] = None

                if property_types[property_name] is not None:
                    if property_name not in infos:
                        infos[property_name] = f"# MorphoNet '{property_name}' information\n"
                        infos[property_name] += f'type:{property_types[property_name]}\n'

                    cell_property = {c: cell_property[c] for c in tissue.cell_ids() if c in cell_property and c!=background}
                    infos[property_name] += _info_property(cell_property, time=i_t, type=property_types[property_name])

        return infos

    def createDataset(self, name: str, form_series, id_NCBI: int, id_type: int, description: str, resolution=0.8, border=True) -> int:
        """Create and upload a dataset

        Args:
            name (str): _name of the dataset
            form_series (_type_): _images_
            id_NCBI (int): _NCBI id if available
            id_type (int): _0:  1 or 2
            description (str): _dataset description_

        Returns:
            int: the id of the created dataset or -1 if there is an error
        """
        print(form_series)
        print(type(form_series))

        times = np.sort(list(form_series.keys()))

        meshes = {i_t: self.transform_to_mn_mesh(form_series[t], time=i_t, resolution=resolution, border=border) for i_t, t in enumerate(times)}
        infos = self.transform_to_mn_infos(form_series)

        old_ds_id = self._net.id_dataset
        self._net.create_dataset(name,
                                 minTime=min(meshes.keys()),
                                 maxTime=max(meshes.keys()),
                                 id_NCBI=id_NCBI,
                                 id_type=id_type)

        if self._net.id_dataset == old_ds_id:
            return -1

        for t, obj in meshes.items():
            if not obj is None:
                self._net.upload_mesh_at(t, obj)

        for info_name, info in infos.items():
            info_id = self._net.upload_info(info_name, info)
            self._net.share_info_by_id(info_id)

        return self._net.id_dataset
    
    def deleteDataset(self, id: int) -> bool:
        """Delete a dataset by id
        """
        if self.selectDataset(id):
            self._net.delete_dataset()
            return self._net.id_dataset == -1        
        return False

class morphonetHelperCreator(gnomonMorphonetHelperCreator):
    def __init__(self):
        super().__init__()
        self.thisown = 0 

    def create(self):
        try:
            obj = MorphonetHelper()
            obj.__disown__()
            return obj
        except Exception as e:
            print(e)
            raise e

creator = morphonetHelperCreator()
setMorphonetHelperCreator(creator)


def visu_debug(polydata=None, img=None):
    """dddff
    """

    renderer = vtk.vtkRenderer()

    # Create an actor
    if polydata:
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(polydata)
        pd_actor = vtk.vtkActor()
        pd_actor.SetMapper(mapper)
        renderer.AddActor(pd_actor)
    
    if img:
        alphaChannelFunc = vtk.vtkPiecewiseFunction()
        alphaChannelFunc.AddPoint(0, .0)
        alphaChannelFunc.AddPoint(1, .0)
        alphaChannelFunc.AddPoint(2, .02)
        alphaChannelFunc.AddPoint(255, .02)
        colorFunc = vtk.vtkColorTransferFunction()
        colorFunc.AddRGBPoint(0, 0., 0., 0.)
        colorFunc.AddRGBPoint(1, 0., 0., 0.)
        colorFunc.AddRGBPoint(2, 0., 0., 0.)
        colorFunc.AddRGBPoint(6, 1., 0., 0.)
        colorFunc.AddRGBPoint(7, 0., 0., 1.)
        colorFunc.AddRGBPoint(9, 1., 0., 1.)
        colorFunc.AddRGBPoint(255, 0., 1.0, 0.0)
        volumeProperty = vtk.vtkVolumeProperty()
        volumeProperty.SetColor(colorFunc)
        volumeProperty.SetScalarOpacity(alphaChannelFunc)

        volumeMapper = vtk.vtkFixedPointVolumeRayCastMapper()
        volumeMapper.SetInputData(img)
        #volumeMapper.SetInputConnection(img.GetOutputPort())

        volume = vtk.vtkVolume()
        volume.SetMapper(volumeMapper)
        volume.SetProperty(volumeProperty)
        renderer.AddVolume(volume)

    # Setup renderer
    axes_actor = vtk.vtkAxesActor()
    axes_actor.AxisLabelsOff()
    renderer.AddActor(axes_actor)

    renderer.ResetCamera()
    #renderer.SetBackground(colors->GetColor3d("SteelBlue").GetData());

    # Setup render window
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(renderer)
    renderWindow.SetWindowName("ImageStencil")

    # Setup render window interactor
    renderWindowInteractor= vtk.vtkRenderWindowInteractor() 
    style = vtk.vtkInteractorStyleTrackballCamera()
    renderWindowInteractor.SetInteractorStyle(style)

    # Render and start interaction
    renderWindowInteractor.SetRenderWindow(renderWindow)
    renderWindow.Render()
    renderWindowInteractor.Start()
    


# if __name__ == "__main__":
    # import gnomon.utils.morphonetHelper as helper
    # mn = helper.MorphonetHelper()
    # mn.connect("trcabel", "....")
    # mn.selectDataset(204)
    # cell_img = mn.loadMnDataAtTime(1, 100, 100, 100)  
    # mesh = mn.transform_to_mn_mesh(cell_img, 1)
    # new_id = mn.createDataset("test1", {1: cell_img}, 0, 0, "mydesc")
    # mn.deleteDataset(new_id)
