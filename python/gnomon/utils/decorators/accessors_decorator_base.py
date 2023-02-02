from typing import Any
import numpy as np

from gnomon.utils.gnomonPlugin import register_input, register_output
from .form_series import buildFormSeries, formDictFromSeries, getFormDataClass


def _is_form_dict_modified(old: dict[str, Any], new: dict[str, Any]) -> bool:
    modified = len(old) != len(new)
    modified = modified or any(key not in new for key in old)
    modified = modified or any(key not in old for key in new)
    if not modified:  # same length and same keys
        modified = modified or np.any(old[key] != new[key] for key in old)
    return modified


def form_input(cls, attr, method, setter_method, data_plugin, form_data_factory, plugin_group, form_class):
    register_input(cls, attr)
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)

    def getter(self, update=True):
        form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                               data_plugin=data_plugin)
        return form_dict

    setattr(cls, method, getter)

    def setter(self, form):
        if form is not None:
            form_dict = formDictFromSeries(form=form, data_plugin=data_plugin)
            if hasattr(self, "refresh_parameters") and _is_form_dict_modified(getattr(self, attr), form_dict):
                setattr(self, attr, form_dict)
                self.refresh_parameters()
            else:
                setattr(self, attr, form_dict)

        elif hasattr(self, "refresh_parameters") and getattr(self, attr):
            setattr(self, attr, {})
            self.refresh_parameters()
        else:
            setattr(self, attr, {})

    setattr(cls, setter_method, setter)

    return cls


def form_output(cls, attr, method, data_plugin, form_data_factory, plugin_group, form_class):
    register_output(cls, attr)
    data_plugin = getFormDataClass(data_plugin, form_data_factory, plugin_group)

    def getter(self, update=True):
        form_dict, data_dict = buildFormSeries(form_dict=getattr(self, attr), form_class=form_class,
                                               data_plugin=data_plugin)
        return form_dict

    setattr(cls, method, getter)

    return cls
