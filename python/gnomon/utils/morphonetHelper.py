import json
import math
import numpy as np
import traceback

import vtk
from vtk.util.numpy_support import numpy_to_vtk, vtk_to_numpy

from gnomon.core import gnomonCellImage, cellImageData_pluginFactory, setMorphonetHelperCreator, gnomonMorphonetHelper, gnomonMorphonetHelperCreator
from gnomon.utils import load_plugin_group

from gnomon.utils.decorators.form_series import formDictFromSeries
from morphonet import Net, tools
from timagetk import TissueImage3D, LabelledImage

load_plugin_group("cellImageData")

class MorphonetHelper(gnomonMorphonetHelper):
    """ Morphonet Helper class
    """

    def __init__(self) :
        super().__init__()
        self._net = None

    def obj_to_tissue_image(self, obj: list[str], dim = (100, 100, 100)):
        """convert a str into a numpy array using a vtk polydata
        1. create a vtkPolydata from obj
        2. a white image of good dimensions
        3. polydataToImageStencil
        4. get np array
        5. make tissueImage
        """

        polydatas = []
        vtk_points = vtk.vtkPoints() # one vtk_points for every pd

        bounds = [100000., -1000000., 1000000., -1000000., 1000000., -1000000.]
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

            polydatas.append((polydata, cell_idx))

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

                    if len(polydatas) > 0:
                        new_bounds = polydata.GetBounds()
                        for i in range(3):
                            bounds[i*2] = min(bounds[i*2], new_bounds[i*2])
                            bounds[i*2+1] = max(bounds[i*2+1], new_bounds[i*2+1])

                    polydata = vtk.vtkPolyData()
                    polydata.Initialize()
                    vtk_cells = vtk.vtkCellArray()
                    polydata.SetPoints(vtk_points)
                    polydata.SetPolys(vtk_cells)

                    polydatas.append((polydata, cell_idx))

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

        new_bounds = polydata.GetBounds()
        for i in range(3):
            bounds[i*2] = min(bounds[i*2], new_bounds[i*2])
            bounds[i*2+1] = max(bounds[i*2+1], new_bounds[i*2+1])

        print(len(polydatas), " polydatas created. bounds: ", bounds, " ids: ", [i for (_, i) in polydatas])
        
        final_img = vtk.vtkImageData()
        spacing = [(bounds[ii*2+1] - bounds[ii*2])/dim[ii] for ii in range(0,3)]

        final_img.SetSpacing(spacing)
        final_img.SetDimensions(dim)
        final_img.SetExtent(0, dim[0] - 1, 0, dim[1] - 1, 0, dim[2] - 1)
        origin = [bounds[ii*2] + spacing[ii] / 2 for ii in range(0,3)]  
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

        print("final image done")

        for pd, cell_idx in polydatas:
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

        tissue = TissueImage3D(np.reshape(arr, (dim[0],dim[1],dim[2])), 
                            background=1,
                            not_a_label=0,
                            origin=origin,
                            voxelsize=spacing)
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

    def loadMnDataAtTime(self, time: int, dim_x: int, dim_y: int, dim_z: int) -> gnomonCellImage:
        """Load morphonet data at time and return a gnomonCellImageData serialized

        Args:
            time (int): time to load
            dim_x (int): dim x for the image
            dim_y (int): dim y for the image
            dim_z (int): dim z for the image

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
            dim = (dim_x, dim_y, dim_z)
            obj = self._net.get_mesh_at(time)
            obj = obj.split("\n")
            tissue = self.obj_to_tissue_image(obj, dim=dim)
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


    def transform_to_mn_mesh(self, seg_img, time):
        """
        Transform a segmentedimage to a morphonet mesh

        Parameters
        ----------
        seg_img: segmented images
            image to transform
        time: int
            time to load

        """
        try:
            background = seg_img.background
        except:
            background = 1

        try:
            tissue = seg_img.data().get_tissue_image()
            # border = 2
            # factor = 1

            # _dataToConvert=np.zeros(np.array(tissue.shape) + border * 2).astype(tissue.dtype)
            # _dataToConvert[:,:,:]=background
            # _dataToConvert[border:-border,border:-border,border:-border]=dataFull[::factor,::factor,::factor]
            # elts=np.unique(_dataToConvert)
            # elts=elts[elts!=background] #Remove Background

            # TODO
            # create 1 polydata for each elts
            # use vtkMarchingCubes
            # contour = vtkDiscreteMarchingCubes()
            # contour.SetInputData(reader.GetOutput())
            # contour.ComputeNormalsOn()
            # contour.ComputeGradientsOn()
            # contour.SetValue(0,255)
            # contour.Update()
            # self.polydata= contour.GetOutput()

            # if self.Smooth and self.polydata.GetPoints() is not None:
            #     smooth_angle=120.0
            #     smoth_passband=0.01
            #     smooth_itertations=25
            #     smoother = vtkWindowedSincPolyDataFilter()
            #     smoother.SetInputData(self.polydata)
            #     smoother.SetFeatureAngle(smooth_angle)
            #     smoother.SetPassBand(smoth_passband)
            #     smoother.SetNumberOfIterations(smooth_itertations)
            #     smoother.NonManifoldSmoothingOn()
            #     smoother.NormalizeCoordinatesOn()
            #     smoother.Update()
            #     self.polydata= smoother.GetOutput()


            # if self.Decimate and self.polydata.GetPoints() is not None:
            #     mesh_fineness=1.0
            #     decimater = vtkQuadricClustering()
            #     decimater.SetInputData(self.polydata)
            #     decimater.SetNumberOfDivisions(*np.uint16(tuple(mesh_fineness*np.array(np.array(_dataToConvert.shape)/2))))
            #     decimater.SetFeaturePointsAngle(30.0)
            #     decimater.CopyCellDataOn()
            #     decimater.Update()
            #     self.polydata= decimater.GetOutput()

            # if self.Reduction and self.polydata.GetPoints() is not None:
            #     decimatePro  = vtkDecimatePro()
            #     decimatePro.SetInputData(self.polydata)
            #     decimatePro.SetTargetReduction(self.TargetReduction)
            #     decimatePro.Update()
            #     self.polydata= decimatePro.GetOutput()

            
            #for each elts
            # obj=""
            # shiftFace=1
            # for tc in all_threads:
            #     polydata=tc.polydata
            #     elt=tc.elt
            #     if polydata.GetPoints() is not None:
            #         obj+="g "+str(t)+","+str(elt)+"\n"
            #         for p in range(polydata.GetPoints().GetNumberOfPoints()):
            #             v=polydata.GetPoints().GetPoint(p)
            #             obj+='v ' + str((v[0]-Border)*factor*VoxelSize[0]-center[0]) +' '+str((v[1]-Border)*factor*VoxelSize[1]-center[1]) +' '+str((v[2]-Border)*factor*VoxelSize[2]-center[2])+'\n'
            #         for f in range(polydata.GetNumberOfCells()):
            #             obj+='f ' + str(shiftFace+polydata.GetCell(f).GetPointIds().GetId(0)) +' '+str(shiftFace+polydata.GetCell(f).GetPointIds().GetId(1)) +' '+str(shiftFace+polydata.GetCell(f).GetPointIds().GetId(2))+'\n'
            #         shiftFace+=polydata.GetPoints().GetNumberOfPoints()


            # TODO is this writing files to disk ????
            mesh = tools.convert_to_OBJ(tissue, time,
                                        background=background, VoxelSize=tissue.voxelsize,
                                        path_write=None)

            with open("test_obj.obj", 'w', encoding='utf-8') as f:
                f.write(mesh)

            return mesh
        except Exception as e:
            print(e)
            traceback.print_exception(type(e), e, e.__traceback__)
            return None

    def createDataset(self, name: str, form_series , id_NCBI: int, id_type: int, description: str) -> int:
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

        meshes = {int(t): self.transform_to_mn_mesh(img, t) for t, img in form_series.items()}

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
