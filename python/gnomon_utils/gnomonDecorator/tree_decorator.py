import gnomoncore

from gnomoncore import gnomonTree
from gnomon_utils.gnomonPlugin import load_plugin_group

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("treeData")

default_plugin = "gnomonTreeDataTreex"
default_setter = "set_tree"
default_attr = "_tree"

form_class = gnomonTree
form_data_factory = gnomoncore.treeData_pluginFactory()
from_form_method = "from_gnomonTree"


def _gnomonTreeInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_tree")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_tree = form_dict
            self._in_tree_data = data_dict
        return self._in_tree

    setattr(cls, method, func)

    def setter_func(self, tree):
        self._in_tree = tree
        setattr(self, attr, {})

        if self._in_tree is not None:
            tree_dict = formDictFromSeries(form=self._in_tree,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
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
        update = update or not hasattr(self, "_out_tree")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_tree = form_dict
            self._out_tree_data = data_dict
        return self._out_tree

    setattr(cls, method, func)

    return cls


def gnomonTreeOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonTreeOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonTreeOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper
