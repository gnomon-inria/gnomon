from .image_decorator import buildImageSeries
from .mesh_decorator import buildMeshSeries
from .point_cloud_decorator import buildPointCloudSeries
from .tree_decorator import buildTreeSeries


def _gnomonFormDictOutput(cls, form_attr, method):
    def func(self):
        self._form_series = {}

        for form in form_attr.keys():
            if form == 'gnomonImage':
                self._form_series[form],_,_ = buildImageSeries(getattr(self ,form_attr[form]))
            elif form == 'gnomonMesh':
                self._form_series[form],_,_ = buildMeshSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonPointCloud':
                self._form_series[form],_,_ = buildPointCloudSeries(getattr(self ,form_attr[form]))
            if form == 'gnomonTree':
                self._form_series[form],_,_ = buildTreeSeries(getattr(self ,form_attr[form]))

        return self._form_series

    setattr(cls, method, func)

    return cls


def gnomonFormDictOutput(cls=None, form_attr={}, method='forms'):
    if cls is not None:
        return _gnomonFormDictOutput(cls, form_attr)
    else:
        def wrapper(cls):
            return _gnomonFormDictOutput(cls, form_attr, method)

        return wrapper