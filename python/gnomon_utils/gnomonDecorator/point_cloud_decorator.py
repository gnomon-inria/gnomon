import gnomoncore

import logging

from gnomoncore import gnomonPointCloud, gnomonPointCloudSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("pointCloudData")

default_plugin = "gnomonPointCloudDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"


def buildPointCloudSeries(pointCloud_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    pointCloud = {}
    pointCloud_data = {}
    if form_series is None:
        pointCloud_series = gnomonPointCloudSeries()
    else:
        pointCloud_series = form_series
        current_time = pointCloud_series.time()

    for time in pointCloud_dict.keys():
        if form_series is None:
            pointCloud[time] = gnomonPointCloud()
            pointCloud_series.insert(time, pointCloud[time])
            pointCloud_data[time] = gnomoncore.pointCloudData_pluginFactory().create(data_plugin)
            pointCloud[time].setData(pointCloud_data[time])
        else:
            pointCloud[time] = pointCloud_series.at(time).asPointCloud()
            pointCloud_data[time] = pointCloud[time].data()
        getattr(pointCloud_data[time], data_setter)(pointCloud_dict[time])
    if form_series is not None:
       pointCloud_series.at(current_time)

    return pointCloud_series, pointCloud, pointCloud_data


def pointCloudDictFromSeries(pointCloud_series, data_plugin=default_plugin, data_attr=default_attr):
    pointCloud = {}
    pointCloud_dict = {}
    for time in pointCloud_series.times():
        pointCloud[time] = pointCloud_series.at(time).asPointCloud()
        if hasattr(pointCloud[time].data(), data_attr):
            pointCloud_dict[time] = getattr(pointCloud[time].data(), data_attr)
        else:
            pointCloud_data = gnomoncore.pointCloudData_pluginFactory().create(data_plugin).from_gnomonPointCloud(pointCloud[time])
            pointCloud_dict[time] = getattr(pointCloud_data, data_attr)

    return pointCloud_dict, pointCloud


def _gnomonPointCloudInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_pointCloud_series")
        if update:
            form_series, form_dict, data_dict = buildPointCloudSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_pointCloud_series = form_series
            self._in_pointCloud = form_dict
            self._in_pointCloud_data = data_dict
        return self._in_pointCloud_series

    setattr(cls, method, func)

    def setter_func(self, pointCloud_series):
        self._in_pointCloud_series = pointCloud_series
        self._in_pointCloud = {}
        setattr(self, attr, {})

        if self._in_pointCloud_series is not None:
            pointCloud_dict, form_dict = pointCloudDictFromSeries(self._in_pointCloud_series, data_plugin, data_attr)
            self._in_pointCloud = form_dict
            setattr(self, attr, pointCloud_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonPointCloudInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonPointCloudInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonPointCloudInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonPointCloudOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_pointCloud_series")
        if update:
            form_series, form_dict, data_dict = buildPointCloudSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_pointCloud_series = form_series
            self._out_pointCloud = form_dict
            self._out_pointCloud_data = data_dict
        return self._out_pointCloud_series

    setattr(cls, method, func)

    return cls


def gnomonPointCloudOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonPointCloudOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonPointCloudOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper


