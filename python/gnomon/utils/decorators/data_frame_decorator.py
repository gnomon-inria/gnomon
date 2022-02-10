import gnomon.core

from gnomon.core import gnomonDataFrame
from gnomon.utils.gnomonPlugin import default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified, getFormDataClass

plugin_group = "dataFrameData"
form_class = gnomonDataFrame
form_data_factory = gnomon.core.dataFrameData_pluginFactory()


def _gnomonDataFrameInput(cls, attr, method, setter_method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_in_dataFrame")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._in_dataFrame = form_dict
            self._in_dataFrame_data = data_dict
        return self._in_dataFrame

    setattr(cls, method, func)

    def setter_func(self, dataFrame):
        if is_form_series_modified(self, "_in_dataFrame", dataFrame):
            self._in_dataFrame = dataFrame
            setattr(self, attr, {})

            if self._in_dataFrame is not None:
                dataFrame_dict = formDictFromSeries(form=self._in_dataFrame, data_plugin=data_plugin)
                setattr(self, attr, dataFrame_dict)

                if hasattr(self,"refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def dataFrameInput(attr, data_plugin, methods=(None, None)):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonDataFrameInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin)

    return decorator


def _gnomonDataFrameOutput(cls, attr, method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_out_dataFrame")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._out_dataFrame = form_dict
            self._out_dataFrame_data = data_dict
        return self._out_dataFrame

    setattr(cls, method, func)

    return cls


def dataFrameOutput(attr, data_plugin, method=None):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonDataFrameOutput(cls, attr, bound_method, data_plugin=data_plugin)

    return decorator
