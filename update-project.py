import os
import argparse
import glob
import subprocess
import concurrent.futures
from functools import wraps
from typing import Callable, Iterable

from git import Repo, Head

parser = argparse.ArgumentParser()
parser.add_argument("path", default=".", help="Path to the directory containing "
                                              "gnomon, gnomon-x, the x-lab projects, etc. "
                                              "Plugin packages may be in a subfolder")
parser.add_argument("--branch", "-b", required=False, default="develop",
                    help="The branch name that should be used to checkout "
                         "and update gnomon, gnomon-x and the plugin"
                         " packages. Defaults to develop")
parser.add_argument("--compile", "-c", action="store_true", required=False, default=False, help="Compile libgnomon "
                                                                                                "and gnomon-x")
parser.add_argument("--update-env", "-u", action="store_true", required=False, default=False, help="Updates the conda"
                                                                                                   " environment")
parser.add_argument("--force-reinstall", action="store_true", required=False, default=False, help="Forces the "
                                                                                                  "reinstallation of "
                                                                                                  "the plugin packages")
args = parser.parse_args()


def resolve_branch(r: Repo, branch_name: str) -> str:
    """
    Checks if branch name present locally
    if not, if present in remote create local branch and set tracking
        if not in remote, check for develop then main then master
    :param r: Repository
    :param branch_name:
    :return: valid branch
    """
    if branch_name in r.branches:
        return branch_name
    print(f" -- no local branch {branch_name}")
    for remote in r.remotes:
        remote.fetch()
        if branch_name in remote.refs:
            print(f" -- found remote branch named {branch_name}, creating local branch")
            ref = remote.refs[branch_name]
            head = r.create_head(branch_name, ref)
            head.set_tracking_branch(ref)
            return branch_name

    print(f" -- no remote branch called {branch_name}, trying defaults")
    if branch_name == "main":
        default_branches = ["master", "develop"]
    elif branch_name == "master":
        default_branches = ["main", "develop"]
    else:
        default_branches = ["develop", "main", "master"]
    for branch_name in default_branches:
        print(f" -- checking {branch_name}", end=" ... ")
        if branch_name in r.branches:
            print("OK")
            return branch_name
        for remote in r.remotes:
            remote.fetch()
        if branch_name in remote.refs:
            ref = remote.refs[branch_name]
            head = r.create_head(branch_name, ref)
            head.set_tracking_branch(ref)
            print("OK")
            return branch_name
        print("NOT FOUND")


def checkout_pull(r: Repo, branch_name: str):
    """
    Checks out branch named branch_name from repository and pulls.
    :param r: Repository
    :param branch_name:
    """
    branch: Head = r.branches[branch_name]
    branch.checkout()
    origin_name = branch.tracking_branch().remote_name
    # r.remote(origin_name).fetch()
    r.remote(origin_name).pull()


def _map_async(func: Callable) -> Callable:
    @wraps(func)
    def f(iterable: Iterable, executor: concurrent.futures.Executor) -> list[concurrent.futures.Future]:
        futures = []
        for arg in iterable:
            futures.append(executor.submit(func, arg))
        return futures
    return f


@_map_async
def _plugin_package_update(path):
    print("Updating ", path)
    r = Repo(path)
    files = glob.glob(path + "/**", recursive=True)
    branch_name = resolve_branch(r, args.branch)
    checkout_pull(r, branch_name)
    diff = len(glob.glob(path + "/**", recursive=True)) - len(files)

    if os.path.exists(os.path.join(path, "CMakeLists.txt")):
        build_folder_path = os.path.join(path, "build")
        subprocess.run(["cmake", "-B",  build_folder_path, path])
        subprocess.run(["make", "-C", build_folder_path, f"-j{os.cpu_count()}", "install"])
    if (diff or args.force_reinstall) and\
            (os.path.exists(os.path.join(path, "pyproject.toml")) or os.path.exists(os.path.join(path, "setup.py"))):
        print(f" -- number of files changed: {diff:+}")
        subprocess.run(["pip", "install", "-e", path])


@_map_async
def _resolve_update(path):
    print("Updating ", path)
    r = Repo(path)
    branch_name = resolve_branch(r, args.branch)
    checkout_pull(r, branch_name)


@_map_async
def _simple_update(path):
    print("Updating ", path)
    r = Repo(path)
    checkout_pull(r, "develop")


def main():
    futures: list[concurrent.futures.Future] = []
    os.chdir(args.path)

    if args.update_env:
        if os.name == "posix":
            subprocess.run(["mamba", "env", "update", "-f", "gnomon/conda/env_macos.yaml"])
        elif os.name == "linux":
            subprocess.run(["mamba", "env", "update", "-f", "gnomon/conda/env_linux.yaml"])

    with concurrent.futures.ThreadPoolExecutor(max_workers=6) as executor:
        # Updating x-lab projects
        futures += _simple_update(glob.glob("x-*/"), executor)

        # Updating cross-parameters
        # futures += _simple_update(glob.glob("cross-parameters/"), executor)

        # Updating dtk-*
        futures += _simple_update(glob.glob("dtk-*/"), executor)

        # Updating gnomon
        futures += _resolve_update(glob.glob("gnomon"), executor)

        # Updating gnomon-x (obsolete)
        # futures += _resolve_update(glob.glob("gnomon-x"), executor)

        # Waiting for completion
        concurrent.futures.wait(futures)
        futures = []

        # Compiling
        if args.compile:
            print("Compiling")
            os.chdir("gnomon/build")
            subprocess.run(["rm", "-r", "wrp"])
            subprocess.run(["make", f"-j{os.cpu_count()}", "install"])
            os.chdir("../../gnomon-x")
            subprocess.run(["cargo",  "build"])
            os.chdir(args.path)

        # Updating gnomon-packages
        futures += _plugin_package_update(glob.glob("**/gnomon-package-*", recursive=True), executor)

        # Waiting for completion
        concurrent.futures.wait(futures)


if __name__ == '__main__':
    main()
