import gnomoncore

from gnomoncore import (gnomonImage, gnomonImageSeries, gnomonLString,
                        gnomonLStringSeries, gnomonMesh, gnomonMeshSeries,
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


def _gnomonTreeInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self ,"tree_series"):
            self.tree_series, self.tree, self.tree_data = buildTreeSeries(getattr(self, attr))
        return self.tree_series

    setattr(cls, method, func)

    def setter_func(self, tree_series):
        self.tree_series = tree_series
        self.tree = {}
        setattr(self, attr, {})

        if self.tree_series is not None:

            for time in self.tree_series.times():
                self.tree[time] = self.tree_series.at(time).asTree()
                getattr(self, attr)[time] = self.tree[time].data()._tree

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
        self.tree_series, self.tree, self.tree_data = buildTreeSeries(getattr(self, attr))
        return self.tree_series

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


def _gnomonPointCloudOutput(cls, attr, method):
    def func(self):
        self.pointCloud_series, self.pointCloud, self.pointCloud_data = buildPointCloudSeries(getattr(self, attr))
        return self.pointCloud_series

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


def _gnomonMeshOutput(cls, attr, method):
    def func(self):
        self.mesh_series, self.mesh, self.mesh_data = buildMeshSeries(getattr(self ,attr))
        return self.mesh_series

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
        if not hasattr(self ,"lString_series"):
            self.lString_series, self.lString, self.lString_data = buildLStringSeries(getattr(self, attr))

        return self.lString_series

    setattr(cls, method, func)

    def setter_func(self, lString_series):
        self.lString_series = lString_series
        self.lString = {}
        setattr(self, attr, {})

        if self.lString_series is not None:

            for time in self.lString_series.times():
                self.lString[time] = self.lString_series.at(time).asLString()
                getattr(self, attr)[time] = self.lString[time].data()._lString

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
        self.lString_series, self.lString, self.lString_data = buildLStringSeries(getattr(self, attr))
        return self.lString_series

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


def _gnomonImageInput(cls, attr, method, setter_method):
    def func(self):
        if not hasattr(self, "image_series"):
            self.image_series, self.image = buildImageSeries(getattr(self ,attr))
        return self.image_series

    setattr(cls, method, func)

    def setter_func(self, image_series):
        self.image_series = image_series
        self.image = {}
        setattr(self, attr, {})

        if self.image_series is not None:

            for time in self.image_series.times():
                self.image[time] = self.image_series.at(time).asImage()
                getattr(self, attr)[time] = {}
                for channel in self.image[time].channels():
                    getattr(self, attr)[time][channel] = dtk_img_to_sp_img(self.image[time].image(channel))

            if hasattr(self ,"refresh_parameters"):
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
        self.image_series, self.image = buildImageSeries(getattr(self, attr))
        return self.image_series

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
        self.form_series = {}

        for form in form_attr.keys():
            if form == 'gnomonImage':
                self.form_series[form],_,_ = buildImageSeries(getattr(self ,form_attr[form]))
            elif form == 'gnomonMesh':
                self.form_series[form],_,_ = buildMeshSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonPointCloud':
                self.form_series[form],_,_ = buildPointCloudSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonTree':
                self.form_series[form],_,_ = buildTreeSeries(getattr(self ,form_attr[form]))

        return self.form_series

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
