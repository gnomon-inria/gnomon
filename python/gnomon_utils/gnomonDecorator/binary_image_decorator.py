import gnomoncore

from gnomoncore import gnomonBinaryImage
from gnomon_utils.gnomonPlugin import load_plugin_group

from .form_series import buildFormSeries, formDictFromSeries

load_plugin_group("binaryImageData")

default_plugin = "binaryImageDataSpatialImage"
default_setter = "set_image"
default_attr = "_image"

form_class = gnomonBinaryImage
form_data_factory = gnomoncore.binaryImageData_pluginFactory()
from_form_method = "from_gnomonBinaryImage"


def _gnomonBinaryImageInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_binaryImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_binaryImage = form_dict
            self._in_binaryImage_data = data_dict
        return self._in_binaryImage

    setattr(cls, method, func)

    def setter_func(self, binaryImage):
        self._in_binaryImage = binaryImage
        setattr(self, attr, {})

        if self._in_binaryImage is not None:
            binaryImage_dict = formDictFromSeries(form=self._in_binaryImage,
                                                  form_data_factory=form_data_factory,
                                                  from_form_method=from_form_method,
                                                  data_plugin=data_plugin,
                                                  data_attr=data_attr)
            setattr(self, attr, binaryImage_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonBinaryImageInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonBinaryImageInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonBinaryImageInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonBinaryImageOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_binaryImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_binaryImage = form_dict
            self._out_binaryImage_data = data_dict
        return self._out_binaryImage

    setattr(cls, method, func)

    return cls


def gnomonBinaryImageOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonBinaryImageOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonBinaryImageOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper
