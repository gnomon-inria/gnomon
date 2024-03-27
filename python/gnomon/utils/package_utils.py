import json
import subprocess
import pathlib
import os
from importlib.metadata import version, PackageNotFoundError
from typing import Optional

from packaging.version import parse as parse_version
from conda.models.match_spec import MatchSpec, VersionSpec


CONDA_EXE = "mamba" if pathlib.Path(os.getenv("CONDA_EXE")).with_name("mamba").exists() else "conda"
INSTALL_CHANNELS = ["-c", "gnomon", "-c", "mosaic", "-c", "conda-forge", "-c", "dtk-forge6", "-c", "morpheme"]


try:
    GNOMON_VERSION = parse_version(version("gnomon"))
except PackageNotFoundError:
    GNOMON_VERSION = parse_version(version("libgnomon"))

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def installed_packages():
    completed_process = subprocess.run(
        [CONDA_EXE, "list", "gnomon", "--json"],
        capture_output=True,
        encoding="utf-8"
    )
    out = json.loads(completed_process.stdout)
    return [(package["name"], package["version"]) for package in out]


def update(packages: list[str]):
    if packages:
        subprocess.run([CONDA_EXE, "update"] + packages + INSTALL_CHANNELS)
    else:
        print("Looking for installed gnomon packages")
        packages = list(list(zip(*installed_packages()))[0])
        if "libgnomon" in packages:
            packages.remove("libgnomon")
        if "gnomon" not in packages:
            packages.append("gnomon")
        packages = [
            package_name.replace("-", "_") if package_name.startswith("gnomon-package-") else package_name
            for package_name in packages
        ]
        subprocess.run([CONDA_EXE, "update"] + packages + INSTALL_CHANNELS)


def available_packages():
    completed_process = subprocess.run(
        ["conda", "search", "-c", "gnomon", "--override-channels", "--json", "gnomon_package*"],
        capture_output=True,
        encoding="utf-8"
    )
    out = json.loads(completed_process.stdout)

    as_is_packages = []
    upgrade_needed_packages = []
    for package_name, package_info_list in out.items():
        for package_info in package_info_list:
            gnomon_spec: list[str] = [
                spec for spec in package_info["depends"] if spec.startswith("gnomon ")
            ]
            libgnomon_spec: list[str] = [
                spec for spec in package_info["depends"] if spec.startswith("libgnomon ")
            ]
            url = f"https://anaconda.org/gnomon/{package_name}"
            if GNOMON_VERSION < parse_version("1.0.0a1"):
                # gnomon is split between app and lib
                # package require libgnomon
                if gnomon_spec:
                    upgrade_needed_packages.append(
                        (package_name, package_info["version"], gnomon_spec[0], url)
                    )
                elif libgnomon_spec and MatchSpec(libgnomon_spec[0]).version.match(str(GNOMON_VERSION)):
                    as_is_packages.append(
                        (package_name, package_info["version"], libgnomon_spec[0], url)
                    )
                elif libgnomon_spec:
                    upgrade_needed_packages.append(
                        (package_name, package_info["version"], libgnomon_spec[0], url)
                    )
            else:
                # only one package for gnomon
                if libgnomon_spec:
                    upgrade_needed_packages.append(
                        (package_name, package_info["version"], libgnomon_spec[0], url)
                    )
                elif gnomon_spec and MatchSpec(gnomon_spec[0]).version.match(str(GNOMON_VERSION)):
                    as_is_packages.append(
                        (package_name, package_info["version"], gnomon_spec[0], url)
                    )
                elif gnomon_spec:
                    upgrade_needed_packages.append(
                        (package_name, package_info["version"], gnomon_spec[0], url)
                    )

    return as_is_packages, upgrade_needed_packages

def print_table(table, header: Optional[list[str]] = None):
    """
    From StackOverflow https://stackoverflow.com/a/52247284
    """
    if header:
        longest_cols = [len(max(col, key=len))+3 for col in zip(*([header] + table))]
        separators = ["-"*(length-3) for length in longest_cols]
        table = [header] + [separators] + table
    else:
        longest_cols = [len(max(col, key=len))+3 for col in zip(*table)]
    row_format = "".join(["{:<" + str(longest_col) + "}" for longest_col in longest_cols])
    for row in table:
        print(row_format.format(*row))


def print_available_packages():
    as_is_packages, upgrade_needed_packages = available_packages()
    print("The current version of gnomon is", bcolors.BOLD + str(GNOMON_VERSION) + bcolors.ENDC)
    print("")
    if as_is_packages:
        print("The following packages are compatible with the", bcolors.BOLD + "current version" + bcolors.ENDC,
              "of gnomon")
        print("")
        print_table(as_is_packages, header=["Package name", "Version", "Required gnomon version", "url"])
    print("\n")
    if upgrade_needed_packages:
        print("The following packages exists but require a", bcolors.BOLD + "different version" + bcolors.ENDC, "of gnomon")
        print("")
        print_table(upgrade_needed_packages, header=["Package name", "Version", "Required gnomon version", "url"])


def install_package(packages: list[str]):
    packages = [
        package_name.replace("-", "_") if package_name.startswith("gnomon-package-") else package_name
        for package_name in packages
    ]
    subprocess.run([CONDA_EXE, "install"] + packages + INSTALL_CHANNELS)

