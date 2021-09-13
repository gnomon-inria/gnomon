import gnomoncore

import logging

from gnomoncore import gnomonMesh, gnomonMeshSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("meshData")

default_plugin = "gnomonMeshDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"


def buildMeshSeries(mesh_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    mesh = {}
    mesh_data = {}
    if form_series is None:
        mesh_series = gnomonMeshSeries()
    else:
        mesh_series = form_series
        current_time = mesh_series.time()

    for time in mesh_dict.keys():
        if form_series is None:
            mesh[time] = gnomonMesh()
            mesh_series.insert(time, mesh[time])
            mesh_data[time] = gnomoncore.meshData_pluginFactory().create(data_plugin)
            mesh[time].setData(mesh_data[time])
        else:
            mesh[time] = mesh_series.at(time).asMesh()
            mesh_data[time] = mesh[time].data()
        getattr(mesh_data[time], data_setter)(mesh_dict[time])
    if form_series is not None:
       mesh_series.at(current_time)

    return mesh_series, mesh, mesh_data


def meshDictFromSeries(mesh_series, data_plugin=default_plugin, data_attr=default_attr):
    mesh = {}
    mesh_dict = {}
    for time in mesh_series.times():
        mesh[time] = mesh_series.at(time).asMesh()
        if hasattr(mesh[time].data(), data_attr):
            mesh_dict[time] = getattr(mesh[time].data(), data_attr)
        else:
            mesh_data = gnomoncore.meshData_pluginFactory().create(data_plugin).from_gnomonMesh(mesh[time])
            mesh_dict[time] = getattr(mesh_data, data_attr)

    return mesh_dict, mesh


def _gnomonMeshInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_mesh_series")
        if update:
            form_series, form_dict, data_dict = buildMeshSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_mesh_series = form_series
            self._in_mesh = form_dict
            self._in_mesh_data = data_dict
        return self._in_mesh_series

    setattr(cls, method, func)

    def setter_func(self, mesh_series):
        self._in_mesh_series = mesh_series
        self._in_mesh = {}
        setattr(self, attr, {})

        if self._in_mesh_series is not None:
            mesh_dict, form_dict = meshDictFromSeries(self._in_mesh_series, data_plugin, data_attr)
            self._in_mesh = form_dict
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
        update = update or not hasattr(self, "_out_mesh_series")
        if update:
            form_series, form_dict, data_dict = buildMeshSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_mesh_series = form_series
            self._out_mesh = form_dict
            self._out_mesh_data = data_dict
        return self._out_mesh_series

    setattr(cls, method, func)

    return cls


def gnomonMeshOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonMeshOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonMeshOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper



