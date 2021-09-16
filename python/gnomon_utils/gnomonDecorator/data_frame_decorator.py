import gnomoncore

from gnomoncore import gnomonDataFrame
from gnomon_utils.gnomonPlugin import load_plugin_group

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


def gnomonDataFrameInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonDataFrameInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonDataFrameInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


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


def gnomonDataFrameOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonDataFrameOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonDataFrameOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper
