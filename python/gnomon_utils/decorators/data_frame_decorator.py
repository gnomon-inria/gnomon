import gnomoncore

from gnomoncore import gnomonDataFrame
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("dataFrameData")

default_plugin = "gnomonDataFrameDataPandas"
default_setter = "set_dataframe"
default_attr = "_df"

form_class = gnomonDataFrame
form_data_factory = gnomoncore.dataFrameData_pluginFactory()
from_form_method = "from_gnomonDataFrame"


def _gnomonDataFrameInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_dataFrame")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_dataFrame = form_dict
            self._in_dataFrame_data = data_dict
        return self._in_dataFrame

    setattr(cls, method, func)

    def setter_func(self, dataFrame):
        self._in_dataFrame = dataFrame
        setattr(self, attr, {})

        if self._in_dataFrame is not None:
            dataFrame_dict = formDictFromSeries(form=self._in_dataFrame,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
            setattr(self, attr, dataFrame_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def dataFrameInput(attr, methods=(None, None), data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonDataFrameInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

    return decorator


def _gnomonDataFrameOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_dataFrame")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_dataFrame = form_dict
            self._out_dataFrame_data = data_dict
        return self._out_dataFrame

    setattr(cls, method, func)

    return cls


def dataFrameOutput(attr, method=None, data_plugin=default_plugin, data_setter=default_setter):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonDataFrameOutput(cls, attr, bound_method, data_plugin=data_plugin, data_setter=data_setter)

    return decorator
