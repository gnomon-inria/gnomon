import gnomoncore
from gnomon_utils.gnomonPlugin import load_plugin_group
from typing import Union


def is_form_series_modified(algo, form_name, form_series):
    modified = not hasattr(algo, form_name)
    modified = modified or any([i is None for i in (getattr(algo, form_name), form_series)])
    modified = modified or (len(getattr(algo, form_name)) != len(form_series))
    modified = modified or any([time not in getattr(algo, form_name) for time in form_series.keys()])
    # TODO: compare C++ object addresses instead of data objects!
    modified = modified or any([getattr(algo, form_name)[time].data() != form_series[time].data() for time in form_series.keys()])
    return modified


def buildFormSeries(form_dict: dict, form_class: type, data_plugin: type):
    form = {}
    form_data = {}

    if isinstance(form_dict, dict):
        for time in form_dict.keys():
            form[time] = form_class()
            form_data[time] = data_plugin()
            form[time].setData(form_data[time])
            form_data[time].__data_setter(form_dict[time])

    return form, form_data


def formDictFromSeries(form, data_plugin: type):
    form_dict = {}
    for time in form.keys():
        try:
            form_dict[time] = form[time].data().__data_getter()
        except AttributeError:
            form_data = data_plugin()
            form_data.fromGnomonForm(form[time])
            form_dict[time] = form_data.__data_getter()

    return form_dict


def getFormDataClass(data_plugin: Union[type, str], form_data_factory, plugin_group: str) -> type:
    if isinstance(data_plugin, type):
        return data_plugin
    elif isinstance(data_plugin, str):
        load_plugin_group(plugin_group)
        data_plugin_instance: object = form_data_factory.create(data_plugin)
        if data_plugin_instance:
            return data_plugin_instance.__class__
        else:
            raise KeyError(f"Could not find and load data plugin {data_plugin} from plugin group {plugin_group}. "
                           f"The plugin might not be installed")
    raise TypeError("Expected type or str for data_plugin argument")

