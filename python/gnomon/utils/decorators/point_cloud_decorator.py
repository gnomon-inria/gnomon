import gnomon.core

from gnomon.core import gnomonPointCloud
from gnomon.utils.gnomonPlugin import default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified, getFormDataClass

plugin_group = "pointCloudData"
form_class = gnomonPointCloud
form_data_factory = gnomon.core.pointCloudData_pluginFactory()


def _gnomonPointCloudInput(cls, attr, method, setter_method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_in_pointCloud")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._in_pointCloud = form_dict
            self._in_pointCloud_data = data_dict
        return self._in_pointCloud

    setattr(cls, method, func)

    def setter_func(self, pointCloud):
        if is_form_series_modified(self, "_in_pointCloud", pointCloud):
            self._in_pointCloud = pointCloud
            setattr(self, attr, {})

            if self._in_pointCloud is not None:
                pointCloud_dict = formDictFromSeries(form=self._in_pointCloud, data_plugin=data_plugin)
                setattr(self, attr, pointCloud_dict)

                if hasattr(self, "refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def pointCloudInput(attr, data_plugin, methods=(None, None)):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonPointCloudInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin)

    return decorator


def _gnomonPointCloudOutput(cls, attr, method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_out_pointCloud")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._out_pointCloud = form_dict
            self._out_pointCloud_data = data_dict
        return self._out_pointCloud

    setattr(cls, method, func)

    return cls


def pointCloudOutput(attr, data_plugin, method=None):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonPointCloudOutput(cls, attr, bound_method, data_plugin=data_plugin)

    return decorator
