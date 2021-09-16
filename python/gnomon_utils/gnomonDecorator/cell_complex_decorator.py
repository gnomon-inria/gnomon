import gnomoncore

from gnomoncore import gnomonCellComplex
from gnomon_utils.gnomonPlugin import load_plugin_group

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("cellComplexData")

default_plugin = "gnomonCellComplexDataPropertyTopomesh"
default_setter = "set_property_topomesh"
default_attr = "_topomesh"

form_class = gnomonCellComplex
form_data_factory = gnomoncore.cellComplexData_pluginFactory()
from_form_method = "from_gnomonCellComplex"


def _gnomonCellComplexInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_cellComplex")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_cellComplex = form_dict
            self._in_cellComplex_data = data_dict
        return self._in_cellComplex

    setattr(cls, method, func)

    def setter_func(self, cellComplex):
        self._in_cellComplex = cellComplex
        setattr(self, attr, {})

        if self._in_cellComplex is not None:
            cellComplex_dict = formDictFromSeries(form=self._in_cellComplex,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
            setattr(self, attr, cellComplex_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonCellComplexInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonCellComplexInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonCellComplexInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonCellComplexOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_cellComplex")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_cellComplex = form_dict
            self._out_cellComplex_data = data_dict
        return self._out_cellComplex

    setattr(cls, method, func)

    return cls


def gnomonCellComplexOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonCellComplexOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonCellComplexOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper
