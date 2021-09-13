import gnomoncore

import logging

from gnomoncore import gnomonTree, gnomonTreeSeries
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("treeData")

default_plugin = "gnomonTreeDataTreex"
default_setter = "set_tree"
default_attr = "_tree"


def buildTreeSeries(tree_dict, data_plugin=default_plugin, data_setter=default_setter, form_series=None):
    tree = {}
    tree_data = {}
    if form_series is None:
        tree_series = gnomonTreeSeries()
    else:
        tree_series = form_series
        current_time = tree_series.time()

    for time in tree_dict.keys():
        if form_series is None:
            tree[time] = gnomonTree()
            tree_series.insert(time, tree[time])
            tree_data[time] = gnomoncore.treeData_pluginFactory().create(data_plugin)
            tree[time].setData(tree_data[time])
        else:
            tree[time] = tree_series.at(time).asTree()
            tree_data[time] = tree[time].data()
        getattr(tree_data[time], data_setter)(tree_dict[time])
    if form_series is not None:
       tree_series.at(current_time)

    return tree_series, tree, tree_data


def treeDictFromSeries(tree_series, data_plugin=default_plugin, data_attr=default_attr):
    tree = {}
    tree_dict = {}
    for time in tree_series.times():
        tree[time] = tree_series.at(time).asTree()
        if hasattr(tree[time].data(), data_attr):
            tree_dict[time] = getattr(tree[time].data(), data_attr)
        else:
            tree_data = gnomoncore.treeData_pluginFactory().create(data_plugin).from_gnomonTree(tree[time])
            tree_dict[time] = getattr(tree_data, data_attr)

    return tree_dict, tree


def _gnomonTreeInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_tree_series")
        if update:
            form_series, form_dict, data_dict = buildTreeSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_tree_series = form_series
            self._in_tree = form_dict
            self._in_tree_data = data_dict
        return self._in_tree_series

    setattr(cls, method, func)

    def setter_func(self, tree_series):
        self._in_tree_series = tree_series
        self._in_tree = {}
        setattr(self, attr, {})

        if self._in_tree_series is not None:
            tree_dict, form_dict = treeDictFromSeries(self._in_tree_series, data_plugin, data_attr)
            self._in_tree = form_dict
            setattr(self, attr, tree_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonTreeInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonTreeInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonTreeInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonTreeOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_tree_series")
        if update:
            form_series, form_dict, data_dict = buildTreeSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_tree_series = form_series
            self._out_tree = form_dict
            self._out_tree_data = data_dict
        return self._out_tree_series

    setattr(cls, method, func)

    return cls


def gnomonTreeOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonTreeOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonTreeOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper

