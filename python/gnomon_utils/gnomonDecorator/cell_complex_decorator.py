import gnomoncore

import logging

from gnomoncore import gnomonCellComplex, gnomonCellComplexSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("cellComplexData")

default_plugin = "gnomonCellComplexDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"


def buildCellComplexSeries(cellComplex_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    cellComplex = {}
    cellComplex_data = {}
    if form_series is None:
        cellComplex_series = gnomonCellComplexSeries()
    else:
        cellComplex_series = form_series

    for time in cellComplex_dict.keys():
        if form_series is None:
            cellComplex[time] = gnomonCellComplex()
            cellComplex_series.insert(time, cellComplex[time])
            cellComplex_data[time] = gnomoncore.cellComplexData_pluginFactory().create(data_plugin)
            cellComplex[time].setData(cellComplex_data[time])
        else:
            cellComplex[time] = cellComplex_series.at(time)
            cellComplex_data[time] = cellComplex[time].data()
        getattr(cellComplex_data[time], data_setter)(cellComplex_dict[time])

    return cellComplex_series, cellComplex, cellComplex_data


def cellComplexDictFromSeries(cellComplex_series, data_plugin=default_plugin, data_attr=default_attr):
    cellComplex = {}
    cellComplex_dict = {}
    for time in cellComplex_series.times():
        cellComplex[time] = cellComplex_series.at(time).asCellComplex()
        if hasattr(cellComplex[time].data(), data_attr):
            cellComplex_dict[time] = getattr(cellComplex[time].data(), data_attr)
        else:
            cellComplex_data = gnomoncore.cellComplexData_pluginFactory().create(data_plugin).from_gnomonCellComplex(cellComplex[time])
            cellComplex_dict[time] = getattr(cellComplex_data, data_attr)

    return cellComplex_dict, cellComplex


def _gnomonCellComplexInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self):
        if not hasattr(self,"_in_cellComplex_series"):
            self._in_cellComplex_series, self._in_cellComplex, self._in_cellComplex_data = buildCellComplexSeries(getattr(self, attr), data_plugin, data_setter)
        return self._in_cellComplex_series

    setattr(cls, method, func)

    def setter_func(self, cellComplex_series):
        self._in_cellComplex_series = cellComplex_series
        self._in_cellComplex = {}
        setattr(self, attr, {})

        if self._in_cellComplex_series is not None:

            cellComplex_dict, self._in_cellComplex = cellComplexDictFromSeries(self._in_cellComplex_series, data_plugin, data_attr)
            setattr(self, attr, cellComplex_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonCellComplexInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonCellComplexInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonCellComplexInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonCellComplexOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, clone=True):
        form_series = self._out_cellComplex_series if not clone else None
        form_series, form_dict, data_dict = buildCellComplexSeries(getattr(self, attr),
                                                                   data_plugin,
                                                                   data_setter,
                                                                   form_series=form_series)
        self._out_cellComplex_series = form_series
        self._out_cellComplex = form_dict
        self._out_cellComplex_data = data_dict
        return self._out_cellComplex_series

    setattr(cls, method, func)

    return cls


def gnomonCellComplexOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonCellComplexOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonCellComplexOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper

