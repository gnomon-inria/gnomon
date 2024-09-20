import jinja2
from importlib_metadata import entry_points, version, metadata
import inspect
from collections import OrderedDict

import gnomon.core as gc
import gnomon.visualization as gv

form_names = [
    name for name, c in gc.__dict__.items() if isinstance(c, type) and issubclass(c, gc.gnomonAbstractForm) and c is not gc.gnomonAbstractForm
]

forms_interfaces = [
    x for x in gc.__dict__.values() if isinstance(x, type) and issubclass(x, gc.gnomonAbstractFormData) and x is not gc.gnomonAbstractFormData
]
algo_interfaces = [
    x for x in gc.__dict__.values() if isinstance(x, type) and issubclass(x, gc.gnomonAbstractAlgorithm) and x is not gc.gnomonAbstractAlgorithm
]
visu_interfaces = [
    x for x in gv.__dict__.values() if isinstance(x, type) and issubclass(x, gv.gnomonAbstractVisualization) and x is not gv.gnomonAbstractVisualization
]

def find_plugin_inputs_outputs(plugin_class):
    inputs, outputs = [], []
    for form_name in form_names:
        if hasattr(plugin_class, "defaultSetter"):
            input = plugin_class.defaultSetter(form_name)
            if input and getattr(plugin_class, input).__module__ not in plugin_class.__base__.__module__:
                inputs.append(form_name)
        if hasattr(plugin_class, "defaultOutput"):
            output = plugin_class.defaultOutput(form_name)
            if output and getattr(plugin_class, output).__module__ not in plugin_class.__base__.__module__:
                outputs.append(form_name)
    return inputs, outputs


def group_name(interface_name: str) -> str:
    n = interface_name.removeprefix("gnomonAbstract")
    return n[0].lower() + n[1:]

def form_name_from_data(data_interface_name: str) -> str:
    return "gnomon" + data_interface_name.removeprefix("gnomonAbstract").removesuffix("Data")

def parse_plugin_package(package_name: str) -> dict:
    """Parses a plugin package and returns the name, description and plugins of the package"""
    eps = entry_points()
    out = OrderedDict(
        {
            "name": package_name,
            "description": metadata(package_name).get("description", ""),
            "version": version(package_name),
            "forms": {},
            "visu": {},
            "algorithms": {},
        }
    )
    for abstract_form_data in forms_interfaces:
        gn = group_name(abstract_form_data.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name:
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                out["forms"][ep.name] = {
                    "name": ep.name,
                    "form": form_name_from_data(abstract_form_data.__name__),
                    "description": inspect.cleandoc(plugin.__doc__) if plugin.__doc__ else "",
                }

    for abstract_visu in visu_interfaces:
        gn = group_name(abstract_visu.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name:
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                inputs, _ = find_plugin_inputs_outputs(plugin)
                out["visu"][ep.name] = {
                    "name": ep.name,
                    "form": inputs,
                    "image": "image",
                }

    for abstract_algo in algo_interfaces:
        gn = group_name(abstract_algo.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name:
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                inputs, outputs = find_plugin_inputs_outputs(plugin)
                out["algorithms"][ep.name] = {
                    "name": ep.name,
                    "inputs": inputs,
                    "outputs": outputs,
                    "description": inspect.cleandoc(plugin.__doc__) if plugin.__doc__ else "",
                }

    return out

env = jinja2.Environment(
    loader=jinja2.FileSystemLoader("."),
    autoescape=jinja2.select_autoescape()
)