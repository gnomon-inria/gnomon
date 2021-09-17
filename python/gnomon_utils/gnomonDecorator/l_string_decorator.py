import gnomoncore

from gnomoncore import gnomonLString
from gnomon_utils.gnomonPlugin import load_plugin_group

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("lStringData")

default_plugin = "gnomonLStringDataLPy"
default_setter = "set_lstring"
default_attr = "_lstring"

form_class = gnomonLString
form_data_factory = gnomoncore.lStringData_pluginFactory()
from_form_method = "from_gnomonLString"


def _gnomonLStringInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_lString")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_lString = form_dict
            self._in_lString_data = data_dict
        return self._in_lString

    setattr(cls, method, func)

    def setter_func(self, lString):
        self._in_lString = lString
        setattr(self, attr, {})

        if self._in_lString is not None:
            lString_dict = formDictFromSeries(form=self._in_lString,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
            setattr(self, attr, lString_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonLStringInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonLStringInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonLStringInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonLStringOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_lString")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_lString = form_dict
            self._out_lString_data = data_dict
        return self._out_lString

    setattr(cls, method, func)

    return cls


def gnomonLStringOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonLStringOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonLStringOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper
