import gnomoncore

from gnomoncore import gnomonTree
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified

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
        if is_form_series_modified(self, "_in_tree", tree):
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


def treeInput(attr, methods=(None, None), data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonTreeInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

    return decorator


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


def treeOutput(attr, method=None, data_plugin=default_plugin, data_setter=default_setter):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonTreeOutput(cls, attr, bound_method, data_plugin=data_plugin, data_setter=data_setter)

    return decorator
