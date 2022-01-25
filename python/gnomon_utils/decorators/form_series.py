import gnomoncore

def is_form_series_modified(algo, form_name, form_series):
    modified = not hasattr(algo, form_name)
    modified = modified or any([i is None for i in (getattr(algo, form_name), form_series)])
    modified = modified or (len(getattr(algo, form_name)) != len(form_series))
    modified = modified or any([time not in getattr(algo, form_name) for time in form_series.keys()])
    # TODO: compare C++ object addresses instead of data objects!
    modified = modified or any([getattr(algo, form_name)[time].data() != form_series[time].data() for time in form_series.keys()])
    return modified

def buildFormSeries(form_dict, form_class, form_data_factory, data_plugin, data_setter):
    form = {}
    form_data = {}

    if isinstance(form_dict, dict):
        for time in form_dict.keys():
            form[time] = form_class()
            form_data[time] = form_data_factory.create(data_plugin)
            form[time].setData(form_data[time])
            getattr(form_data[time], data_setter)(form_dict[time])

    return form, form_data


def formDictFromSeries(form, form_data_factory, from_form_method, data_plugin, data_attr):
    form_dict = {}
    for time in form.keys():
        if hasattr(form[time].data(), data_attr):
            form_dict[time] = getattr(form[time].data(), data_attr)
        else:
            form_data = getattr(form_data_factory.create(data_plugin), from_form_method)(form[time])
            form_dict[time] = getattr(form_data, data_attr)

    return form_dict