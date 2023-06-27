import argparse
from gnomon.utils import gnomonTemplateUtils


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


def run(args: argparse.Namespace):
    from gnomon.utils.pipelines import load_pipeline
    pr = load_pipeline(args.pipeline, args.data_dir)
    if args.ios:
        nodes = args.ios[::2]
        paths = args.ios[1::2]
        for node_name, path in zip(nodes, paths):
            pr.path_dict[node_name] = path
    pr.run()


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


# pipeline command
generate_parser = subparsers.add_parser(
    "run",
    description=f"""
    Run a pipeline from a pipeline file.
    
    Example:
    gnomon-utils run ./my_pipeline.json --ios imageReader1 ./my_image.inr imageReader2 ./other_image.inr
    """
)
generate_parser.set_defaults(func=run)
generate_parser.add_argument("pipeline", help="Path to the pipeline file")
generate_parser.add_argument("--data-dir", type=str, default="", dest="data_dir",
                             help="Path to the directory containing the data")
generate_parser.add_argument("--ios", action="extend", nargs="+", type=str,
                             help="List of pairs of node names / path to file.")
