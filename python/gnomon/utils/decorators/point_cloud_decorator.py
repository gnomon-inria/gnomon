import gnomon.core

from gnomon.core import gnomonPointCloud
from gnomon.utils.gnomonPlugin import default_input_accessors, default_output_accessors
from .accessors_decorator_base import form_input, form_output

plugin_group = "pointCloudData"
form_class = gnomonPointCloud
form_data_factory = gnomon.core.pointCloudData_pluginFactory()


def pointCloudInput(attr, data_plugin, methods=(None, None)):
    """
    Implements the accessor methods to attr for a point cloud input.
    Those accessors will convert between a gnomonPointCloudSeries and the type
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
        e.g. gnomonPointCloudDataPandas --> DataFrame (from pandas)
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
        return form_input(cls, attr, local_getter_method, local_setter_method, data_plugin, form_data_factory, plugin_group, form_class)

    return decorator


def pointCloudOutput(attr, data_plugin, method=None):
    """
    Implements the getter method to attr for a point cloud output.
    This getter will convert between a gnomonPointCloudSeries and the type
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
        e.g. gnomonPointCloudDataPandas --> DataFrame (from pandas)
    method: str
        Name of the getter to be implemented
    """
    def decorator(cls):
        if method is None:
            bound_method = default_output_accessors(cls, form_class)
        else:
            bound_method = method
        return form_output(cls, attr, bound_method, data_plugin, form_data_factory, plugin_group, form_class)

    return decorator
