import gnomoncore

import logging

from gnomoncore import gnomonDataFrame, gnomonDataFrameSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("dataFrameData")

default_plugin = "gnomonDataFrameDataPandas"
default_setter = "set_dataframe"
default_attr = "_df"


def buildDataFrameSeries(dataFrame_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    dataFrame = {}
    dataFrame_data = {}
    if form_series is None:
        dataFrame_series = gnomonDataFrameSeries()
    else:
        dataFrame_series = form_series
        current_time = dataFrame_series.time()

    for time in dataFrame_dict.keys():
        if form_series is None:
            dataFrame[time] = gnomonDataFrame()
            dataFrame_series.insert(time, dataFrame[time])
            dataFrame_data[time] = gnomoncore.dataFrameData_pluginFactory().create(data_plugin)
            dataFrame[time].setData(dataFrame_data[time])
        else:
            dataFrame[time] = dataFrame_series.at(time).asDataFrame()
            dataFrame_data[time] = dataFrame[time].data()
        getattr(dataFrame_data[time], data_setter)(dataFrame_dict[time])
    if form_series is not None:
       dataFrame_series.at(current_time)

    return dataFrame_series, dataFrame, dataFrame_data


def dataFrameDictFromSeries(dataFrame_series, data_plugin=default_plugin, data_attr=default_attr):
    dataFrame = {}
    dataFrame_dict = {}
    for time in dataFrame_series.times():
        dataFrame[time] = dataFrame_series.at(time).asDataFrame()
        if hasattr(dataFrame[time].data(), data_attr):
            dataFrame_dict[time] = getattr(dataFrame[time].data(), data_attr)
        else:
            dataFrame_data = gnomoncore.dataFrameData_pluginFactory().create(data_plugin).from_gnomonDataFrame(dataFrame[time])
            dataFrame_dict[time] = getattr(dataFrame_data, data_attr)

    return dataFrame_dict, dataFrame


def _gnomonDataFrameInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_dataFrame_series")
        if update:
            form_series, form_dict, data_dict = buildDataFrameSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_dataFrame_series = form_series
            self._in_dataFrame = form_dict
            self._in_dataFrame_data = data_dict
        return self._in_dataFrame_series

    setattr(cls, method, func)

    def setter_func(self, dataFrame_series):
        self._in_dataFrame_series = dataFrame_series
        self._in_dataFrame = {}
        setattr(self, attr, {})

        if self._in_dataFrame_series is not None:
            dataFrame_dict, form_dict = dataFrameDictFromSeries(self._in_dataFrame_series, data_plugin, data_attr)
            self._in_dataFrame = form_dict
            setattr(self, attr, dataFrame_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonDataFrameInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonDataFrameInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonDataFrameInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonDataFrameOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_dataFrame_series")
        if update:
            form_series, form_dict, data_dict = buildDataFrameSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_dataFrame_series = form_series
            self._out_dataFrame = form_dict
            self._out_dataFrame_data = data_dict
        return self._out_dataFrame_series

    setattr(cls, method, func)

    return cls


def gnomonDataFrameOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonDataFrameOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonDataFrameOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper

