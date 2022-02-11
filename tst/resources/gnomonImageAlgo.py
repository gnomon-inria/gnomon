import numpy as np
import scipy.ndimage as nd

from dtkcore import d_bool, d_int, d_real, d_inliststring, d_inliststringlist

import gnomon.core

from gnomon.utils import load_plugin_group, corePlugin
from gnomon.utils.decorators import imageInput, imageOutput

from timagetk.components import SpatialImage

@corePlugin(version='0.1.0', coreversion='0.20.0')
@imageInput(attr='in_img')
@imageOutput(attr='out_img')
class imageAlgorithm(gnomoncore.gnomonAbstractFormAlgorithm):

    def __init__(self):
        super().__init__()

        self._parameters = {}
        self._parameters['sigma'] = d_real("The sigma parameter", 1., 0., 5., 2)

        self.in_img = {}
        self.out_img = {}

    def run(self):
        self.out_img = {}

        for time in self.in_img.keys():
            self.out_img[time] = {}

            for channel in self.in_img[time].keys():
                img = self.in_img[time][channel]
                filtered_img = nd.gaussian_filter(img.get_array(),sigma=self['sigma']/np.array(img.voxelsize))
                self.out_img[time][channel] = SpatialImage(filtered_img,voxelsize=img.voxelsize)

