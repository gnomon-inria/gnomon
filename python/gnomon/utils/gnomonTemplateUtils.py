import os
import json
from glob import glob
from collections import OrderedDict, UserDict
import importlib.resources as ir
import pathlib

from jinja2 import Environment, StrictUndefined, FileSystemLoader, BaseLoader


if not __package__:
    __package__ = "gnomon.utils"

# awfull line where the context manager is never closed
templates_path = ir.as_file(ir.files(__package__).joinpath('templates')).__enter__()
template_env = Environment(
    loader=FileSystemLoader(templates_path),
    autoescape=False,
    undefined=StrictUndefined,
)


class Schematic(UserDict):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.jinja_env = Environment(
            loader=BaseLoader,
            autoescape=False,
            undefined=StrictUndefined,
        )

    def get_templated(self, key, env, default=None):
        if default:
            output = self.get(key, default)
        else:
            output = self.get(key)
        if isinstance(output, str):
            template = self.jinja_env.from_string(output, env)
            return template.render(env)
        else:
            raise TypeError(f"Expected string not {type(output)} for item ({key}, {output})")


def dispatch(node: Schematic, env: dir, cur_path):
    # updating env: env resolution priority - ask > env > node["vars"]
    new_env = node.get("vars", {})
    new_env.update(env)  # base env gets priority
    # getting asks
    if "ask" in node:
        new_env = get_asks(node["ask"], new_env)
    # dispatching
    if "tree" in node:  # directory
        return directory_walker(node, new_env, cur_path)
    elif node.get("python_package", False) is True:  # empty python package
        return empty_package(node, new_env, cur_path)
    elif "template" in node:  # file from template
        return template_handler(node, new_env, cur_path)
    elif "source" in node:  # file from template string
        return source_handler(node, new_env, cur_path)
    elif "schematic" in node:  # sub schematic
        return schematic_handler(node, new_env, cur_path)
    else:
        raise TypeError(f"node {node.get('name', '[name is not defined either]')} does not define either a filetree"
                        f" [tree] or a source template [template]")


def directory_walker(node: Schematic, env: dir, cur_path):
    if "schematic_name" not in node:  # True if node is not the root of the schematic
        new_path = os.path.join(cur_path, node.get_templated("name", env))
        try:
            os.mkdir(new_path)
        except FileExistsError:
            pass
    else:
        new_path = cur_path
    if node.get("python_package", False) is True:
        with open(os.path.join(new_path, "../__init__.py"), "w"):
            pass
    for child in node["tree"]:
        dispatch(Schematic(child), env, new_path)


def empty_package(node: Schematic, env: dir, cur_path):
    new_path = os.path.join(cur_path, node.get_templated("name", env))
    try:
        os.mkdir(new_path)
    except FileExistsError:
        pass
    with open(os.path.join(new_path, "../__init__.py"), "w"):
        pass


def template_handler(leaf: Schematic, env, cur_path):
    file_path = os.path.join(cur_path, leaf.get_templated("name", env))
    template = template_env.get_template(leaf["template"])
    with open(file_path, "w") as file:
        file.write(template.render(env))


def source_handler(leaf: Schematic, env, cur_path):
    file_path = os.path.join(cur_path, leaf.get_templated("name", env))
    template = template_env.from_string(leaf["source"])
    with open(file_path, "w") as file:
        file.write(template.render(env))


def schematic_handler(node, new_env, cur_path):
    # resolve path
    if os.path.isabs(node["path"]):
        path = node["path"]
    else:
        path = os.path.join(cur_path, node["path"])
    # calling the schematic
    schematic_reader_from_path(node["schematic"], path, new_env)


def get_asks(questions, base_env):
    print("Complete the following prompts : ")
    new_env = base_env.copy()
    for key, prompt in questions.items():
        if key in base_env:
            if isinstance(prompt, dict):
                new_env[key] = get_asks(prompt, base_env[key])
            else:
                new_env[key] = input(f"[default: {base_env[key]}] " + prompt)
        else:
            if isinstance(prompt, dict):
                new_env[key] = get_asks(prompt, {})
            else:
                new_env[key] = input("[mandatory] " + prompt)
    return new_env


def schematic_reader_from_path(schematic_path, output_path, base_env):
    with ir.as_file(pathlib.Path(schematic_path).absolute())  as file_path:
        with open(file_path, "r") as file:
            root = Schematic(json.loads(file.read()))
    os.makedirs(output_path, exist_ok=True)
    dispatch(Schematic(root), base_env, output_path)


def register_known_schematics(path="") -> OrderedDict:
    schematic_list = OrderedDict()
    if not path:
        path = ir.files(__package__).joinpath("schematic")
    else:
        path = pathlib.PosixPath(path)

    for entry in sorted(filter(lambda _path: _path.match("*.json"), path.iterdir())):
        with ir.as_file(entry) as file_path:
            with open(file_path, "r") as file:
                name = json.load(file)["schematic_name"]
            schematic_list[name] = entry
    return schematic_list


if __name__ == '__main__':
    schematic_reader_from_path("schematic/plugin.json", "tmp", {})