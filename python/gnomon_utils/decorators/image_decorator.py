import gnomoncore

from gnomoncore import gnomonImage
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified

load_plugin_group("imageData")

default_plugin = "gnomonImageDataSpatialImageDict"
default_setter = "set_image_dict"
default_attr = "_img_dict"

form_class = gnomonImage
form_data_factory = gnomoncore.imageData_pluginFactory()
from_form_method = "from_gnomonImage"


def _gnomonImageInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_image")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._in_image = form_dict
            self._in_image_data = data_dict
        return self._in_image

    setattr(cls, method, func)

    def setter_func(self, image):
        if is_form_series_modified(self, "_in_image", image):
            self._in_image = image
            setattr(self, attr, {})

            if self._in_image is not None:
                image_dict = formDictFromSeries(form=self._in_image,
                                                form_data_factory=form_data_factory,
                                                from_form_method=from_form_method,
                                                data_plugin=data_plugin,
                                                data_attr=data_attr)
                setattr(self, attr, image_dict)

                if hasattr(self, "refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def imageInput(attr, methods=(None, None), data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonImageInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

    return decorator



def _gnomonImageOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_image")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr),
                                                   form_class=form_class,
                                                   form_data_factory=form_data_factory,
                                                   data_plugin=data_plugin,
                                                   data_setter=data_setter)
            self._out_image = form_dict
            self._out_image_data = data_dict
        return self._out_image

    setattr(cls, method, func)

    return cls


def imageOutput(attr, method=None, data_plugin=default_plugin, data_setter=default_setter):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonImageOutput(cls, attr, bound_method, data_plugin=data_plugin, data_setter=data_setter)

    return decorator
