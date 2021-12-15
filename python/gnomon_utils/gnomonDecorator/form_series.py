import gnomoncore


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