import gnomoncore

import logging

from gnomoncore import gnomonLString, gnomonLStringSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("lStringData")

default_plugin = "gnomonLStringDataLPy"
default_setter = "set_lstring"
default_attr = "_lstring"


def buildLStringSeries(lString_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    lString = {}
    lString_data = {}
    if form_series is None:
        lString_series = gnomonLStringSeries()
    else:
        lString_series = form_series
        current_time = lString_series.time()

    for time in lString_dict.keys():
        if form_series is None:
            lString[time] = gnomonLString()
            lString_series.insert(time, lString[time])
            lString_data[time] = gnomoncore.lStringData_pluginFactory().create(data_plugin)
            lString[time].setData(lString_data[time])
        else:
            lString[time] = lString_series.at(time).asLString()
            lString_data[time] = lString[time].data()
        getattr(lString_data[time], data_setter)(lString_dict[time])
    if form_series is not None:
       lString_series.at(current_time)

    return lString_series, lString, lString_data


def lStringDictFromSeries(lString_series, data_plugin=default_plugin, data_attr=default_attr):
    lString = {}
    lString_dict = {}
    for time in lString_series.times():
        lString[time] = lString_series.at(time).asLString()
        if hasattr(lString[time].data(), data_attr):
            lString_dict[time] = getattr(lString[time].data(), data_attr)
        else:
            lString_data = gnomoncore.lStringData_pluginFactory().create(data_plugin).from_gnomonLString(lString[time])
            lString_dict[time] = getattr(lString_data, data_attr)

    return lString_dict, lString


def _gnomonLStringInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_lString_series")
        if update:
            form_series, form_dict, data_dict = buildLStringSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_lString_series = form_series
            self._in_lString = form_dict
            self._in_lString_data = data_dict
        return self._in_lString_series

    setattr(cls, method, func)

    def setter_func(self, lString_series):
        self._in_lString_series = lString_series
        self._in_lString = {}
        setattr(self, attr, {})

        if self._in_lString_series is not None:
            lString_dict, form_dict = lStringDictFromSeries(self._in_lString_series, data_plugin, data_attr)
            self._in_lString = form_dict
            setattr(self, attr, lString_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonLStringInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonLStringInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonLStringInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonLStringOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_lString_series")
        if update:
            form_series, form_dict, data_dict = buildLStringSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_lString_series = form_series
            self._out_lString = form_dict
            self._out_lString_data = data_dict
        return self._out_lString_series

    setattr(cls, method, func)

    return cls


def gnomonLStringOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonLStringOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonLStringOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper

