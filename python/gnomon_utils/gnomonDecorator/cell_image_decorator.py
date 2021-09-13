import gnomoncore

import logging

from gnomoncore import gnomonCellImage, gnomonCellImageSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("cellImageData")

default_plugin = "gnomonCellImageDataPropertySpatialImage"
default_setter = "set_property_image"
default_attr = "_p_img"


def buildCellImageSeries(cellImage_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    cellImage = {}
    cellImage_data = {}
    if form_series is None:
        cellImage_series = gnomonCellImageSeries()
    else:
        cellImage_series = form_series
        current_time = cellImage_series.time()

    for time in cellImage_dict.keys():
        if form_series is None:
            cellImage[time] = gnomonCellImage()
            cellImage_series.insert(time, cellImage[time])
            cellImage_data[time] = gnomoncore.cellImageData_pluginFactory().create(data_plugin)
            cellImage[time].setData(cellImage_data[time])
        else:
            cellImage[time] = cellImage_series.at(time).asCellImage()
            cellImage_data[time] = cellImage[time].data()
        getattr(cellImage_data[time], data_setter)(cellImage_dict[time])
    if form_series is not None:
       cellImage_series.at(current_time)

    return cellImage_series, cellImage, cellImage_data


def cellImageDictFromSeries(cellImage_series, data_plugin=default_plugin, data_attr=default_attr):
    cellImage = {}
    cellImage_dict = {}
    for time in cellImage_series.times():
        cellImage[time] = cellImage_series.at(time).asCellImage()
        if hasattr(cellImage[time].data(), data_attr):
            cellImage_dict[time] = getattr(cellImage[time].data(), data_attr)
        else:
            cellImage_data = gnomoncore.cellImageData_pluginFactory().create(data_plugin).from_gnomonCellImage(cellImage[time])
            cellImage_dict[time] = getattr(cellImage_data, data_attr)

    return cellImage_dict, cellImage


def _gnomonCellImageInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_cellImage_series")
        if update:
            form_series, form_dict, data_dict = buildCellImageSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_cellImage_series = form_series
            self._in_cellImage = form_dict
            self._in_cellImage_data = data_dict
        return self._in_cellImage_series

    setattr(cls, method, func)

    def setter_func(self, cellImage_series):
        self._in_cellImage_series = cellImage_series
        self._in_cellImage = {}
        setattr(self, attr, {})

        if self._in_cellImage_series is not None:
            cellImage_dict, form_dict = cellImageDictFromSeries(self._in_cellImage_series, data_plugin, data_attr)
            self._in_cellImage = form_dict
            setattr(self, attr, cellImage_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonCellImageInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonCellImageInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonCellImageInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonCellImageOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_cellImage_series")
        if update:
            form_series, form_dict, data_dict = buildCellImageSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_cellImage_series = form_series
            self._out_cellImage = form_dict
            self._out_cellImage_data = data_dict
        return self._out_cellImage_series

    setattr(cls, method, func)

    return cls


def gnomonCellImageOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonCellImageOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonCellImageOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper


