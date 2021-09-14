import gnomoncore

import logging

from gnomoncore import gnomonMesh
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("meshData")

default_plugin = "gnomonMeshDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"

import gnomoncore

import logging

from gnomoncore import gnomonCellComplex
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("cellComplexData")

default_plugin = "gnomonCellComplexDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"


def buildMeshSeries(mesh_dict, data_plugin=default_plugin, data_setter=default_setter, form_dict=None):
    mesh = {}
    mesh_data = {}

    for time in mesh_dict.keys():
        if form_dict is None:
            mesh[time] = gnomonMesh()
            mesh_data[time] = gnomoncore.meshData_pluginFactory().create(data_plugin)
            mesh[time].setData(mesh_data[time])
        else:
            mesh[time] = form_dict[time]
            mesh_data[time] = mesh[time].data()
        getattr(mesh_data[time], data_setter)(mesh_dict[time])

    return mesh, mesh_data


def meshDictFromSeries(mesh, data_plugin=default_plugin, data_attr=default_attr):
    mesh_dict = {}
    for time in mesh.keys():
        if hasattr(mesh[time].data(), data_attr):
            mesh_dict[time] = getattr(mesh[time].data(), data_attr)
        else:
            mesh_data = gnomoncore.meshData_pluginFactory().create(data_plugin).from_gnomonMesh(mesh[time])
            mesh_dict[time] = getattr(mesh_data, data_attr)

    return mesh_dict


def _gnomonMeshInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_mesh")
        if update:
            form_dict, data_dict = buildMeshSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_mesh = form_dict
            self._in_mesh_data = data_dict
        return self._in_mesh

    setattr(cls, method, func)

    def setter_func(self, mesh):
        self._in_mesh = mesh
        setattr(self, attr, {})

        if self._in_mesh is not None:
            mesh_dict = meshDictFromSeries(self._in_mesh, data_plugin, data_attr)
            setattr(self, attr, mesh_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonMeshInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonMeshInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonMeshInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonMeshOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_mesh")
        if update:
            form_dict, data_dict = buildMeshSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_mesh = form_dict
            self._out_mesh_data = data_dict
        return self._out_mesh

    setattr(cls, method, func)

    return cls


def gnomonMeshOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonMeshOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonMeshOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper

