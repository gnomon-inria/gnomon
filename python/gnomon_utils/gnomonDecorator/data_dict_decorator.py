import gnomoncore

from gnomoncore import gnomonDataDict
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("dataDictData")

default_plugin = "gnomonNumpyDataDictData"
default_setter = "set_data"
default_attr = "_data"

form_class = gnomonDataDict
form_data_factory = gnomoncore.dataDictData_pluginFactory()
from_form_method = "from_gnomonDataDict"


def _gnomonDataDictInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_dataDict")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_dataDict = form_dict
            self._in_dataDict_data = data_dict
        return self._in_dataDict

    setattr(cls, method, func)

    def setter_func(self, dataDict):
        self._in_dataDict = dataDict
        setattr(self, attr, {})

        if self._in_dataDict is not None:
            dataDict_dict = formDictFromSeries(form=self._in_dataDict,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
            setattr(self, attr, dataDict_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonDataDictInput(attr, method=None, setter_method=None, data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    def wrapper(cls):
        if method is None or setter_method is None:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        else:
            local_getter_method, local_setter_method = method, setter_method

        return _gnomonDataDictInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

    return wrapper



def _gnomonDataDictOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_dataDict")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_dataDict = form_dict
            self._out_dataDict_data = data_dict
        return self._out_dataDict

    setattr(cls, method, func)

    return cls


def gnomonDataDictOutput(attr, method=None, data_plugin=default_plugin, data_setter=default_setter):
    def wrapper(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonDataDictOutput(cls, attr, bound_method, data_plugin=data_plugin, data_setter=data_setter)

    return wrapper
