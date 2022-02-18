import gnomon.core

from gnomon.core import gnomonBinaryImage
from gnomon.utils.gnomonPlugin import default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified, getFormDataClass

plugin_group = "binaryImageData"
form_class = gnomonBinaryImage
form_data_factory = gnomon.core.binaryImageData_pluginFactory()


def _gnomonBinaryImageInput(cls, attr, method, setter_method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_in_binaryImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._in_binaryImage = form_dict
            self._in_binaryImage_data = data_dict
        return self._in_binaryImage

    setattr(cls, method, func)

    def setter_func(self, binaryImage):
        if is_form_series_modified(self, "_in_binaryImage", binaryImage):
            self._in_binaryImage = binaryImage
            setattr(self, attr, {})

            if self._in_binaryImage is not None:
                binaryImage_dict = formDictFromSeries(form=self._in_binaryImage, data_plugin=data_plugin)
                setattr(self, attr, binaryImage_dict)

                if hasattr(self, "refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def binaryImageInput(attr, data_plugin, methods=(None, None)):
    """
    Implements the accessor methods to attr for a binary image input.
    Those accessors will convert between a gnomonBinaryImageSeries and the type
    of attr as defined by the data_plugin.

    The data_plugin defines the type of attr as it defines how the form
    is represented on python's side.
    The methods to be implemented are usually inferred from the base class
    of the decorated class. Some classes may require the methods names
    implemented to be specified.

    Parameters
    ----------
    attr: str
        Attribute of the class this decorator will define accessors for.
    data_plugin: Union[type, str]
        Data plugin class or plugin factory key of said class.
        Defines the type of attr (defines how the form is represented).
        e.g. binaryImageDataSpatialImage --> SpatialImage (from timagetk)
    methods: (str, str)
        Methods which are going to be implemented (getter, setter)
    """
    def decorator(cls):
        if None in methods:
            local_getter_method, local_setter_method = default_input_accessors(cls, form_class)
        elif len(methods) == 2:
            local_getter_method, local_setter_method = methods
        else:
            raise TypeError("Expected 2-tuple (getter, setter) of type (str, str)")
        return _gnomonBinaryImageInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin)

    return decorator


def _gnomonBinaryImageOutput(cls, attr, method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_out_binaryImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._out_binaryImage = form_dict
            self._out_binaryImage_data = data_dict
        return self._out_binaryImage

    setattr(cls, method, func)

    return cls


def binaryImageOutput(attr, data_plugin, method=None):
    """
    Implements the getter method to attr for a binary image output.
    This getter will convert between a gnomonBinaryImageSeries and the type
    of attr as defined by the data_plugin.

    The data_plugin defines the type of attr as it defines how the form
    is represented on python's side.
    The method to be implemented is usually inferred from the base class
    of the decorated class. Some classes may require the method name
    to be specified.

    Parameters
    ----------
    attr: str
        Attribute of the class this decorator will define accessors for.
    data_plugin: Union[type, str]
        Data plugin class or plugin factory key of said class.
        Defines the type of attr.
        e.g. binaryImageDataSpatialImage --> SpatialImage (from timagetk)
    method: str
        Name of the getter to be implemented
    """
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonBinaryImageOutput(cls, attr, bound_method, data_plugin=data_plugin)

    return decorator
