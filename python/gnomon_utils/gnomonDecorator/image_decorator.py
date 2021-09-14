import gnomoncore

import logging

from gnomoncore import gnomonImage
from gnomon_utils.gnomonPlugin import load_plugin_group

load_plugin_group("imageData")

default_plugin = "gnomonImageDataSpatialImageDict"
default_setter = "set_image_dict"
default_attr = "_img_dict"


def buildImageSeries(image_dict, data_plugin=default_plugin, data_setter=default_setter, form_dict=None):
    image = {}
    image_data = {}

    for time in image_dict.keys():
        if form_dict is None:
            image[time] = gnomonImage()
            image_data[time] = gnomoncore.imageData_pluginFactory().create(data_plugin)
            image[time].setData(image_data[time])
        else:
            image[time] = form_dict[time]
            image_data[time] = image[time].data()
        getattr(image_data[time], data_setter)(image_dict[time])

    return image, image_data


def imageDictFromSeries(image, data_plugin=default_plugin, data_attr=default_attr):
    image_dict = {}
    for time in image.keys():
        if hasattr(image[time].data(), data_attr):
            image_dict[time] = getattr(image[time].data(), data_attr)
        else:
            image_data = gnomoncore.imageData_pluginFactory().create(data_plugin).from_gnomonImage(image[time])
            image_dict[time] = getattr(image_data, data_attr)

    return image_dict


def _gnomonImageInput(cls, attr, method, setter_method, data_plugin, data_setter, data_attr):
    def func(self, update=True):
        update = update or not hasattr(self, "_in_image")
        if update:
            form_dict, data_dict = buildImageSeries(getattr(self, attr), data_plugin, data_setter)
            self._in_image = form_dict
            self._in_image_data = data_dict
        return self._in_image

    setattr(cls, method, func)

    def setter_func(self, image):
        self._in_image = image
        setattr(self, attr, {})

        if self._in_image is not None:
            image_dict = imageDictFromSeries(self._in_image, data_plugin, data_attr)
            setattr(self, attr, image_dict)

            if hasattr(self,"refresh_parameters"):
                self.refresh_parameters()

    setattr(cls, setter_method, setter_func)

    return cls


def gnomonImageInput(cls=None, attr=None, method='input', setter_method='setInput', data_plugin=default_plugin, data_setter=default_setter, data_attr=default_attr):
    if cls is not None:
        return _gnomonImageInput(cls, attr, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)
    else:
        def wrapper(cls):
            return _gnomonImageInput(cls, attr, method, setter_method, data_plugin=data_plugin, data_setter=data_setter, data_attr=data_attr)

        return wrapper


def _gnomonImageOutput(cls, attr, method, data_plugin, data_setter):
    def func(self, update=True):
        update = update or not hasattr(self, "_out_image")
        if update:
            form_dict, data_dict = buildImageSeries(getattr(self, attr), data_plugin, data_setter)
            self._out_image = form_dict
            self._out_image_data = data_dict
        return self._out_image

    setattr(cls, method, func)

    return cls


def gnomonImageOutput(cls=None, attr=None, method='output', data_plugin=default_plugin, data_setter=default_setter):
    if cls is not None:
        return _gnomonImageOutput(cls, attr, data_plugin=data_plugin, data_setter=data_setter)
    else:
        def wrapper(cls):
            return _gnomonImageOutput(cls, attr, method, data_plugin=data_plugin, data_setter=data_setter)

        return wrapper



