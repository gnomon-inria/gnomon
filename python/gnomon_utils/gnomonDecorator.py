import gnomoncore

from gnomoncore import (gnomonImage, gnomonImageSeries,  
                        gnomonCellComplex, gnomonCellComplexSeries,
                        gnomonCellImage, gnomonCellImageSeries,
                        gnomonDataFrame, gnomonDataFrameSeries,
                        gnomonLString, gnomonLStringSeries, 
                        gnomonMesh, gnomonMeshSeries,
                        gnomonPointCloud, gnomonPointCloudSeries,
                        gnomonTree, gnomonTreeSeries)

from dtkTimagetkConverter.dtkTimagetkConverter import dtk_img_to_sp_img
from dtkTimagetkConverter.dtkTimagetkConverter import sp_img_to_dtk_img

from .gnomonPlugin import load_plugin_group

# ------------------------------------------------------------------------------
# --------------------------------- Tree ----------------------------------
# ------------------------------------------------------------------------------


load_plugin_group("treeData")

def buildTreeSeries(tree_dict, data_plugin="gnomonTreeDataTreex", data_setter="set_tree"):
    tree_series = gnomonTreeSeries()
    tree = {}
    tree_data = {}

    for time in tree_dict.keys():
        tree[time] = gnomonTree()
        tree_series.insert(time, tree[time])
        tree_data[time] = gnomoncore.treeData_pluginFactory().create(data_plugin)
        getattr(tree_data[time],data_setter)(tree_dict[time])
        tree[time].setData(tree_data[time])

    return tree_series, tree, tree_data


def treeDictFromSeries(tree_series, data_attr='_tree'):
    tree = {}
    tree_dict = {}
    for time in tree_series.times():
        tree[time] = tree_series.at(time).asTree()
        tree_dict[time] = getattr(tree[time].data(), data_attr)

    return tree_dict, tree


def _gnomonTreeInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"_in_tree_series"):
            self._in_tree_series, self._in_tree, self._in_tree_data = buildTreeSeries(getattr(self, attr))
        return self._in_tree_series

    setattr(cls, method, func)

    def setter_func(self, tree_series):
        self._in_tree_series = tree_series
        self._in_tree = {}
        setattr(self, attr, {})

        if self._in_tree_series is not None:
            tree_dict, self._in_tree = treeDictFromSeries(self._in_tree_series)
            setattr(self, attr, tree_dict)
            
            if hasattr(self ,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonTreeInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonTreeInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonTreeInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonTreeOutput(cls, attr, method):
    def func(self):
        self._out_tree_series, self._out_tree, self._out_tree_data = buildTreeSeries(getattr(self, attr))
        return self._out_tree_series

    setattr(cls, method, func)

    return cls


def gnomonTreeOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonTreeOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonTreeOutput(cls, attr, method)

        return wrapper

# ------------------------------------------------------------------------------
# --------------------------------- CellImage -------------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("cellImageData")


def buildCellImageSeries(cellImage_dict, data_plugin="gnomonCellImageDataPropertySpatialImage", data_setter="set_property_image"):
    cellImage_series = gnomonCellImageSeries()
    cellImage = {}
    cellImage_data = {}

    for time in cellImage_dict.keys():
        cellImage[time] = gnomonCellImage()
        cellImage_series.insert(time, cellImage[time])
        cellImage_data[time] = gnomoncore.cellImageData_pluginFactory().create(data_plugin)
        getattr(cellImage_data[time],data_setter)(cellImage_dict[time])
        cellImage[time].setData(cellImage_data[time])

    return cellImage_series, cellImage, cellImage_data


def cellImageDictFromSeries(cellImage_series, data_attr='_p_img'):
    cellImage = {}
    cellImage_dict = {}
    for time in cellImage_series.times():
        cellImage[time] = cellImage_series.at(time).asCellImage()
        cellImage_dict[time] = getattr(cellImage[time].data(), data_attr)

    return cellImage_dict, cellImage


def _gnomonCellImageInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"_in_cellImage_series"):
            self._in_cellImage_series, self._in_cellImage, self._in_cellImage_data = buildCellImageSeries(getattr(self, attr))
        return self._in_cellImage_series

    setattr(cls, method, func)

    def setter_func(self, cellImage_series):
        self._in_cellImage_series = cellImage_series
        self._in_cellImage = {}
        setattr(self, attr, {})

        if self._in_cellImage_series is not None:
            cellImage_dict, self._in_cellImage = cellImageDictFromSeries(self._in_cellImage_series)
            setattr(self, attr, cellImage_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonCellImageInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonCellImageInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonCellImageInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonCellImageOutput(cls, attr, method):
    def func(self):
        self._out_cellImage_series, self._out_cellImage, self._out_cellImage_data = buildCellImageSeries(getattr(self ,attr))
        return self._out_cellImage_series

    setattr(cls, method, func)

    return cls


def gnomonCellImageOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonCellImageOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonCellImageOutput(cls, attr, method)

        return wrapper
    

# ------------------------------------------------------------------------------
# --------------------------------- CellComplex -------------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("cellComplexData")


def buildCellComplexSeries(cellComplex_dict, data_plugin="gnomonCellComplexDataPropertyTopomesh", data_setter="set_property_topomesh"):
    cellComplex_series = gnomonCellComplexSeries()
    cellComplex = {}
    cellComplex_data = {}

    for time in cellComplex_dict.keys():
        cellComplex[time] = gnomonCellComplex()
        cellComplex_series.insert(time, cellComplex[time])
        cellComplex_data[time] = gnomoncore.cellComplexData_pluginFactory().create(data_plugin)
        getattr(cellComplex_data[time],data_setter)(cellComplex_dict[time])
        cellComplex[time].setData(cellComplex_data[time])

    return cellComplex_series, cellComplex, cellComplex_data


def cellComplexDictFromSeries(cellComplex_series, data_attr='_topomesh'):
    cellComplex = {}
    cellComplex_dict = {}
    for time in cellComplex_series.times():
        cellComplex[time] = cellComplex_series.at(time).asCellComplex()
        cellComplex_dict[time] = getattr(cellComplex[time].data(), data_attr)

    return cellComplex_dict, cellComplex


def _gnomonCellComplexInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"_in_cellComplex_series"):
            self._in_cellComplex_series, self._in_cellComplex, self._in_cellComplex_data = buildCellComplexSeries(getattr(self, attr))
        return self._in_cellComplex_series

    setattr(cls, method, func)

    def setter_func(self, cellComplex_series):
        self._in_cellComplex_series = cellComplex_series
        self._in_cellComplex = {}
        setattr(self, attr, {})

        if self._in_cellComplex_series is not None:
            cellComplex_dict, self._in_cellComplex = cellComplexDictFromSeries(self._in_cellComplex_series)
            setattr(self, attr, cellComplex_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonCellComplexInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonCellComplexInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonCellComplexInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonCellComplexOutput(cls, attr, method):
    def func(self):
        self._out_cellComplex_series, self._out_cellComplex, self._out_cellComplex_data = buildCellComplexSeries(getattr(self ,attr))
        return self._out_cellComplex_series

    setattr(cls, method, func)

    return cls


def gnomonCellComplexOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonCellComplexOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonCellComplexOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- DataFrame -------------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("dataFrameData")


def buildDataFrameSeries(dataFrame_dict, data_plugin="gnomonDataFrameDataPandas", data_setter="set_dataframe"):
    dataFrame_series = gnomonDataFrameSeries()
    dataFrame = {}
    dataFrame_data = {}

    for time in dataFrame_dict.keys():
        dataFrame[time] = gnomonDataFrame()
        dataFrame_series.insert(time, dataFrame[time])
        dataFrame_data[time] = gnomoncore.dataFrameData_pluginFactory().create(data_plugin)
        getattr(dataFrame_data[time], data_setter)(dataFrame_dict[time])
        dataFrame[time].setData(dataFrame_data[time])

    return dataFrame_series, dataFrame, dataFrame_data


def dataFrameDictFromSeries(dataFrame_series, data_attr='_df'):
    dataFrame = {}
    dataFrame_dict = {}
    for time in dataFrame_series.times():
        dataFrame[time] = dataFrame_series.at(time).asDataFrame()
        dataFrame_dict[time] = getattr(dataFrame[time].data(), data_attr)

    return dataFrame_dict, dataFrame


def _gnomonDataFrameInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self, "_in_dataFrame_series"):
            self._in_dataFrame_series, self._in_dataFrame, self._in_dataFrame_data = buildDataFrameSeries(getattr(self, attr))
        return self._in_dataFrame_series

    setattr(cls, method, func)

    def setter_func(self, dataFrame_series):
        self._in_dataFrame_series = dataFrame_series
        self._in_dataFrame = {}
        setattr(self, attr, {})

        if self._in_dataFrame_series is not None:
            dataFrame_dict, self._in_dataFrame = dataFrameDictFromSeries(self._in_dataFrame_series)
            setattr(self, attr, dataFrame_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonDataFrameInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonDataFrameInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonDataFrameInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonDataFrameOutput(cls, attr, method):
    def func(self):
        self._out_dataFrame_series, self._out_dataFrame, self._out_dataFrame_data = buildDataFrameSeries(getattr(self, attr))
        return self._out_dataFrame_series

    setattr(cls, method, func)

    return cls


def gnomonDataFrameOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonDataFrameOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonDataFrameOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- PointCloud -------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("pointCloudData")


def buildPointCloudSeries(pointCloud_dict, data_plugin="gnomonPointCloudDataPropertyTopomesh", data_setter="set_property_topomesh"):
    pointCloud_series = gnomonPointCloudSeries()
    pointCloud = {}
    pointCloud_data = {}

    for time in pointCloud_dict.keys():
        pointCloud[time] = gnomonPointCloud()
        pointCloud_series.insert(time, pointCloud[time])
        pointCloud_data[time] = gnomoncore.pointCloudData_pluginFactory().create(data_plugin)
        getattr(pointCloud_data[time],data_setter)(pointCloud_dict[time])
        pointCloud[time].setData(pointCloud_data[time])

    return pointCloud_series, pointCloud, pointCloud_data


def pointCloudDictFromSeries(pointCloud_series, data_attr='_topomesh'):
    pointCloud = {}
    pointCloud_dict = {}
    for time in pointCloud_series.times():
        pointCloud[time] = pointCloud_series.at(time).asPointCloud()
        pointCloud_dict[time] = getattr(pointCloud[time].data(),data_attr)
        
    return pointCloud_dict, pointCloud
    

def _gnomonPointCloudInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"_in_pointCloud_series"):
            self._in_pointCloud_series, self._in_pointCloud, self._in_pointCloud_data = buildPointCloudSeries(getattr(self, attr))
        return self._in_pointCloud_series

    setattr(cls, method, func)

    def setter_func(self, pointCloud_series):
        self._in_pointCloud_series = pointCloud_series
        self._in_pointCloud = {}
        setattr(self, attr, {})

        if self._pointCloud_series is not None:
            pointCloud_dict, self._in_pointCloud = pointCloudDictFromSeries(self._in_pointCloud_series)
            setattr(self, attr, pointCloud_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonPointCloudInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonPointCloudInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonPointCloudInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonPointCloudOutput(cls, attr, method):
    def func(self):
        self._out_pointCloud_series, self._out_pointCloud, self._out_pointCloud_data = buildPointCloudSeries(getattr(self, attr))
        return self._out_pointCloud_series

    setattr(cls, method, func)

    return cls


def gnomonPointCloudOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonPointCloudOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonPointCloudOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- Mesh -------------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("meshData")


def buildMeshSeries(mesh_dict, data_plugin="gnomonMeshDataPropertyTopomesh", data_setter="set_property_topomesh"):
    mesh_series = gnomonMeshSeries()
    mesh = {}
    mesh_data = {}

    for time in mesh_dict.keys():
        mesh[time] = gnomonMesh()
        mesh_series.insert(time, mesh[time])
        mesh_data[time] = gnomoncore.meshData_pluginFactory().create(data_plugin)
        getattr(mesh_data[time],data_setter)(mesh_dict[time])
        mesh[time].setData(mesh_data[time])

    return mesh_series, mesh, mesh_data


def meshDictFromSeries(mesh_series, data_attr='_mesh'):
    mesh = {}
    mesh_dict = {}
    for time in mesh_series.times():
        mesh[time] = mesh_series.at(time).asMesh()
        mesh_dict[time] = getattr(mesh[time].data(), data_attr)

    return mesh_dict, mesh


def _gnomonMeshInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self,"_in_mesh_series"):
            self._in_mesh_series, self._in_mesh, self._in_mesh_data = buildMeshSeries(getattr(self, attr))
        return self._in_mesh_series

    setattr(cls, method, func)

    def setter_func(self, mesh_series):
        self._in_mesh_series = mesh_series
        self._in_mesh = {}
        setattr(self, attr, {})

        if self._mesh_series is not None:

            mesh_dict, self._in_mesh = meshDictFromSeries(self._in_mesh_series)
            setattr(self, attr, mesh_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonMeshInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonMeshInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonMeshInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonMeshOutput(cls, attr, method):
    def func(self):
        self._out_mesh_series, self._out_mesh, self._out_mesh_data = buildMeshSeries(getattr(self ,attr))
        return self._out_mesh_series

    setattr(cls, method, func)

    return cls


def gnomonMeshOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonMeshOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonMeshOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- LString -----------------------------------
# ------------------------------------------------------------------------------

load_plugin_group("lStringData")

def buildLStringSeries(lString_dict, data_plugin="gnomonLStringDataLPy", data_setter="set_lString"):
    lString_series = gnomonLStringSeries()
    lString = {}
    lString_data = {}

    for time in lString_dict.keys():
        lString[time] = gnomonLString()
        lString_series.insert(time, lString[time])
        lString_data[time] = gnomoncore.lStringData_pluginFactory().create(data_plugin)
        getattr(lString_data[time],data_setter)(lString_dict[time])
        lString[time].setData(lString_data[time])

    return lString_series, lString, lString_data


def _gnomonLStringInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"_in_lString_series"):
            self._in_lString_series, self._in_lString, self._in_lString_data = buildLStringSeries(getattr(self, attr))

        return self._lString_series

    setattr(cls, method, func)

    def setter_func(self, lString_series):
        self._in_lString_series = lString_series
        self._in_lString = {}
        setattr(self, attr, {})

        if self._in_lString_series is not None:

            for time in self._in_lString_series.times():
                self._in_lString[time] = self._in_lString_series.at(time).asLString()
                getattr(self, attr)[time] = self._in_lString[time].data()._lString

            if hasattr(self ,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonLStringInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonLStringInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonLStringInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonLStringOutput(cls, attr, method):
    def func(self):
        self._out_lString_series, self._out_lString, self._out_lString_data = buildLStringSeries(getattr(self, attr))
        return self._out_lString_series

    setattr(cls, method, func)

    return cls


def gnomonLStringOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonLStringOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonLStringOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- Image -------------------------------------
# ------------------------------------------------------------------------------

def buildImageSeries(image_dict):
    image_series = gnomonImageSeries()
    image = {}

    for time in image_dict.keys():
        image[time] = gnomonImage()
        image_series.insert(time, image[time])
        for channel in image_dict[time].keys():
            dtk_img = sp_img_to_dtk_img(image_dict[time][channel])
            image[time].setImage(dtk_img, channel)

    return image_series, image


def imageDictFromSeries(image_series, converter_func=dtk_img_to_sp_img):
    image = {}
    image_dict = {}
    for time in image_series.times():
        image[time] = image_series.at(time).asImage()
        image_dict[time] = {}
        for channel in image[time].channels():
            image_dict[time][channel] = converter_func(image[time].image(channel))
    return image_dict, image


def _gnomonImageInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self, "_in_image_series"):
            self._in_image_series, self._in_image = buildImageSeries(getattr(self ,attr))
        return self._in_image_series

    setattr(cls, method, func)

    def setter_func(self, image_series):
        self._in_image_series = image_series
        self._in_image = {}
        setattr(self, attr, {})

        if self._in_image_series is not None:

            image_dict, self._in_image = imageDictFromSeries(image_series)
            setattr(self, attr, image_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonImageInput(cls=None, attr=None, method='input', setter_method='setInput'):
    if cls is not None:
        return _gnomonImageInput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonImageInput(cls, attr, method, setter_method)

        return wrapper


def _gnomonImageOutput(cls, attr, method):
    def func(self):
        self._out_image_series, self._out_image = buildImageSeries(getattr(self, attr))
        return self._out_image_series

    setattr(cls, method, func)

    return cls


def gnomonImageOutput(cls=None, attr=None, method='output'):
    if cls is not None:
        return _gnomonImageOutput(cls, attr)
    else:
        def wrapper(cls):
            return _gnomonImageOutput(cls, attr, method)

        return wrapper


# ------------------------------------------------------------------------------
# --------------------------------- FormDict ----------------------------------
# ------------------------------------------------------------------------------

def _gnomonFormDictOutput(cls, form_attr, method):
    def func(self):
        self._form_series = {}

        for form in form_attr.keys():
            if form == 'gnomonImage':
                self._form_series[form],_,_ = buildImageSeries(getattr(self ,form_attr[form]))
            elif form == 'gnomonMesh':
                self._form_series[form],_,_ = buildMeshSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonPointCloud':
                self._form_series[form],_,_ = buildPointCloudSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonTree':
                self._form_series[form],_,_ = buildTreeSeries(getattr(self ,form_attr[form]))

        return self._form_series

    setattr(cls, method, func)

    return cls


def gnomonFormDictOutput(cls=None, form_attr={}, method='forms'):
    if cls is not None:
        return _gnomonFormDictOutput(cls, form_attr)
    else:
        def wrapper(cls):
            return _gnomonFormDictOutput(cls, form_attr, method)

        return wrapper

# ------------------------------------------------------------------------------
