import gnomoncore

from gnomoncore import gnomonPointCloud
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("pointCloudData")

default_plugin = "gnomonPointCloudDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"

form_class = gnomonPointCloud
form_data_factory = gnomoncore.pointCloudData_pluginFactory()
from_form_method = "from_gnomonPointCloud"


def _gnomonPointCloudInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_pointCloud")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_pointCloud = form_dict
            self._in_pointCloud_data = data_dict
        return self._in_pointCloud

    setattr(cls, method, func)

    def setter_func(self, pointCloud):
        self._in_pointCloud = pointCloud
        setattr(self, attr, {})

        if self._in_pointCloud is not None:
            pointCloud_dict = formDictFromSeries(form=self._in_pointCloud,
                                            form_data_factory=form_data_factory,
                                            from_form_method=from_form_method,
                                            data_plugin=data_plugin,
                                            data_attr=data_attr)
            setattr(self, attr, pointCloud_dict)

            if hasattr(self, "refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def pointCloudInput(attr, methods=(None, None), data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonPointCloudInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

    return decorator


def _gnomonPointCloudOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_pointCloud")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_pointCloud = form_dict
            self._out_pointCloud_data = data_dict
        return self._out_pointCloud

    setattr(cls, method, func)

    return cls


def pointCloudOutput(attr, method=None, data_plugin=default_plugin, data_setter=default_setter):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonPointCloudOutput(cls, attr, bound_method, data_plugin=data_plugin, data_setter=data_setter)

    return decorator
