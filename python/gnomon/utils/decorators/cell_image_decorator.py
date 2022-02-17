import gnomon.core

from gnomon.core import gnomonCellImage
from gnomon.utils.gnomonPlugin import default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified, getFormDataClass

plugin_group = "cellImageData"
form_class = gnomonCellImage
form_data_factory = gnomon.core.cellImageData_pluginFactory()


def _gnomonCellImageInput(cls, attr, method, setter_method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_in_cellImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._in_cellImage = form_dict
            self._in_cellImage_data = data_dict
        return self._in_cellImage

    setattr(cls, method, func)

    def setter_func(self, cellImage):
        if is_form_series_modified(self, "_in_cellImage", cellImage):
            self._in_cellImage = cellImage
            setattr(self, attr, {})
    
            if self._in_cellImage is not None:
                cellImage_dict = formDictFromSeries(form=self._in_cellImage, data_plugin=data_plugin)
                setattr(self, attr, cellImage_dict)
    
                if hasattr(self,"refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def cellImageInput(attr, data_plugin, methods=(None, None)):
    """
    Implements the accessor methods to attr for a cell image input.
    Those accessors will convert between a gnomonCellImageSeries and the type
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
        e.g. gnomonCellImageDataTissueImage --> TissueImage (from timagetk)
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
        return _gnomonCellImageInput(cls, attr, local_getter_method, local_setter_method, data_plugin=data_plugin)

    return decorator


def _gnomonCellImageOutput(cls, attr, method, data_plugin):
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)
    def func(self, update=True):
        update = update or not hasattr(self, "_out_cellImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   data_plugin=data_plugin)
            self._out_cellImage = form_dict
            self._out_cellImage_data = data_dict
        return self._out_cellImage

    setattr(cls, method, func)

    return cls


def cellImageOutput(attr, data_plugin, method=None):
    """
    Implements the getter method to attr for a cell image output.
    This getter will convert between a gnomonCellImageSeries and the type
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
        e.g. gnomonCellImageDataTissueImage --> TissueImage (from timagetk)
    method: str
        Name of the getter to be implemented
    """
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonCellImageOutput(cls, attr, bound_method, data_plugin=data_plugin)

    return decorator
