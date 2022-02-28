import logging

from matplotlib import cm, colors
from matplotlib.colors import to_rgb
import matplotlib.pyplot as plt

from collections.abc import Iterable

from gnomon.visualization import ParameterColorMap


def _rgb_color(color):
    col = None
    if isinstance(color, str):
        if color[0] == "#":
            col = colors.hex2color(color)
        elif color in colors.get_named_colors_mapping().keys():
            col = to_rgb(colors.get_named_colors_mapping()[color])
    elif isinstance(color, Iterable):
        if any([c>1 for c in color]):
            col = [c/255. for c in color]
        else:
            col = color
    return col


def mpl_colormap(colormap, name=None, reverse=False):
    color_dict = dict(red=[], green=[], blue=[])
    for pos, color in colormap.items():
        col = _rgb_color(color)
        if reverse:
            pos = 1-pos
        color_dict['red'] += [(pos, col[0], col[0])]
        color_dict['green'] += [(pos, col[1], col[1])]
        color_dict['blue'] += [(pos, col[2], col[2])]
    return colors.LinearSegmentedColormap(name, color_dict)


def register_gnomon_colormaps():
    p = ParameterColorMap()
    mpl_cmaps = plt.colormaps()

    for colormap_name in p.availableCluts():
        if colormap_name not in mpl_cmaps:
            p.setValue(colormap_name)
            colormap = p.value()
            cm.register_cmap(colormap_name, mpl_colormap(colormap, colormap_name))
            logging.info(f"Successfully registered {colormap_name} colormap to matplotlib.cm")
            cm.register_cmap(colormap_name+"_r", mpl_colormap(colormap, colormap_name+"_r", reverse=True))
            logging.info(f"Successfully registered {colormap_name}_r colormap to matplotlib.cm")
        else:
            logging.debug(f"Colormap {colormap_name}_r already registered in matplotlib.cm")
