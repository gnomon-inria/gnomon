import json
import subprocess
import pathlib
import os
from datetime import datetime
from importlib.metadata import version, PackageNotFoundError
from typing import Optional
from dataclasses import dataclass
from functools import partial

from packaging.version import parse as parse_version
from conda.models.match_spec import MatchSpec, VersionSpec


CONDA_EXE = "mamba" if pathlib.Path(os.getenv("CONDA_EXE")).with_name("mamba").exists() else "conda"
INSTALL_CHANNELS = ["-c", "gnomon", "-c", "mosaic", "-c", "conda-forge", "-c", "dtk-forge6", "-c", "morpheme"]


try:
    GNOMON_VERSION = parse_version(version("gnomon"))
except PackageNotFoundError:
    GNOMON_VERSION = parse_version(version("libgnomon"))

@dataclass
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

    def __new__(cls, *args, **kwargs):
        for key in dir(cls):
            if key.isupper():
                setattr(cls, key.lower(), partial(cls._apply_code_arround, code=getattr(cls, key)))

    @classmethod
    def values(cls) -> list[str]:
        return [cls.HEADER, cls.OKBLUE, cls.OKCYAN, cls.OKGREEN, cls.WARNING, cls.FAIL, cls.ENDC,
                cls.BOLD, cls.UNDERLINE]

    @classmethod
    def _apply_code_arround(cls, string: str, code: str) -> str:
        return code + string + (cls.ENDC if not string.endswith(cls.ENDC) else "")



def installed_packages():
    completed_process = subprocess.run(
        [CONDA_EXE, "list", "gnomon", "--json"],
        capture_output=True,
        encoding="utf-8"
    )
    out = json.loads(completed_process.stdout)
    return [(package["name"], package["version"], package["build_string"]) for package in out]


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
            def package_info_func(spec):
                return (
                    package_name, package_info["version"],
                    package_info["build"],
                    datetime.fromtimestamp(package_info["timestamp"]/1000),
                    spec[0],
                    url
                    )
            if GNOMON_VERSION < parse_version("1.0.0a1"):
                # gnomon is split between app and lib
                # package require libgnomon
                if gnomon_spec:
                    upgrade_needed_packages.append(
                        package_info_func(gnomon_spec)
                    )
                elif libgnomon_spec and MatchSpec(libgnomon_spec[0]).version.match(str(GNOMON_VERSION)):
                    as_is_packages.append(
                        package_info_func(libgnomon_spec)
                    )
                elif libgnomon_spec:
                    upgrade_needed_packages.append(
                        package_info_func(libgnomon_spec)
                    )
            else:
                # only one package for gnomon
                if libgnomon_spec:
                    upgrade_needed_packages.append(
                        package_info_func(libgnomon_spec)
                    )
                elif gnomon_spec and MatchSpec(gnomon_spec[0]).version.match(str(GNOMON_VERSION)):
                    as_is_packages.append(
                        package_info_func(gnomon_spec)
                    )
                elif gnomon_spec:
                    upgrade_needed_packages.append(
                        package_info_func(gnomon_spec)
                    )

    # sorting by name (asc) then version (desc) then date (desc)
    as_is_packages.sort(key=lambda x: x[3], reverse=True)
    as_is_packages.sort(key=lambda x: parse_version(x[1]), reverse=True)
    as_is_packages.sort(key=lambda x: x[0])

    # changing datetime to str representation
    for i, package_info in enumerate(as_is_packages):
        name, vers, build, date, spec, url = package_info
        as_is_packages[i] = (name, vers, build, date.strftime("%d/%m/%Y %H:%M:%S"), spec, url)
    for i, package_info in enumerate(upgrade_needed_packages):
        name, vers, build, date, spec, url = package_info
        upgrade_needed_packages[i] = (name, vers, build, date.strftime("%d/%m/%Y %H:%M:%S"), spec, url)

    # inserting empty row between packets with different names
    new_list = []
    last_packet_name = ""
    for i, package_info in enumerate(as_is_packages):
        name = package_info[0]
        if name != last_packet_name and last_packet_name:
            new_list.append((" ", " ", " ", " ", " ", " "))
        new_list.append(package_info)
        last_packet_name = name
    as_is_packages = new_list
    new_list = []
    last_packet_name = ""
    for i, package_info in enumerate(upgrade_needed_packages):
        name = package_info[0]
        if name != last_packet_name and last_packet_name:
            new_list.append(("", "", "", "", "", ""))
        new_list.append(package_info)
        last_packet_name = name
    upgrade_needed_packages = new_list

    # checking if package already installed
    # TODO: breaks tabs length
    packages = installed_packages()
    for i, package_info in enumerate(as_is_packages):
        name, vers, build, date, spec, url = package_info
        if (name, vers, build) in packages:
            as_is_packages[i] = tuple(bcolors.OKGREEN+info+bcolors.ENDC for info in package_info)

    for i, package_info in enumerate(upgrade_needed_packages):
        name, vers, build, date, spec, url = package_info
        if (name, vers, build) in packages:
            upgrade_needed_packages[i] = tuple(bcolors.OKGREEN+info+bcolors.ENDC for info in package_info)

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


def print_available_packages(all: bool = False):
    as_is_packages, upgrade_needed_packages = available_packages()
    print("The current version of gnomon is", bcolors.BOLD + str(GNOMON_VERSION) + bcolors.ENDC)
    print("")
    if as_is_packages:
        print("The following packages are compatible with the", bcolors.BOLD + "current version" + bcolors.ENDC,
              "of gnomon")
        print("")
        print_table(
            as_is_packages,
            header=["Package name", "Version", "Build", "Upload date", "Required Gnomon version", "url"]
        )
    print("\n")
    if upgrade_needed_packages and all:
        print("The following packages exists but require a", bcolors.BOLD + "different version" + bcolors.ENDC, "of gnomon")
        print("")
        print_table(
            upgrade_needed_packages,
            header=["Package name", "Version", "Build", "Upload date", "Required Gnomon version", "url"]
        )


def install_package(packages: list[str]):
    packages = [
        package_name.replace("-", "_") if package_name.startswith("gnomon-package-") else package_name
        for package_name in packages
    ]
    subprocess.run([CONDA_EXE, "install"] + packages + INSTALL_CHANNELS)

