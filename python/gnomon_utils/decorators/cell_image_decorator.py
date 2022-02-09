import gnomoncore

from gnomoncore import gnomonCellImage
from gnomon_utils.gnomonPlugin import load_plugin_group, default_input_accessors, default_output_accessors

from .form_series import buildFormSeries, formDictFromSeries, is_form_series_modified

load_plugin_group("cellImageData")

form_class = gnomonCellImage
form_data_factory = gnomoncore.cellImageData_pluginFactory()


def _gnomonCellImageInput(cls, attr, method, setter_method, data_plugin):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_cellImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   form_data_factory=form_data_factory, data_plugin=data_plugin)
            self._in_cellImage = form_dict
            self._in_cellImage_data = data_dict
        return self._in_cellImage

    setattr(cls, method, func)

    def setter_func(self, cellImage):
        if is_form_series_modified(self, "_in_cellImage", cellImage):
            self._in_cellImage = cellImage
            setattr(self, attr, {})
    
            if self._in_cellImage is not None:
                cellImage_dict = formDictFromSeries(form=self._in_cellImage, form_data_factory=form_data_factory,
                                                    data_plugin=data_plugin)
                setattr(self, attr, cellImage_dict)
    
                if hasattr(self,"refresh_parameters"):
                    self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def cellImageInput(attr, data_plugin, methods=(None, None)):
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
    def func(self, update=True):
        update = update or not hasattr(self, "_out_cellImage")
        if update:
            form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                                   form_data_factory=form_data_factory, data_plugin=data_plugin)
            self._out_cellImage = form_dict
            self._out_cellImage_data = data_dict
        return self._out_cellImage

    setattr(cls, method, func)

    return cls


def cellImageOutput(attr, data_plugin, method=None):
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return _gnomonCellImageOutput(cls, attr, bound_method, data_plugin=data_plugin)

    return decorator
