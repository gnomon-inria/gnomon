import sys
import argparse
from gnomon_utils import gnomonTemplateUtils


def main():
    try:
        args = main_parser.parse_args()
    except TypeError:
        main_parser.print_help()
        return 0
    args.func(args)


def generate(args: argparse.Namespace):
    if args.inpath:
        gnomonTemplateUtils.schematic_reader_from_path(
            args.inpath,
            args.outpath,
            {},
        )
    else:
        try:
            schematic_path = schematic_list[args.schematic]
        except KeyError:
            raise KeyError(f"Invalid schematic name. Expected one of: \n"
                           f"{list(schematic_list.keys())}")
        gnomonTemplateUtils.schematic_reader_from_path(
            schematic_path,
            args.outpath,
            {},
        )


# setting parsers
main_parser = argparse.ArgumentParser(prog="gnomon-utils", description="Gnomon command line tools")
subparsers = main_parser.add_subparsers(required=True)


# generate command
schematic_list = gnomonTemplateUtils.register_known_schematics()
generate_parser = subparsers.add_parser(
    "generate",
    description=f"""
    Generate a project following a schematic.
    
    Available schematics: {list(schematic_list.keys())}
    """
)
generate_parser.set_defaults(func=generate)
generate_parser.add_argument("schematic", help="Name of the schematic")
generate_parser.add_argument("outpath", help="Target folder where the files will be generated")
generate_parser.add_argument("--inpath", help="Optional path to a json schematic that will be used instead")
