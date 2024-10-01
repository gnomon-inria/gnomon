import argparse
import inspect
import json
import os
import pathlib
import re
import secrets
import shutil
import subprocess
from collections import OrderedDict
from functools import reduce

import gnomon.core as gc
import gnomon.visualization as gv
import jinja2
import requests
from importlib_metadata import entry_points, metadata, version
from packaging.version import parse as parse_version

BASE_GNOMON_ENV = "gnomon-doc"
DOC_ARCHIVE = pathlib.Path.home().joinpath("gnomon_doc_archive")
HISTORY_FILE = DOC_ARCHIVE.joinpath("gnomon_package_doc_history.txt")

CONDA_EXE = pathlib.Path(os.getenv("CONDA_EXE")).with_name("mamba") if pathlib.Path(os.getenv("CONDA_EXE")).with_name("mamba").exists() else pathlib.Path(os.getenv("CONDA_EXE"))


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


FORM_BADGE_TABLE = {
    "gnomonImage": ("Image", "../../user_guide/forms/image.html"),
    "gnomonCellImage": ("CellImage", "../../user_guide/forms/cell_image.html"),
    "gnomonDataDict": ("DataDict", "../../user_guide/forms/data_dict.html"),
    "gnomonDataFrame": ("DataFrame", "../../user_guide/forms/dataframe.html"),
    "gnomonLString": ("LString", "../../user_guide/forms/lstring.html"),
    "gnomonMesh": ("Mesh", "../../user_guide/forms/mesh.html"),
    "gnomonPointCloud": ("PointCloud", "../../user_guide/forms/point_cloud.html"),
}

def format_form_to_badge(form_name, outline=False):
    if form_name in FORM_BADGE_TABLE:
        name, link = FORM_BADGE_TABLE[form_name]
    else:
        stripped_name: str = form_name.removeprefix("gnomon")
        indices = [i for i, c in enumerate(stripped_name) if c.isupper()] + [len(stripped_name)]
        parts = [stripped_name[start:finish] for start, finish in zip(indices[:-1], indices[1:])]
        name = stripped_name
        link = "../../user_guide/forms/{}.html".format("_".join(map(lambda s:s.lower(), parts)))
        print(f"Unknown form: {form_name}, guessing name: {name} and link: {link}")
    if outline:
        return "{bdg-link-success-line}`" + f"{name} <{link}>`"
    else:
        return "{bdg-link-success}`" + f"{name} <{link}>`"


def parse_plugin_package(package_name: str) -> dict:
    """Parses a plugin package and returns the name, description and plugins of the package"""
    eps = entry_points()
    _description: str = metadata(package_name).get("description", "").strip()
    match = re.match("(# [\S ]*)[\s]*([\S\s]*)", _description)
    out = OrderedDict(
        {
            "name": package_name,
            "description": match.group(2),
            "version": version(package_name),
            "forms": OrderedDict(),
            "visu": OrderedDict(),
            "algorithms": OrderedDict(),
        }
    )
    for abstract_form_data in forms_interfaces:
        gn = group_name(abstract_form_data.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name.replace("_", "-"):
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                out["forms"][ep.name] = {
                    "name": ep.name,
                    "form": format_form_to_badge(form_name_from_data(abstract_form_data.__name__)),
                    "description": inspect.cleandoc(plugin.__doc__) if plugin.__doc__ else "",
                }

    for abstract_visu in visu_interfaces:
        gn = group_name(abstract_visu.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name.replace("_", "-"):
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                inputs, _ = find_plugin_inputs_outputs(plugin)
                out["visu"][ep.name] = {
                    "name": ep.name,
                    "form": "<br/>".join(map(format_form_to_badge, inputs)),
                    "image": "",
                }

    for abstract_algo in algo_interfaces:
        gn = group_name(abstract_algo.__name__)
        for ep in eps.select(group=gn):
            if ep.dist.name.replace("_", "-") == package_name.replace("_", "-"):
                try:
                    module = ep.load()
                except:
                    continue
                plugin = getattr(module, ep.name)
                inputs, outputs = find_plugin_inputs_outputs(plugin)
                out["algorithms"][ep.name] = {
                    "name": ep.name,
                    "inputs": "<br/>".join(map(format_form_to_badge, inputs)),
                    "outputs": "<br/>".join(map(format_form_to_badge, outputs)),
                    "description": inspect.cleandoc(plugin.__doc__) if plugin.__doc__ else "",
                }

    return out

def generate_template(args: argparse.Namespace):
    package_name = args.package_name
    template_var = parse_plugin_package(package_name)

    # 5 - render template
    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader("./templates"),
        autoescape=jinja2.select_autoescape()
    )
    template = env.get_template("package_doc_template.md.jinja")
    pathlib.PosixPath("plugins/packages").mkdir(exist_ok=True)
    with open(f"plugins/packages/{package_name}.md", "w") as f:
        f.write(template.render(template_var))
    with open(DOC_ARCHIVE.joinpath(f"{package_name}.md"), "w") as f:
        f.write(template.render(template_var))

def get_package_list() ->dict[str, dict]:
    package_list_address = "https://gitlab.com/gnomon-inria/gnomon-package-list/-/raw/main/package_list.json?ref_type=heads"
    r = requests.get(package_list_address)
    return r.json()


def get_package_generation_history(path: str):
    out = {}
    if pathlib.Path(path).exists():
        with open(path, "r") as f:
            for line in f:
                package, install_name, _version, build = line.strip().split(" ")
                out[package] = (install_name, _version, build)
    return out


def has_conda_package_been_updated(install_infos: dict, old_specs: tuple[str, str, str]):
    install_name = install_infos["package_name"]
    print(f"Looking for {install_name:50} :: ", end="", flush=True)
    channels = reduce(lambda x, y: x + y, [["-c", channel] for channel in install_infos["channels"]])
    completed_process = subprocess.run(
        [CONDA_EXE, "search"] + channels + ["--json", install_name],
        capture_output=True,
        encoding="utf-8", executable=CONDA_EXE
    )
    out = json.loads(completed_process.stdout)
    package_list = sorted(out[install_name], key=lambda x: x["timestamp"], reverse=True)
    _name, _version, _build = old_specs
    old_timestamp = -1
    for package in package_list:
        if package["name"] == _name and package["version"] == _version and package["build"] == _build:
            old_timestamp = package["timestamp"]
    for package in package_list:
        if package["name"] == _name and parse_version(package["version"]) >= parse_version(_version) and package["timestamp"] > old_timestamp:
            print("new version or build available")
            return True
    print("up to date")
    return False


def get_packages_to_process():
    to_process = {}
    others = {}
    package_list = get_package_list()
    history = get_package_generation_history(HISTORY_FILE)
    for package, infos in package_list.items():
        if package not in history:
            to_process[package] = infos
            continue
        if infos["install"]["method"] == "conda" and has_conda_package_been_updated(infos["install"], history[package]):
            to_process[package] = infos
            continue
        others[package] = (history[package])
    return to_process, others


def process_package(package_name, infos):
    source = infos["source"]
    install_info = infos["install"]
    # 1 - clone base env
    env_name = f"{package_name}-doc-{secrets.token_hex(5)}"
    command = [CONDA_EXE.stem, "create", "--name", env_name, "--clone", BASE_GNOMON_ENV]
    print("\n1 - ", " ".join(command))
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )

    # 2 - install package
    channels = reduce(lambda x, y: x + y, [["-c", channel] for channel in install_info["channels"]])
    command = [CONDA_EXE.stem, "install", "-C", "-y", "-n", env_name] + channels + [install_info["package_name"]]
    print("\n2 - ", " ".join(command))
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )
    # TODO: remove when install fix for numpy is done
    command = [CONDA_EXE.stem, "run", "-n", env_name, "pip", "uninstall", "numpy", "-y"]
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )
    command = [CONDA_EXE.stem, "install", "-C", "-y", "--force-reinstall", "-n", env_name] + channels + ['"numpy<2"']
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )
    # 3 - parse package
    command = [CONDA_EXE.stem, "run", "-n", env_name, "python", "package_doc_generator.py", "build", install_info["package_name"]]
    print("\n3 - ", " ".join(command))
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )
    template_var = parse_plugin_package(install_info["package_name"])

    # 4 - get build marker (dist_name)
    command = [CONDA_EXE.stem, "list", "-n", env_name, install_info["package_name"], "--json"]
    print("\n4 - ", " ".join(command))
    completed_process = subprocess.run(
        command,
        capture_output=True,
        encoding="utf-8", executable=CONDA_EXE
    )
    output = json.loads(completed_process.stdout)
    install_name = output[0]["name"]
    _version = output[0]["version"]
    _build = output[0]["build_string"]

    # 5 - remove env
    command = [CONDA_EXE.stem, "env", "remove", "-n", env_name]
    print("\n5 - ", " ".join(command))
    completed_process = subprocess.run(
        command,
        capture_output=False,
        encoding="utf-8", executable=CONDA_EXE
    )
    print("done - ", install_name, _version, _build)
    print("======================================")
    return install_name, _version, _build

def main(args: argparse.Namespace):
    DOC_ARCHIVE.mkdir(exist_ok=True)

    to_process, others = get_packages_to_process()
    history = [f"{package_name} {' '.join(spec)}\n" for package_name, spec in others.items()]
    for package_name, infos in others.items():
        file_name, _version, _build = infos
        shutil.copy(DOC_ARCHIVE.joinpath(f"{file_name}.md"), f"plugins/packages/{file_name}.md")
    for package_name, infos in to_process.items():
        print(f"\n\n === Processing {package_name} ===\n\n")
        install_name, _version, _build = process_package(package_name, infos)
        history.append(f"{package_name} {install_name} {_version} {_build}\n")
    with open(HISTORY_FILE, "w") as f:
        f.writelines(history)


if __name__ == "__main__":
    arg_parser = argparse.ArgumentParser(prog="package_doc_generator", description="Tool to generate docs for gnomon plugin packages")
    subparsers = arg_parser.add_subparsers(required=True)
    all = subparsers.add_parser("all", description="Create docs for all packages listed at https://gitlab.com/gnomon-inria/gnomon-package-list/-/raw/main/package_list.json?ref_type=heads")
    all.set_defaults(func=main)
    build = subparsers.add_parser("build", description="Build docs for one gnomon plugin package")
    build.set_defaults(func=generate_template)
    build.add_argument("package_name")
    try:
        args = arg_parser.parse_args()
    except TypeError:
        arg_parser.print_help()
    else:
        args.func(args)

